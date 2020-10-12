// LZW streaming compressor/decompressor based on LZW AB.
// Copyright (c) 2016 David Bryant, 2018+ other authors, all rights reserved (see AUTHORS).
// Distributed under the BSD Software License (see LICENSE).

#include "file.h"

#include <string.h>

#include "buffer.h"
#include "compressor/main.h"
#include "decompressor/main.h"
#include "log.h"

// -- file --

static inline lzws_result_t read_file(
  FILE*        source_file,
  lzws_byte_t* source_buffer,
  size_t*      source_length_ptr,
  size_t       source_buffer_length,
  bool         quiet)
{
  size_t read_length = fread(source_buffer, 1, source_buffer_length, source_file);
  if (read_length == 0 && feof(source_file)) {
    return LZWS_FILE_READ_FINISHED;
  }

  if (read_length != source_buffer_length && ferror(source_file)) {
    if (!quiet) {
      LZWS_LOG_ERROR("read file failed");
    }

    return LZWS_FILE_READ_FAILED;
  }

  *source_length_ptr = read_length;

  return 0;
}

static inline lzws_result_t
  write_file(FILE* destination_file, lzws_byte_t* destination_buffer, size_t destination_length, bool quiet)
{
  size_t written_length = fwrite(destination_buffer, 1, destination_length, destination_file);
  if (written_length != destination_length) {
    if (!quiet) {
      LZWS_LOG_ERROR("write file failed");
    }

    return LZWS_FILE_WRITE_FAILED;
  }

  return 0;
}

// -- buffer --

// We have read some source from file into source buffer.
// Than algorithm has read part of this source.
// We need to move remaining source to the top of source buffer.
// Than we can read more source from file.
// Algorithm can use same buffer again.

static inline lzws_result_t read_more_source(
  FILE*         source_file,
  lzws_byte_t** source_ptr,
  size_t*       source_length_ptr,
  lzws_byte_t*  source_buffer,
  size_t        source_buffer_length,
  bool          quiet)
{
  const lzws_byte_t* source        = *source_ptr;
  size_t             source_length = *source_length_ptr;

  if (source != source_buffer) {
    if (source_length != 0) {
      memmove(source_buffer, source, source_length);
    }

    // Source can be accessed even if next code will fail.
    *source_ptr = source_buffer;
  }

  size_t remaining_source_buffer_length = source_buffer_length - source_length;
  if (remaining_source_buffer_length == 0) {
    // We want to read more data at once, than buffer has.
    if (!quiet) {
      LZWS_LOG_ERROR("not enough source buffer, length: %zu", source_buffer_length);
    }

    return LZWS_FILE_NOT_ENOUGH_SOURCE_BUFFER;
  }

  lzws_byte_t* remaining_source_buffer = source_buffer + source_length;
  size_t       new_source_length;

  lzws_result_t result =
    read_file(source_file, remaining_source_buffer, &new_source_length, remaining_source_buffer_length, quiet);

  if (result != 0) {
    return result;
  }

  *source_length_ptr = source_length + new_source_length;

  return 0;
}

#define BUFFERED_READ_SOURCE(function, ...)                                                                        \
  do {                                                                                                             \
    bool is_function_called = false;                                                                               \
                                                                                                                   \
    while (true) {                                                                                                 \
      result = read_more_source(source_file, &source, &source_length, source_buffer, source_buffer_length, quiet); \
      if (result == LZWS_FILE_READ_FINISHED) {                                                                     \
        /* Some UNIX compress implementations write alignment at the end of the file. */                           \
        /* So source length here can not be zero here, we have to ignore it. */                                    \
        break;                                                                                                     \
      } else if (result != 0) {                                                                                    \
        return result;                                                                                             \
      }                                                                                                            \
                                                                                                                   \
      result = function(__VA_ARGS__);                                                                              \
      if (result != 0) {                                                                                           \
        return result;                                                                                             \
      }                                                                                                            \
                                                                                                                   \
      is_function_called = true;                                                                                   \
    }                                                                                                              \
                                                                                                                   \
    if (!is_function_called) {                                                                                     \
      /* Function should be called at least once. */                                                               \
      result = function(__VA_ARGS__);                                                                              \
      if (result != 0) {                                                                                           \
        return result;                                                                                             \
      }                                                                                                            \
    }                                                                                                              \
  } while (false);

// Algorithm has written data into destination buffer.
// We need to write this data into file.
// Than algorithm can use same buffer again.

static inline lzws_result_t flush_destination_buffer(
  FILE*        destination_file,
  lzws_byte_t* destination_buffer,
  size_t*      destination_length_ptr,
  size_t       destination_buffer_length,
  bool         quiet)
{
  if (*destination_length_ptr == 0) {
    // We want to write more data at once, than buffer has.
    if (!quiet) {
      LZWS_LOG_ERROR("not enough destination buffer, length: %zu", destination_buffer_length);
    }

    return LZWS_FILE_NOT_ENOUGH_DESTINATION_BUFFER;
  }

  lzws_result_t result = write_file(destination_file, destination_buffer, *destination_length_ptr, quiet);
  if (result != 0) {
    return result;
  }

  *destination_length_ptr = 0;

  return 0;
}

