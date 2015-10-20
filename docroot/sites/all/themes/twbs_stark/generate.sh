#!/bin/bash

# This script is used by the MAINTAINERS.

set -o xtrace

ORIG=`pwd`
STARTERKIT=../STARTERKIT
TWBS=../../../../modules/contrib/twbs

# Copy all template files from TWBS.
find $TWBS/alter -type f -name '*.tpl.php' -exec cp {} ../templates/ \;
