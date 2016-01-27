<?php
/**
 * @file
 * nm_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_events_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function nm_events_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function nm_events_image_default_styles() {
  $styles = array();

  // Exported image style: nodemaker_event_page.
  $styles['nodemaker_event_page'] = array(
    'name' => 'nodemaker_event_page',
    'effects' => array(
      8 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '900',
          'height' => '380',
        ),
        'weight' => '1',
      ),
    ),
  );

  // Exported image style: nodemaker_event_teaser.
  $styles['nodemaker_event_teaser'] = array(
    'name' => 'nodemaker_event_teaser',
    'effects' => array(
      6 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '900',
          'height' => '380',
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
function nm_events_node_info() {
  $items = array(
    'nm_event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('Use <em>Event</em> for content with a date, time and location.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Use <em>Event</em> for content with a date, time and location.'),
    ),
  );
  return $items;
}
