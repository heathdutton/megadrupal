<?php
/**
 * @file
 * kcpd.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function kcpd_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "bean_admin_ui" && $api == "bean") {
    return array("version" => "5");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_image_default_styles().
 */
function kcpd_image_default_styles() {
  $styles = array();

  // Exported image style: bio_photo_full.
  $styles['bio_photo_full'] = array(
    'label' => 'Bio Photo Full',
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 400,
          'height' => 600,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: bio_photo_small.
  $styles['bio_photo_small'] = array(
    'label' => 'Bio Photo Small',
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 200,
          'height' => 300,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: move_cover_full.
  $styles['move_cover_full'] = array(
    'label' => 'Move Cover Full',
    'effects' => array(
      3 => array(
        'name' => 'image_scale',
        'data' => array(
          'width' => 400,
          'height' => '',
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function kcpd_node_info() {
  $items = array(
    'actor' => array(
      'name' => t('Actor'),
      'base' => 'node_content',
      'description' => t('Add a new actor or actress.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'movie' => array(
      'name' => t('Movie'),
      'base' => 'node_content',
      'description' => t('Add a new movie.'),
      'has_title' => '1',
      'title_label' => t('Movie Title'),
      'help' => '',
    ),
    'review' => array(
      'name' => t('Review'),
      'base' => 'node_content',
      'description' => t('Write a review about a movie.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
