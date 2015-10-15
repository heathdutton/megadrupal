#!/usr/bin/env php
<?php

/**
 * Sprites Generator
 *
 * Check for your PHP interpreter - on Windows you'll probably have to
 * replace line 1 with
 *   #!c:/program files/php/php.exe
 */


$dir = '.';
$resulteFile = '../icons.png';
//Check if GD extension is loaded.
if (!extension_loaded('gd') && !extension_loaded('gd2')) {
  exit('GD is not loaded!');
}

$max_width = $newHeight = 0;
echo str_repeat('-', 50) . "\n";
foreach (scandir($dir) as $file) {
  if (pathinfo($file, PATHINFO_EXTENSION) == 'png') {
    $image = new stdClass;
    $image->handler = imagecreatefrompng("$dir/$file");
    $image->file = $file;
    $image->width = imagesx($image->handler);
    $image->height = imagesy($image->handler);
    echo "Add $file ({$image->width}x{$image->height})\n";
    $images[] = $image;
    $newWidth = $image->width > $max_width ? $image->width : $max_width;
    $newHeight += $image->height;
  }
}
echo str_repeat('-', 50) . "\n";

$cnt = count($images);


echo "Added images: $cnt \n";
echo "Result image: $resulteFile ({$newWidth}x{$newHeight})\n";

$newImage = imagecreatetruecolor($newWidth, $newHeight);
imagealphablending($newImage, FALSE);
imagesavealpha($newImage, TRUE);
$transparent = imagecolorallocatealpha($newImage, 255, 255, 255, 127);
imagefilledrectangle($newImage, 0, 0, $newWidth, $newHeight, $transparent);

$current_height = 0;
for ($i = 0; $i < $cnt; $i++) {
  imagecopyresampled(
    $newImage,
    $images[$i]->handler,
    0,
    $current_height,
    0,
    0,
    $images[$i]->width,
    $images[$i]->height,
    $images[$i]->width,
    $images[$i]->height
  );
  $current_height += $images[$i]->height;
}
imagepng($newImage , $resulteFile);
echo 'Complete...';