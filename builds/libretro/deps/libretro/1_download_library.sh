#!/bin/bash

# abort on errors
set -e

export WORKSPACE=$PWD

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source $SCRIPT_DIR/../shared/import.sh

msg " [1] Downloading generic libraries"

# zlib
rm -rf $ZLIB_DIR
download_and_extract $ZLIB_URL

# libpng
rm -rf $LIBPNG_DIR
download_and_extract $LIBPNG_URL

# freetype
rm -rf $FREETYPE_DIR
download_and_extract $FREETYPE_URL

# harfbuzz
rm -rf $HARFBUZZ_DIR
download_and_extract $HARFBUZZ_URL

# pixman
rm -rf $PIXMAN_DIR
download_and_extract $PIXMAN_URL

# expat
rm -rf $EXPAT_DIR
download_and_extract $EXPAT_URL

# libogg
rm -rf $LIBOGG_DIR
download_and_extract $LIBOGG_URL

# libvorbis
rm -rf $LIBVORBIS_DIR
download_and_extract $LIBVORBIS_URL

# mpg123
rm -rf $MPG123_DIR
download_and_extract $MPG123_URL

# libsndfile
rm -rf $LIBSNDFILE_DIR
download_and_extract $LIBSNDFILE_URL

# libxmp-lite
rm -rf $LIBXMP_LITE_DIR
download_and_extract $LIBXMP_LITE_URL

# libsamplerate
rm -rf $LIBSAMPLERATE_DIR
download_and_extract $LIBSAMPLERATE_URL

# wildmidi
rm -rf $WILDMIDI_DIR
download_and_extract $WILDMIDI_URL

# opus
rm -rf $OPUS_DIR
download_and_extract $OPUS_URL

# opusfile
rm -rf $OPUSFILE_DIR
download_and_extract $OPUSFILE_URL

# ICU
rm -rf $ICU_DIR
download_and_extract $ICU_URL

# icudata
rm -f $ICUDATA_FILES
download_and_extract $ICUDATA_URL
