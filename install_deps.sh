#!/usr/bin/env bash

set -e

GLEW_VERSION="2.1.0"
GLM_VERSION="0.9.9.8"

MY_PATH="`dirname \"$0\"`"              # relative
MY_PATH="`( cd \"$MY_PATH\" && pwd )`"  # absolutized and normalized
if [ -z "$MY_PATH" ] ; then
  # error; for some reason, the path is not accessible
  # to the script (e.g. permissions re-evaled after suid)
  exit 1  # fail
fi


GLEW_TAR_PATH="$MY_PATH/.temp/unix/glew.tgz"
GLM_TAR_PATH="$MY_PATH/.temp/unix/glm.tar.gz"

GLEW_TEMP_PATH="$MY_PATH/.temp/unix/glew"
GLM_TEMP_PATH="$MY_PATH/.temp/unix/glm"

GLEW_INSTALL_PATH="$MY_PATH/libs/unix/glew"
GLM_INSTALL_PATH="$MY_PATH/libs/unix/glm"

GLEW_TAR_DOWNLOAD_URL="https://iweb.dl.sourceforge.net/project/glew/glew/$GLEW_VERSION/glew-$GLEW_VERSION.tgz?viasf=1"
GLM_TAR_DOWNLOAD_URL="https://github.com/g-truc/glm/archive/$GLM_VERSION.tar.gz"

rm -rf "$MY_PATH/libs"

mkdir -p $GLEW_TEMP_PATH $GLM_TEMP_PATH $GLEW_INSTALL_PATH $GLM_INSTALL_PATH

if [ ! -f "$GLEW_TAR_PATH" ]; then
  wget -O $GLEW_TAR_PATH $GLEW_TAR_DOWNLOAD_URL
fi

if [ ! -f "$GLM_TAR_PATH" ]; then
  wget -O $GLM_TAR_PATH $GLM_TAR_DOWNLOAD_URL
fi

tar xzf $GLEW_TAR_PATH -C $GLEW_TEMP_PATH
tar xzf $GLM_TAR_PATH -C $GLM_TEMP_PATH

mv $GLEW_TEMP_PATH/glew-$GLEW_VERSION/* $GLEW_INSTALL_PATH
mv $GLM_TEMP_PATH/glm-$GLM_VERSION/* $GLM_INSTALL_PATH

(cd $GLEW_INSTALL_PATH && make clean && make)
