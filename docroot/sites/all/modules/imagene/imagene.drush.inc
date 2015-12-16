<?php


/**
 * Implements hook_drush_help().
 */
function imagene_drush_help($section) {
  switch ($section) {
    case 'drush:imagene':
      return dt('Create a gradient image with imagene.');
  }
}


/**
 * Implements hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function imagene_drush_command() {
  $items = array();

  $items['imagene'] = array(
    'description' => "Create an xy gradient image with imagene.",
    'drupal dependencies' => array('imagene', 'xautoload'),
    'arguments' => array(
      'code' => 'Code specifying the bezier controls colors.',
    ),
  );

  return $items;
}


function drush_imagene($slug) {
  $im = imagene_image_load("$slug.png", 'gradient-xy');

  _imagene_include();

  if (file_exists($im->file)) {
    // Nothing to do.
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

    // Save the image
    $dir = dirname($im->file);
    $success = file_prepare_directory($dir, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
    imagepng($image, drupal_realpath($im->file));
  }

  print "\n  Gradient image created in\n";
  print "  " . drupal_realpath($im->file) . "\n\n";
}
