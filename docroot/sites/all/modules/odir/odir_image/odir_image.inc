<?php
/**
 * @file
 * Page callbacks for odir_image module.
 */

/**
 * Returns a jpeg image to the browser.
 */
function odir_image_jpeg($path) {
  $path = odir_current($path);
  drupal_add_http_header('Content-Type:', 'image/jpeg');
  $im = imagecreatefromjpeg(odir_system_path($path));
  imagejpeg($im);
  imagedestroy($im);
}

/**
 * Return rendered html of supplid image path
 *
 * @param string $htmllink_image_cache_preset
 *   An  image_cache preset.
 * @param string $path
 *   Path to file.
 */
function odir_image_html($htmllink_image_cache_preset, $path) {
  $image_cache_preset  = $htmllink_image_cache_preset;
  $htmllink_image_cache_preset = "";
  return _odir_image_show_image($image_cache_preset, $htmllink_image_cache_preset, $path);
}

/**
* Transforms image with $presetname and delivers it to the browser.
*/
function odir_image_deliver_style($presetname, $path) {
  $path = odir_decode($path);
  $scheme = "odir";
  image_style_deliver(image_style_load($presetname), $scheme, '/' . $path);
}
