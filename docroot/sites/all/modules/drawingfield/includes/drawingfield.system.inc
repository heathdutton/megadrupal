<?php
/**
 * @file
 * System module integration.
 */

/**
 * Implements hook_theme().
 */
function drawingfield_theme() {
  $items = array();
  $items['drawingfield'] = array(
    'template' => 'templates/drawingfield',
    'render element' => 'element',
  );
  $items['drawingfield_display'] = array(
    'variables' => array(
      'data' => NULL,
      'settings' => array(),
    ),
  );
  drupal_alter('drawingfield_theme', $items);
  return $items;
}

/**
 * Implements hook_element_info().
 */
function drawingfield_element_info() {
  $type = array();
  $type['drawingfield'] = array(
    '#input' => TRUE,
    '#after_build' => array('drawingfield_after_build'),
    '#height' => DEFAULT_DRAWING_HEIGHT,
    '#width' => DEFAULT_DRAWING_WIDTH,
    '#background_color' => 'transparent',
    '#theme' => 'drawingfield',
  );
  return $type;
}

/**
 * Determines the value for drawing field form element.
 */
function form_type_drawingfield_value($element, $edit, $form_state) {
  if (!empty($edit)) {
    // If previous value exists delete original image.
    if (!empty($element['#default_value'])) {
      $values = unserialize($element['#default_value']);
      drupal_unlink($values['image_path']);
    }
    return drawingfield_base64_to_image($edit);
  }
  return $edit;
}

/**
 * This function includes required javascript files once field is created.
 */
function drawingfield_after_build($element, $form_state) {
  global $base_url;
  $drawing_edit_path = '';
  $libpath_literallycanvas = libraries_get_path('literallycanvas');
  $libpath_reactjs = libraries_get_path('reactjs');
  $modpath = drupal_get_path('module', 'drawingfield');
  if (!empty($element['#default_value'])) {
    $values = unserialize($element['#default_value']);
    $drawing_edit_path = $values['json'];
  }
  $width = round($element['#width'] * 8.5/100);
  $height = round($element['#height'] * 8.5/100);
  $element['#attached']['js'] = array(
    "{$libpath_reactjs}/react.js",
    "{$libpath_literallycanvas}/js/literallycanvas.min.js",
    "{$modpath}/drawingfield.js",
  );
  $element['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array(
      'drawingfield' => array(
        'backgroundColor' => check_plain($element['#background_color']),
        'validateFields' => FALSE,
        'imageUrlPrefix' => $base_url . '/' . $libpath_literallycanvas . '/img',
        'drawingEditPath' => $drawing_edit_path,
        'width' => $element['#width'] - $width,
		    'height' => $element['#height'] - $height,
      ),
    ),
  );
  $element['#attached']['css'] = array(
    "{$libpath_literallycanvas}/css/literallycanvas.css",
  );
  return $element;
}

/**
 * Implements hook_preprocess_HOOK().
 */
function drawingfield_preprocess_drawingfield(&$vars) {
  $values = unserialize($vars['element']['#default_value']);
  $vars['element']['#default_value'] = $values['json_base64'];
}

/**
 * Theme function to render the drawing field.
 */
function theme_drawingfield_display($element) {
  $values = unserialize(is_array($element) && !empty($element['data']) ? $element['data'] : $element);
  return theme('image', array('path' => $values['image_path']));
}
