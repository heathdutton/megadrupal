<?php

function hangouts_get_icon_images() {
  $images = array(
    '15x79',
    '20x86',
    '24x100',
    '60x230',
  );
  $images = array_fill_keys($images, 'key');
  foreach ($images as $key => &$val) {
    $val = hangouts_get_image($key);
  }
  return $images;
}

function hangouts_get_image($size) {
  $variables = array(
    'path' => 'https://ssl.gstatic.com/s2/oz/images/stars/hangout/1/gplus-hangout-' . $size . '-normal.png',
  );
  return theme('image', $variables);
}
