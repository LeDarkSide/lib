#!/bin/bash

echoerr() { echo "$@" 1>&2; }

MODULE_PATH="$1"
ACTION="$2"

INFO_PATH="$MODULE_PATH/info.json"
SCRIPT_PATH="$MODULE_PATH/script.sh"

if [ ! -d "$MODULE_PATH" ]; then
	echoerr "Module path is not a directory"
	exit 1
fi

if [ ! -f "$INFO_PATH" ]; then
	echoerr "Info not found"
	exit 1
fi

if [ ! -f "$SCRIPT_PATH" ]; then
	echoerr "Script not found"
	exit 1
fi

MODULE_ID="$(cat "$INFO_PATH" | \
						python3 -c "import sys, json; print(json.load(sys.stdin)['id'])")"

MODULE_CONFIG_PATH=~/.local/share/TheDarkSide/modules/"$MODULE_ID"

source "$SCRIPT_PATH"

cd "$MODULE_PATH"
case $ACTION in
	info)
		_is_installed
		_installed_version
		_last_version
		export IS_INSTALLED
		export INSTALLED_VERSION
		export LAST_VERSION
		echo "IS_INSTALLED=$IS_INSTALLED"
		echo "INSTALLED_VERSION=$INSTALLED_VERSION"
		echo "LAST_VERSION=$LAST_VERSION"
		;;
	install)
		mkdir -p "$MODULE_CONFIG_PATH"
		_install
		;;
	uninstall)
		_uninstall
		rm -fR "$MODULE_CONFIG_PATH"
		;;
	update)
		_update
		;;
	*)
		echo "Unknown action"
		;;
esac
