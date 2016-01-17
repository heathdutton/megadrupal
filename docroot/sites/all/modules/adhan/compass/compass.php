<?php
  /**
   * @file
   * Generates an image showing a compass with qibla, sun and moon direction.
   */

$gif = isset($_GET['fmt']) && $_GET['fmt'] == 'gif';

// Load source images
$path = dirname(__FILE__);
if ($gif) {
  $g = imagecreatefromgif($path . '/compass.gif');
  $kaaba = imagecreatefromgif($path . '/kaaba.gif');
  $sun = imagecreatefromgif($path . '/sun.gif');
  $moon = imagecreatefromgif($path . '/moon.gif');
}
else {
  $g = imagecreatefrompng($path . '/compass.png');
  $kaaba = imagecreatefrompng($path . '/kaaba.png');
  $sun = imagecreatefrompng($path . '/sun.png');
  $moon = imagecreatefrompng($path . '/moon.png');
}

$width = imagesx($g);
$height = imagesy($g);

$gx = $width / 2;
$gy = $height / 2;

$angle = isset($_GET['qibla']) ? deg2rad($_GET['qibla']) : 0;
$factor = $gx - imagesx($kaaba) / 2;
$kaabax = sin($angle) * $factor + $factor;
$kaabax1 = $kaabax + imagesx($kaaba) / 2;
$kaabax2 = sin($angle) * $factor / 3 + $gx;
$factor = $gy - imagesy($kaaba) / 2;
$kaabay = -cos($angle) * $factor + $factor;
$kaabay1 = $kaabay + imagesy($kaaba) / 2;
$kaabay2 = -cos($angle) * $factor / 3 + $gy;

$angle = isset($_GET['sun']) ? deg2rad($_GET['sun']) : 0;
$factor = $gx - imagesx($sun) / 2;
$sunx = sin($angle) * $factor + $factor;
$sunx1 = $sunx + imagesx($sun) / 2;
$sunx2 = sin($angle) * $factor / 3 + $gx;
$factor = $gy - imagesy($sun) / 2;
$suny = -cos($angle) * $factor + $factor;
$suny1 = $suny + imagesy($sun) / 2;
$suny2 = -cos($angle) * $factor / 3 + $gy;

$angle = isset($_GET['moon']) ? deg2rad($_GET['moon']) : 0;
$factor = $gx - imagesx($moon) / 2;
$moonx = sin($angle) * $factor + $factor;
$moonx1 = $moonx + imagesx($moon) / 2;
$moonx2 = sin($angle) * $factor / 3 + $gx;
$factor = $gy - imagesy($moon) / 2;
$moony = -cos($angle) * $factor + $factor;
$moony1 = $moony + imagesy($moon) / 2;
$moony2 = -cos($angle) * $factor / 3 + $gy;

$colorkaaba = imagecolorallocate($g, 0x00, 0x00, 0x00);
imageline($g, $kaabax1, $kaabay1, $kaabax2, $kaabay2, $colorkaaba);
$colorsun = imagecolorallocate($g, 0xBF, 0xBF, 0x00);
imageline($g, $sunx1, $suny1, $sunx2, $suny2, $colorsun);
$colormoon = imagecolorallocate($g, 0x7F, 0x7F, 0x7F);
imageline($g, $moonx1, $moony1, $moonx2, $moony2, $colormoon);

imagecopy($g, $kaaba, $kaabax, $kaabay, 0, 0, imagesx($kaaba), imagesy($kaaba));
imagecopy($g, $sun, $sunx, $suny, 0, 0, imagesx($sun), imagesy($sun));
imagecopy($g, $moon, $moonx, $moony, 0, 0, imagesx($moon), imagesy($moon));

// Output image
imagealphablending($g, FALSE);
imagesavealpha($g, TRUE);

if ($gif) {
  header('Content-type: image/gif');
  imagegif($g);
}
else {
  header('Content-type: image/png');
  imagepng($g);
}

// Free resources
imagedestroy($g);
imagedestroy($base);
imagedestroy($kaaba);
imagedestroy($sun);
imagedestroy($moon);
