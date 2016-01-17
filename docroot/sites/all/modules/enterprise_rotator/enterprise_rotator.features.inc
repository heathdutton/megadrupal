<?php
/**
 * @file
 * enterprise_rotator.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_rotator_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_rotator_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function enterprise_rotator_image_default_styles() {
  $styles = array();

  // Exported image style: oe_slide
  $styles['oe_slide'] = array(
    'name' => 'oe_slide',
    'effects' => array(
      1 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '940',
          'height' => '240',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function enterprise_rotator_node_info() {
  $items = array(
    'oe_slide' => array(
      'name' => t('Banner Slide'),
      'base' => 'node_content',
      'description' => t('A <em>Banner Slide</em> is a slide that will be placed on the rotating banner on the home page. It contains an image, text and a link for when a user clicks on the banner.'),
      'has_title' => '1',
      'title_label' => t('Banner Headline'),
      'help' => '',
    ),
  );
  return $items;
}
