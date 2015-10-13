<?php
/**
 * @file
 * storiedmaps_icon.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function storiedmaps_icon_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "panels_mini" && $api == "panels_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function storiedmaps_icon_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function storiedmaps_icon_image_default_styles() {
  $styles = array();

  // Exported image style: icon-large.
  $styles['icon-large'] = array(
    'label' => 'icon-large',
    'effects' => array(
      9 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 100,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: icon_medium.
  $styles['icon_medium'] = array(
    'label' => 'icon_medium',
    'effects' => array(
      7 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 60,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: icon_medium_small.
  $styles['icon_medium_small'] = array(
    'label' => 'icon_medium_small',
    'effects' => array(
      10 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 40,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
