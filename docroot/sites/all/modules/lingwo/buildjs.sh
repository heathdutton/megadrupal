#!/bin/bash

SCRIPTDIR=`dirname $PWD/$0`
. $SCRIPTDIR/buildjs-config.sh

STUBS=$SCRIPTDIR/js/require-stubs.js

if which node; then
	BUILDSH=$REQUIREJS/build/build.sh
elif which java; then
	BUILDSH=$REQUIREJS/build/buildj.sh
else
	echo "You must install either node 0.4.x or java to run the build!"
	exit 1
fi

$BUILDSH $1

