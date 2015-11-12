<?php
/**
 * @file
 * drupalgive.features.inc
 */

/**
 * Implements hook_views_api().
 */
function drupalgive_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function drupalgive_image_default_styles() {
  $styles = array();

  // Exported image style: drupal_give_35-50
  $styles['drupal_give_35-50'] = array(
    'name' => 'drupal_give_35-50',
    'effects' => array(
      10 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '35',
          'height' => '50',
        ),
        'weight' => '1',
      ),
      11 => array(
        'label' => 'Desaturate',
        'help' => 'Desaturate converts an image to grayscale.',
        'effect callback' => 'image_desaturate_effect',
        'dimensions passthrough' => TRUE,
        'module' => 'image',
        'name' => 'image_desaturate',
        'data' => array(),
        'weight' => '2',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function drupalgive_node_info() {
  $items = array(
    'drupalgive_contribution' => array(
      'name' => t('Drupalgive-Contribution'),
      'base' => 'node_content',
      'description' => t('A contribution made by a team member to the Drupal or open source community'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
