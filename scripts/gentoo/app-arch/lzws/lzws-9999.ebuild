# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

EAPI="6"

inherit cmake-multilib cmake-utils git-r3

DESCRIPTION="LZW streaming compressor/decompressor compatible with UNIX compress."
HOMEPAGE="https://github.com/andrew-aladev/lzws"
EGIT_REPO_URI="https://github.com/andrew-aladev/lzws.git"
EGIT_SUBMODULES=()

LICENSE="BSD-3-Clause"
SLOT="0"
KEYWORDS=""

IUSE="static test noman"

COMPRESSOR_DICTIONARIES="linked-list sparse-array"
for compressor_dictionary in ${COMPRESSOR_DICTIONARIES}; do
  IUSE+=" compressor_dictionary_${compressor_dictionary}"
done

RDEPEND="
  virtual/libc
  dev-libs/gmp
  !noman? ( app-text/asciidoc )
"
DEPEND="${RDEPEND}"

src_configure() {
  local mycmakeargs=(
    -DLZWS_COMPRESSOR_DICTIONARY=$(usex compressor_dictionary_linked-list linked-list sparse-array)
    -DLZWS_SHARED=ON
    -DLZWS_STATIC=$(usex static)
    -DLZWS_CLI=ON
    -DLZWS_TESTS=$(usex test)
    -DLZWS_EXAMPLES=OFF
    -DLZWS_MAN=$(usex noman OFF ON)
  )

  cmake-multilib_src_configure
}
