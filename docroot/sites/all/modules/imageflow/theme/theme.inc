<?php
/**
 * @file
 * Theming functions for the imageflow module.
 */

/**
 * Template preprocess handler for 'imageflow_container' theme.
 */
function template_preprocess_imageflow_container(&$vars) {
  // Each imageflow instance gets a unique id.
  $imageflow_id = &drupal_static('imageflow_id', 0);
  $vars['id'] = ++$imageflow_id;

  // Load the used preset.
  if (!empty($vars['settings']['imageflow_preset'])) {
    $preset = imageflow_preset_load($vars['settings']['imageflow_preset']);
  }
  if (empty($preset)) {
    $preset = imageflow_preset_load('default');
  }

  // Attach imageflow JavaScript.
  imageflow_add_js($imageflow_id, $preset);

  // Prepare image elements.
  $items = $vars['items'];
  $vars['items'] = array();
  $image_style = empty($preset['imagestyle']) ? 'medium' : $preset['imagestyle'];
  foreach ($items as $delta => $item) {
    // Link to image file as default fallback.
    if (!isset($item['attributes']['longdesc'])) {
      $item['attributes']['longdesc'] = file_create_url($item['uri']);
    }
    $vars['imageflow'][$delta] = array(
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $image_style,
    );
  }
}

/**
 * Theme 'views_view_imageflow'.
 */
function theme_views_view_imageflow($vars) {
  $items = array();

  $view = $vars['view'];
  $img_field_name = $vars['img_field_name'];
  $link_field_name = $vars['link_field_name'];
  foreach ($vars['rows'] as $row) {
    if (!isset($row->_field_data[$view->base_field]['entity']->{$img_field_name}) ||
    !$row->_field_data[$view->base_field]['entity']->{$img_field_name}) {
      continue;
    }
    $field = current($row->_field_data[$view->base_field]['entity']->{$img_field_name});
    $item = $field[0];
    if (isset($row->_field_data[$view->base_field]['entity']->{$link_field_name})) {
      $field = reset($row->_field_data[$view->base_field]['entity']->{$link_field_name});
      $item['attributes']['longdesc'] = $field[0]['url'];
    }
    $items[] = $item;
  }

  return theme('imageflow_container', array(
      'items' => $items,
      'settings' => $vars['options'],
    ));
}
