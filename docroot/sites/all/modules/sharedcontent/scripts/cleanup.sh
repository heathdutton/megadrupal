#!/bin/sh
# Clean up script to remove the project files when done.

chmod -R 777 $BUILD_TARGET
rm -R $BUILD_TARGET
