<?php
/**
 * @file
 * Theming functions for the Photowall module.
 */

/**
 * Template preprocess handler for 'photowall' theme.
 */
function template_preprocess_photowall(&$variables) {

  //Add CSS.
  drupal_add_css(drupal_get_path('module', 'photowall') . '/css/photowall.css');

  //Add JQuery plugin and JS.
  $path = libraries_get_path('jquery-photowall');
  drupal_add_js($path . '/jquery-photowall.js');
  drupal_add_js(drupal_get_path('module', 'photowall') . '/js/photowall.js');

  $field_type = $variables['field_type'];

  $photowall = array();
  $variables['items'] = array_reverse($variables['items']);
  foreach ($variables['items'] as $num => $item) {

    //Generate ids
    $id = 'photowall-' . ($num + 1);

    if ($field_type == 'image') {
      $image['path'] = file_create_url($item['uri']);
    }
    if ($field_type == 'media') {
      $image['path'] = file_create_url($item['file']->uri);
    }

    $variables['items'][$num]['image_src'] = $image['path'];
    $image['alt'] = isset($item['alt']) ? $item['alt'] : '';

    if (isset($item['width']) && isset($item['height'])) {
      $image['width'] = $item['width'];
      $image['height'] = $item['height'];
    }
    else {
      $image_dims = getimagesize($image['path']);
      $image['width'] = $image_dims[0];
      $image['height'] = $image_dims[1];
    }
    // The height and width will be adjusted by photowall plugin itself.
    $photowall[$id] = array(
      'id' => $id,
      'img' => $variables['items'][$num]['image_src'],  //Source image for Showbox.
      'width' => $image['width'],
      'height' => $image['height'],
      'th' => array(
        'src' => $variables['items'][$num]['image_src'],  //Source image for Photowall thumbnails.
        'width' => $image['width'],
        'height' => $image['height'],
        'zoom_src' => '',
        'zoom_factor' => $variables['zoom_factor'],
      ),
    );
  }
  drupal_add_js(array('photowall' => $photowall), 'setting');
}

/**
 * Theme 'views_view_photowall'.
 */
function theme_views_view_photowall($vars) {
  $items = array();

  $view = $vars['view'];
  $img_field_name = $vars['img_field_name'];

  // Prepare items for our theme.
  foreach ($vars['rows'] as $row) {
    $lang = $row->_field_data[$view->base_field]['entity']->language;
    // omit rows without image field.
    if (!isset($row->_field_data[$view->base_field]['entity']->{$img_field_name})) {
      continue;
    }
    $item = $row->_field_data[$view->base_field]['entity']->{$img_field_name}[$lang][0];
    $items[] = $item;
  }

  // Get field type.
  $field_info = field_info_field($img_field_name);
  $field_type = $field_info['type'];

  return theme('photowall', array(
    'items' => $items,
    'zoom_factor' => $vars['options']['photowall_zoom_factor'],
    'field_type' => $field_type,
  ));
}
