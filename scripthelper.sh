#!/bin/bash

echoerr() { echo "$@" 1>&2; }

PACKAGE_PATH="$1"
ACTION="$2"

INFO_PATH="$PACKAGE_PATH/info.json"
SCRIPT_PATH="$PACKAGE_PATH/script.sh"

if [ ! -d "$PACKAGE_PATH" ]; then
  echoerr "Invalid package path: $PACKAGE_PATH"
  exit 1
fi

if [ ! -f "$INFO_PATH" ]; then
  echoerr "The info file does not exist: $INFO_PATH"
  exit 1
fi

if [ ! -f "$SCRIPT_PATH" ]; then
  echoerr "The script file does not exist: $SCRIPT_PATH"
  exit 1
fi

DATA_LOCATION_PATH="${DATA_LOCATION_PATH:-$HOME/.local/share/LeDarkSide}"
REPOSITORIES_PATH="${REPOSITORIES_PATH:-$DATA_LOCATION_PATH/repositories}"
PACKAGES_PATH="${PACKAGES_PATH:-$DATA_LOCATION_PATH/packages}"

PACKAGE_BASENAME="$(basename "$PACKAGE_PATH")"
PACKAGE_ID="$(cat "$INFO_PATH" | \
              python3 -c "import sys, json; print(json.load(sys.stdin)['id'])")"
PACKAGE_DEST="$PACKAGES_PATH/$PACKAGE_BASENAME"

source "$SCRIPT_PATH"

case $ACTION in
  info)
    cd "$PACKAGE_PATH"
    _is_installed
    _installed_version
    _last_version
    echo "IS_INSTALLED=$IS_INSTALLED"
    echo "INSTALLED_VERSION=$INSTALLED_VERSION"
    echo "LAST_VERSION=$LAST_VERSION"
    ;;
  install)
    echo "Installing of $PACKAGE_ID"
    if [ -d "$PACKAGE_DEST" ]; then
      echoerr "A package with the same name ($PACKAGE_BASENAME) is already installed in $PACKAGES_PATH"
      exit 1
    fi
    cp -vr "$PACKAGE_PATH" "$PACKAGES_PATH"
    cd "$PACKAGE_DEST"
    _install
    ;;
  uninstall)
    echo "Uninstalling of $PACKAGE_ID"
    cd "$PACKAGE_PATH"
    _uninstall
    rm -vrf "$PACKAGE_DEST"
    ;;
  update)
    echo "Updating of $PACKAGE_ID"
    cd "$PACKAGE_PATH"
    _update
    ;;
  *)
    echoerr "Unknown action: $ACTION"
    exit 1
    ;;
esac
