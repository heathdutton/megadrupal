<?php
/**
 * @file
 * sprout_media.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sprout_media_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function sprout_media_image_default_styles() {
  $styles = array();

  // Exported image style: imce_preview.
  $styles['imce_preview'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => '',
          'height' => 300,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'imce_preview',
  );

  return $styles;
}
