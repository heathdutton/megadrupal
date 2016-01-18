<?php
/**
 * @file
 * Contains all wrapper theme functions.
 */


/**
 * @param $variables
 * @return string
 */
function strapped_column_wrapper($variables) {

  // Start off with a blank attributes array.
  $attributes = isset($variables['element']['#attributes']) ? $variables['element']['#attributes'] : array();

  // If a #tag was passed in then override the default of div, used by form_element_label
  $tag = isset($variables['element']['#tag']) ? $variables['element']['#tag'] : 'div';

  if (isset($variables['element']['#grid'])) {

    $classes = _strapped_grid_classes($variables['element']['#grid']);
    if (isset($attributes['class'])) {
      $attributes['class'] = array_merge($classes, $attributes['class'] );
    } else {
      $attributes['class'] = $classes;
    }


//    $attributes['class'] = array_merge(  $attributes['class'], _strapped_grid_classes($variables['element']['#grid']));

  }

  return '<' . $tag . ' ' . drupal_attributes($attributes) . '>' . $variables['element']['#children'] . '</' . $tag . '>';
}

function _strapped_grid_classes($grid) {
  // Columns
  if (isset($grid['columns'])) {
    foreach ($grid['columns'] as $size => $width) {
      if (!empty($width)) {
        $classes[] = 'col-' . $size . '-' . $width;
      }
    }
  }

  // Offset
  if (isset($grid['offset'])) {
    foreach ($grid['offset'] as $size => $width) {
      if (!empty($width)) {
        $classes[] = 'col-' . $size . '-offset-' . $width;
      }
    }
  }

  // Push
  if (isset($grid['#push'])) {
    foreach ($grid['#push'] as $size => $width) {
      if (!empty($width)) {
        $classes[] = 'col-' . $size . '-push-' . $width;
      }
    }
  }

  // Pull
  if (isset($grid['pull'])) {
    foreach ($grid['pull'] as $size => $width) {
      if (!empty($width)) {
        $classes[] = 'col-' . $size . '-pull-' . $width;
      }

    }
  }

  return $classes;
}




/**
 * Row Wrappers
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing
 */
function strapped_row_wrapper($variables) {
  return '<div class="row">' . $variables['element']['#children'] . '</div>';
}


/**
 * Checkbox Wrappers
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing
 */
function strapped_checkbox_wrapper($variables) {
  return '<div class="checkbox">' . $variables['element']['#children'] . '</div>';
}

/**
 * Radio Wrappers
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing
 */
function strapped_radio_wrapper($variables) {
  return '<div class="radio">' . $variables['element']['#children'] . '</div>';
}

/**
 * Radio Wrappers
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing
 */
function strapped_form_type_wrapper($variables) {
  return '<div class="form-' . $variables['element']['#form_type'] . '">' . $variables['element']['#children'] . '</div>';
}


/**
 * Radio Wrappers
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing
 */
function strapped_form_group_wrapper($variables) {

  $attributes = array();

  return '<div class="form-group">' . $variables['element']['#children'] . '</div>';
}


/**
 * Radio Wrappers
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render containing
 */
function strapped_form_field_wrapper($variables) {

  $attributes = $variables['element']['#form_field_wrapper_attributes'];

  return '<div ' . drupal_attributes($attributes) . '>' . $variables['element']['#children'] . '</div>';
}


function strapped_form_control_static_wrapper($variables) {

  return '<div class="form-control-static">' . $variables['element']['#children'] . '</div>';
}