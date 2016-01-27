<?php
/**
 * @file
 *
 * This php file returnes Captcha image in image/jpeg format
 */

/**
 * Generate captcha picture.
 */
function form_maker_captcha() {
  if (isset($_GET['digit'])) {
    $cap_width = $_GET['digit'] * 10 + 15;
  }
  else {
    $cap_width = 6 * 10 + 15;
  }
  $cap_height = 30;
  $cap_quality = 100;
  if (isset($_GET['digit'])) {
    $cap_length_min = $_GET['digit'];
  }
  else {
    $cap_length_min = 6;
  }

  if (isset($_GET['digit'])) {
    $cap_length_max = $_GET['digit'];
  }
  else {
    $cap_length_max = 6;
  }
  $cap_digital = 1;
  $cap_latin_char = 1;
  $l = rand($cap_length_min, $cap_length_max);
  $code = form_maker_code_generic($l, $cap_digital, $cap_latin_char);
  $_SESSION['form_maker_captcha'] = $code;
  $canvas = imagecreatetruecolor($cap_width, $cap_height);
  $c = imagecolorallocate($canvas, rand(150, 255), rand(150, 255), rand(150, 255));
  imagefilledrectangle($canvas, 0, 0, $cap_width, $cap_height, $c);
  $count = drupal_strlen($code);
  $color_text = imagecolorallocate($canvas, 0, 0, 0);
  for ($it = 0; $it < $count; $it++) {
    $letter = $code[$it];
    imagestring($canvas, 6, (10 * $it + 10), $cap_height / 4, $letter, $color_text);
  }

  for ($c = 0; $c < 150; $c++) {
    $x = rand(0, $cap_width - 1);
    $y = rand(0, 29);
    $col = '0x' . rand(0, 9) . '0' . rand(0, 9) . '0' . rand(0, 9) . '0';
    imagesetpixel($canvas, $x, $y, $col);
  }

  header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
  header('Cache-Control: no-store, no-cache, must-revalidate');
  header('Cache-Control: post-check=0, pre-check=0', FALSE);
  header('Pragma: no-cache');
  header('Content-Type: image/jpeg');
  imagejpeg($canvas, NULL, $cap_quality);
}

/**
 * Generate captcha code.
 */
function form_maker_code_generic($_length, $_digital = 1, $_latin_char = 1) {
  $dig = array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
  $lat = array(
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'x',
    'y',
    'z');
  $main = array();

  if ($_digital) {
    $main = array_merge($main, $dig);
  }
  if ($_latin_char) {
    $main = array_merge($main, $lat);
  }
  shuffle($main);
  $pass = drupal_substr(implode('', $main), 0, $_length);
  return $pass;
}