static inline lzws_result_t write_remaining_destination(
  FILE*        destination_file,
  lzws_byte_t* destination_buffer,
  size_t       destination_length,
  bool         quiet)
{
  if (destination_length == 0) {
    return 0;
  }

  return write_file(destination_file, destination_buffer, destination_length, quiet);
}

// -- compress --

#define BUFFERED_COMPRESS(function, ...)                                                                         \
  while (true) {                                                                                                 \
    lzws_byte_t* remaining_destination_buffer             = destination_buffer + *destination_length_ptr;        \
    size_t       remaining_destination_buffer_length      = destination_buffer_length - *destination_length_ptr; \
    size_t       prev_remaining_destination_buffer_length = remaining_destination_buffer_length;                 \
                                                                                                                 \
    result = function(__VA_ARGS__, &remaining_destination_buffer, &remaining_destination_buffer_length);         \
    if (result != 0 && result != LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION) {                                       \
      return LZWS_FILE_COMPRESSOR_UNEXPECTED_ERROR;                                                              \
    }                                                                                                            \
                                                                                                                 \
    *destination_length_ptr += prev_remaining_destination_buffer_length - remaining_destination_buffer_length;   \
                                                                                                                 \
    if (result == LZWS_COMPRESSOR_NEEDS_MORE_DESTINATION) {                                                      \
      result = flush_destination_buffer(                                                                         \
        destination_file, destination_buffer, destination_length_ptr, destination_buffer_length, quiet);         \
                                                                                                                 \
      if (result != 0) {                                                                                         \
        return result;                                                                                           \
      }                                                                                                          \
                                                                                                                 \
      continue;                                                                                                  \
    }                                                                                                            \
                                                                                                                 \
    break;                                                                                                       \
  }

static inline lzws_result_t buffered_compress(
  lzws_compressor_state_t* state_ptr,
  lzws_byte_t**            source_ptr,
  size_t*                  source_length_ptr,
  FILE*                    destination_file,
  lzws_byte_t*             destination_buffer,
  size_t*                  destination_length_ptr,
  size_t                   destination_buffer_length,
  bool                     quiet)
{
  lzws_result_t result;

  BUFFERED_COMPRESS(lzws_compress, state_ptr, source_ptr, source_length_ptr);

  return 0;
}

static inline lzws_result_t buffered_compressor_finish(
  lzws_compressor_state_t* state_ptr,
  FILE*                    destination_file,
  lzws_byte_t*             destination_buffer,
  size_t*                  destination_length_ptr,
  size_t                   destination_buffer_length,
  bool                     quiet)
{
  lzws_result_t result;

  BUFFERED_COMPRESS(lzws_compressor_finish, state_ptr);

  return 0;
}

static inline lzws_result_t compress(
  lzws_compressor_state_t* state_ptr,
  FILE*                    source_file,
  lzws_byte_t*             source_buffer,
  size_t                   source_buffer_length,
  FILE*                    destination_file,
  lzws_byte_t*             destination_buffer,
  size_t                   destination_buffer_length,
  bool                     quiet)
{
  lzws_result_t result;
  lzws_byte_t*  source             = source_buffer;
  size_t        source_length      = 0;
  size_t        destination_length = 0;

  BUFFERED_READ_SOURCE(
    buffered_compress,
    state_ptr,
    &source,
    &source_length,
    destination_file,
    destination_buffer,
    &destination_length,
    destination_buffer_length,
    quiet);

  result = buffered_compressor_finish(
    state_ptr, destination_file, destination_buffer, &destination_length, destination_buffer_length, quiet);

  if (result != 0) {
    return result;
  }

  return write_remaining_destination(destination_file, destination_buffer, destination_length, quiet);
}

lzws_result_t lzws_compress_file(
  FILE*                            source_file,
  size_t                           source_buffer_length,
  FILE*                            destination_file,
  size_t                           destination_buffer_length,
  const lzws_compressor_options_t* options_ptr)
{
  lzws_compressor_state_t* state_ptr;

  lzws_result_t result = lzws_compressor_get_initial_state(&state_ptr, options_ptr);
  if (result != 0) {
    switch (result) {
      case LZWS_COMPRESSOR_ALLOCATE_FAILED:
        return LZWS_FILE_ALLOCATE_FAILED;
      case LZWS_COMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH:
        return LZWS_FILE_VALIDATE_FAILED;
      default:
        return LZWS_FILE_COMPRESSOR_UNEXPECTED_ERROR;
    }
  }

  lzws_byte_t* source_buffer;

  result = lzws_create_source_buffer_for_compressor(&source_buffer, &source_buffer_length, state_ptr->options.quiet);
  if (result != 0) {
    lzws_compressor_free_state(state_ptr);
    return LZWS_FILE_ALLOCATE_FAILED;
  }

  lzws_byte_t* destination_buffer;

  result = lzws_create_destination_buffer_for_compressor(
    &destination_buffer, &destination_buffer_length, state_ptr->options.quiet);

  if (result != 0) {
    free(source_buffer);
    lzws_compressor_free_state(state_ptr);
    return LZWS_FILE_ALLOCATE_FAILED;
  }

  result = compress(
    state_ptr,
    source_file,
    source_buffer,
    source_buffer_length,
    destination_file,
    destination_buffer,
    destination_buffer_length,
    state_ptr->options.quiet);

  free(source_buffer);
  free(destination_buffer);
  lzws_compressor_free_state(state_ptr);

  return result;
}

