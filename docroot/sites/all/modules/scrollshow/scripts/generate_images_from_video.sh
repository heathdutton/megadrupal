#!/bin/bash

VIDEO=$1
DIRS="lo-res hi-res"

if ! test -f "$VIDEO"; then
	echo "Cannot find input file: $VIDEO"
	exit 1
fi

for d in $DIRS; do
	if test -e "$d"; then
		echo "$d: Directory already exists - delete first."
		exit 1
	fi
done

for d in $DIRS; do
	mkdir $d
done

# make hi-res version (not totally hi-res, JPG is lossy)
ffmpeg -i $VIDEO -f image2 -sameq hi-res/image-%04d.jpg

# loop through and convert hi-res to lo-res
HIRES_IMAGES=`cd hi-res && ls *.jpg`
for INPUT in $HIRES_IMAGES; do
	#OUTPUT=lo-res/`echo $INPUT | sed -e s/\.jpg$/.png/`
	OUTPUT=lo-res/$INPUT
	INPUT=hi-res/$INPUT

	convert $INPUT \
		-set colorspace Gray -separate -average \
		-resize 320x180 \
		-quality 100 \
		$OUTPUT
done

