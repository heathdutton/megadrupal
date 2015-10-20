<?php
/**
 * @file
 * Hook definitions of odir image project
 */


/**
 * Define slideshow modes.
 */
function hook_odir_image_supported_slideshow_modes() {
  return array('colorbox' => 'ColorBox');
}

/**
 * Setting attributes for slideshow bases on the mode-name
 * from hook_odir_image_supported_slideshow_modes.
 */
function hook_odir_image_show_image($image_cache_preset, $htmllink_image_cache_preset, $path) {
  $p = array();
  if (odir_image_check_slideshow_mode('colorbox')) {
    $p['rel_attribute'] = "odir_colorbox_slideshow";
    $p['url_link'] = url("odir/image/jpeg/" . $htmllink_image_cache_preset . '/' . $path);
  }
  return $p;
}