// -- decompress --

static inline lzws_result_t buffered_decompress(
  lzws_decompressor_state_t* state_ptr,
  lzws_byte_t**              source_ptr,
  size_t*                    source_length_ptr,
  FILE*                      destination_file,
  lzws_byte_t*               destination_buffer,
  size_t*                    destination_length_ptr,
  size_t                     destination_buffer_length,
  bool                       quiet)
{
  lzws_result_t result;

  while (true) {
    lzws_byte_t* remaining_destination_buffer             = destination_buffer + *destination_length_ptr;
    size_t       remaining_destination_buffer_length      = destination_buffer_length - *destination_length_ptr;
    size_t       prev_remaining_destination_buffer_length = remaining_destination_buffer_length;

    result = lzws_decompress(
      state_ptr, source_ptr, source_length_ptr, &remaining_destination_buffer, &remaining_destination_buffer_length);

    if (result != 0 && result != LZWS_DECOMPRESSOR_NEEDS_MORE_DESTINATION) {
      switch (result) {
        case LZWS_DECOMPRESSOR_INVALID_MAGIC_HEADER:
        case LZWS_DECOMPRESSOR_INVALID_MAX_CODE_BIT_LENGTH:
          return LZWS_FILE_VALIDATE_FAILED;
        case LZWS_DECOMPRESSOR_CORRUPTED_SOURCE:
          return LZWS_FILE_DECOMPRESSOR_CORRUPTED_SOURCE;
        default:
          return LZWS_FILE_DECOMPRESSOR_UNEXPECTED_ERROR;
      }
    }

    *destination_length_ptr += prev_remaining_destination_buffer_length - remaining_destination_buffer_length;

    if (result == LZWS_DECOMPRESSOR_NEEDS_MORE_DESTINATION) {
      result = flush_destination_buffer(
        destination_file, destination_buffer, destination_length_ptr, destination_buffer_length, quiet);

      if (result != 0) {
        return result;
      }

      continue;
    }

    break;
  }

  return 0;
}

static inline lzws_result_t decompress(
  lzws_decompressor_state_t* state_ptr,
  FILE*                      source_file,
  lzws_byte_t*               source_buffer,
  size_t                     source_buffer_length,
  FILE*                      destination_file,
  lzws_byte_t*               destination_buffer,
  size_t                     destination_buffer_length,
  bool                       quiet)
{
  lzws_result_t result;
  lzws_byte_t*  source             = source_buffer;
  size_t        source_length      = 0;
  size_t        destination_length = 0;

  BUFFERED_READ_SOURCE(
    buffered_decompress,
    state_ptr,
    &source,
    &source_length,
    destination_file,
    destination_buffer,
    &destination_length,
    destination_buffer_length,
    quiet);

  return write_remaining_destination(destination_file, destination_buffer, destination_length, quiet);
}

lzws_result_t lzws_decompress_file(
  FILE*                              source_file,
  size_t                             source_buffer_length,
  FILE*                              destination_file,
  size_t                             destination_buffer_length,
  const lzws_decompressor_options_t* options_ptr)
{
  lzws_decompressor_state_t* state_ptr;

  lzws_result_t result = lzws_decompressor_get_initial_state(&state_ptr, options_ptr);
  if (result != 0) {
    switch (result) {
      case LZWS_DECOMPRESSOR_ALLOCATE_FAILED:
        return LZWS_FILE_ALLOCATE_FAILED;
      default:
        return LZWS_FILE_DECOMPRESSOR_UNEXPECTED_ERROR;
    }
  }

  lzws_byte_t* source_buffer;

  result = lzws_create_source_buffer_for_decompressor(&source_buffer, &source_buffer_length, state_ptr->options.quiet);
  if (result != 0) {
    lzws_decompressor_free_state(state_ptr);
    return LZWS_FILE_ALLOCATE_FAILED;
  }

  lzws_byte_t* destination_buffer;

  result = lzws_create_destination_buffer_for_decompressor(
    &destination_buffer, &destination_buffer_length, state_ptr->options.quiet);

  if (result != 0) {
    free(source_buffer);
    lzws_decompressor_free_state(state_ptr);
    return LZWS_FILE_ALLOCATE_FAILED;
  }

  result = decompress(
    state_ptr,
    source_file,
    source_buffer,
    source_buffer_length,
    destination_file,
    destination_buffer,
    destination_buffer_length,
    state_ptr->options.quiet);

  free(source_buffer);
  free(destination_buffer);
  lzws_decompressor_free_state(state_ptr);

  return result;
}
