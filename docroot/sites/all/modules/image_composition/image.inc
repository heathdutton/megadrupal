<?php
/**
 * @file
 *   A simple image resize script that resizes images given either a maxsize, maxheight or maxwidth
 *
 * MODIFIED by dman 2012-06 to be aware of D7 file wrappers
 *
 * Usage
 * =====
 * -to resize an image to a max of 400px along the longest side
 * <img src="image.php?size=400&file=filename.jpg" />
 *
 * -to resize an image to a height of 400px (width will be kept to the right aspect ratio)
 * <img src="image.php?size=h400&file=filename.jpg" />
 *
 * -to resize an image to a width of 400px (height will be kept to the right aspect ratio)
 * <img src="image.php?size=w400&file=filename.jpg" />
 *
 * This script is very simple and should not be considered for production use. There are many image
 * resizing scripts available that have better error checking, support for other formats (this only
 * supports jpg) and have image caching. Cachine makes a HUGE difference to overall speed.
 *
 * @author Harvey Kane harvey@harveykane.com 2006
 * @author semi-drupalized by dman 2012-02
 *
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function image_composition_image() {

  /* Get information from Query String */
  if (!isset($_GET['file']) || !isset($_GET['size'])) {
    echo "Image variables not specified correctly";
    exit();
  }

  $file_uri = $_GET['file'];
  $size = $_GET['size'];

  $file = drupal_realpath($file_uri);

  // Drupal security layer.
  // Check file is in the public files dir.
#  if (!file_check_location($file, file_directory_path())) {
#    echo t("Image path '%file' not in an allowed Drupal public files directory.", array('%file' => $file));
#    exit();
#  }

  /* Get image dimensions and ratio */
  list($width, $height) = getimagesize($file);
  $ratio = $width / $height;

  /* Decide how we should resize image - fixed width or fixed height */
  if (substr($size, 0, 1) == 'h') {
    $type = 'fixedheight';
  }
  elseif (substr($size, 0, 1) == 'w') {
    $type = 'fixedwidth';
  }
  elseif ($height > $width) {
    $type = 'fixedheight';
  }
  else {
    $type = 'fixedwidth';
  }

  /* Calculate new dimensions */
  if ($type == 'fixedheight') {
    $new_width = floor(str_replace('h', '', $size) * $ratio);
    $new_height = str_replace('h', '', $size);
  }
  else {
    $new_width = str_replace('w', '', $size);
    $new_height = floor(str_replace('w', '', $size) / $ratio);
  }

  /* Resample */
  $new_image = imagecreatetruecolor($new_width, $new_height);
  $old_image = imagecreatefromjpeg($file);
  imagecopyresampled($new_image, $old_image, 0, 0, 0, 0, $new_width, $new_height, $width, $height);

  /* Output */
  header('Content-type: image/jpeg');
  imagejpeg($new_image, null, 100);
  exit();

}
