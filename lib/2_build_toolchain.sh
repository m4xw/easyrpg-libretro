#!/bin/bash

# abort on errors
set -e

export WORKSPACE=$PWD

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SCRIPT_DIR/import.sh

# Number of CPU
nproc=$(nproc)

# Use ccache?
test_ccache

if [ ! -f .patches-applied ]; then
	echo "patching libraries"

	patches_common

	# Fix mpg123
	pushd $MPG123_DIR
	patch -Np1 < $SCRIPT_DIR/../shared/extra/mpg123.patch
	autoreconf -fi
	popd

	# Fix libsndfile
	pushd $LIBSNDFILE_DIR
	patch -Np1 < $SCRIPT_DIR/../shared/extra/libsndfile.patch
	autoreconf -fi
	popd

	# Wildmidi:
	# Generic patch for all libretro platforms
	pushd $WILDMIDI_DIR
	patch -Np1 < $SCRIPT_DIR/wildmidi-libretro.patch
	popd

        # disable libsamplerate examples and tests
	pushd $LIBSAMPLERATE_DIR
	perl -pi -e 's/examples tests//' Makefile.am
	autoreconf -fi
	popd

	# Fix icu build
	# Generic patch for all libretro platforms
	cp -rup icu icu-native
	patch -Np0 < $SCRIPT_DIR/icu59-libretro.patch

	touch .patches-applied
fi

cd $WORKSPACE

echo "Preparing toolchain"

export PLATFORM_PREFIX=$WORKSPACE

echo "Libretro build configuration:"
echo "TARGET_HOST: $RETRO_TARGET_HOST"
echo "CC: $RETRO_CC"
echo "CXX: $RETRO_CXX"
echo "CFLAGS: $RETRO_CFLAGS"
echo "CXXFLAGS: $RETRO_CXXFLAGS"
echo "CPPFLAGS: $RETRO_CPPFLAGS"
echo "LDFLAGS: $RETRO_LDFLAGS"

export PKG_CONFIG_PATH=$PLATFORM_PREFIX/lib/pkgconfig
export PKG_CONFIG_LIBDIR=$PKG_CONFIG_PATH
export MAKEFLAGS="-j${nproc:-2}"

function set_build_flags {
	export CC="$RETRO_CC"
	export CXX="$RETRO_CXX"
	if [ "$ENABLE_CCACHE" ]; then
		export CC="ccache $CC"
		export CXX="ccache $CXX"
	fi
	export CFLAGS="$RETRO_CFLAGS"
	export CXXFLAGS="$RETRO_CXXFLAGS"
	export CPPFLAGS="-I$PLATFORM_PREFIX/include $RETRO_CPPFLAGS"
	export LDFLAGS="-L$PLATFORM_PREFIX/lib $RETRO_LDFLAGS"
}

# Build native icu59
if [ "$RETRO_CROSSCOMPILING" -eq "1" ]; then
	install_lib_icu_native
fi

# Install libraries
set_build_flags
install_lib_zlib
install_lib $LIBPNG_DIR $LIBPNG_ARGS
#install_lib $FREETYPE_DIR $FREETYPE_ARGS --without-harfbuzz
#install_lib $HARFBUZZ_DIR $HARFBUZZ_ARGS
#install_lib $FREETYPE_DIR $FREETYPE_ARGS --with-harfbuzz
install_lib $PIXMAN_DIR $PIXMAN_ARGS
install_lib_cmake $EXPAT_DIR $EXPAT_ARGS
install_lib $LIBOGG_DIR $LIBOGG_ARGS
install_lib $LIBVORBIS_DIR $LIBVORBIS_ARGS
install_lib $MPG123_DIR $MPG123_ARGS
install_lib $LIBSNDFILE_DIR $LIBSNDFILE_ARGS
install_lib_cmake $LIBXMP_LITE_DIR $LIBXMP_LITE_ARGS
install_lib $LIBSAMPLERATE_DIR $LIBSAMPLERATE_ARGS
install_lib_cmake $WILDMIDI_DIR $WILDMIDI_ARGS
#install_lib $OPUS_DIR $OPUS_ARGS
#install_lib $OPUSFILE_DIR $OPUSFILE_ARGS
if [ "$RETRO_CROSSCOMPILING" -eq "1" ]; then
	install_lib_icu_cross
else
	install_lib $ICU_DIR/source $ICU_ARGS
fi
