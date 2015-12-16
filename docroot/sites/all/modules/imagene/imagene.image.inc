<?php


/**
 * Menu callback for [files base]/imagene/[gradient type]/%imagene_image
 *
 * @param $im
 *   Image object, as returned by imagene_image_load()
 */
function imagene_image(stdClass $im) {

  _imagene_include();

  if (file_exists($im->file)) {
    $image = imagecreatefrompng($im->file);
  }
  else if (!user_access('see non-existing imagene images')) {
    // An admin should visit this page, before normal visitors do.
    $image = imagecreatefrompng(drupal_get_path('module', 'imagene') . '/empty.png');
  }
  else {
    // Create the image
    list($w, $h) = $im->image->getDimensions();
    $image = imagecreatetruecolor($w, $h);
    imagealphablending($image, FALSE);
    imagesavealpha($image, TRUE);
    for ($x = 0; $x < $w; ++$x) {
      for ($y = 0; $y < $h; ++$y) {
        @list($r,$g,$b,$a) = $im->image->getPixelRGBA($x, $y);
        if ($im->image->hasAlpha()) {
          // php does only count alpha up to 127, not 255.
          // Thus, we have to divide alpha by 2.
          $color = imagecolorallocatealpha($image, $r, $g, $b, $a/2);
        }
        else {
          $color = imagecolorallocate($image, $r, $g, $b);
        }
        imagesetpixel($image, $x, $y, $color);
      }
    }
    if (user_access('save imagene images')) {
      // Save the image
      $dir = dirname($im->file);
      $success = file_prepare_directory($dir, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
      imagepng($image, drupal_realpath($im->file));
    }
  }

  // Send the image
  // TODO:
  //   Do something similar to image_style_deliver() in image.module.
  drupal_add_http_header('Content-type', 'image/png');
  imagepng($image);
  imagedestroy($image);
  drupal_exit();
}




