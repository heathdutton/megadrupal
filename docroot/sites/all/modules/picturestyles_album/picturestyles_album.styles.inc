<?php
/**
 * @file
 * Image styles for Album picture style.
 *
 * The default styles are the default image styles for the formatter.
 * These can be overriden either in the GUI, or a custom module
 * using hook_image_styles_alter.
 */

function _picturestyles_album_image_default_styles() {
  $styles = array();

  $styles['picturestyles_album_large'] = array();
  $styles['picturestyles_album_small_top'] = array();
  $styles['picturestyles_album_small_btm'] = array();

  $styles['picturestyles_album_large']['effects'] = array(
    array(
      // Name of the image effect. See image_image_effect_info() in
      'name' => 'image_scale_and_crop',
      'data' => array(
        'width' => 265,
        'height' => 185,
      ),
      // The order in which image effects should be applied when using this
      // style.
      'weight' => 0,
    ),
    // Add a second effect to this image style. Effects are executed in order
    // and are cummulative. When applying an image style to an image the result
    // will be the combination of all effects associated with that style.

  );

  $styles['picturestyles_album_small_top']['effects'] = array(
    array(
      // Name of the image effect. See image_image_effect_info() in
      'name' => 'image_scale_and_crop',
      'data' => array(
        'width' => 127,
        'height' => 88,
      ),
      // The order in which image effects should be applied when using this
      // style.
      'weight' => 0,
    ),
    // Add a second effect to this image style. Effects are executed in order
    // and are cummulative. When applying an image style to an image the result
    // will be the combination of all effects associated with that style.

  );

  $styles['picturestyles_album_small_btm']['effects'] = array(
    array(
      // Name of the image effect. See image_image_effect_info() in
      'name' => 'image_scale_and_crop',
      'data' => array(
        'width' => 127,
        'height' => 94,
      ),
      // The order in which image effects should be applied when using this
      // style.
      'weight' => 0,
    ),
    // Add a second effect to this image style. Effects are executed in order
    // and are cummulative. When applying an image style to an image the result
    // will be the combination of all effects associated with that style.

  );

  return $styles;

}

