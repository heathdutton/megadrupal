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


    // Columns
    if (isset($variables['element']['#grid']['columns'])) {
      foreach ($variables['element']['#grid']['columns'] as $size => $width) {
        if (!empty($width)) {
          $attributes['class'][] = 'col-' . $size . '-' . $width;
        }
      }
    }

    // Offset
    if (isset($variables['element']['#grid']['offset'])) {
      foreach ($variables['element']['#grid']['offset'] as $size => $width) {
        if (!empty($width)) {
          $attributes['class'][] = 'col-' . $size . '-offset-' . $width;
        }
      }
    }

    // Push
    if (isset($variables['element']['#grid']['#push'])) {
      foreach ($variables['element']['#grid']['#push'] as $size => $width) {
        if (!empty($width)) {
          $attributes['class'][] = 'col-' . $size . '-push-' . $width;
        }
      }
    }

    // Pull
    if (isset($variables['element']['#grid']['pull'])) {
      foreach ($variables['element']['#grid']['pull'] as $size => $width) {
        if (!empty($width)) {
          $attributes['class'][] = 'col-' . $size . '-pull-' . $width;
        }

      }
    }

  }

  return '<' . $tag . ' ' . drupal_attributes($attributes) . '>' . $variables['element']['#children'] . '</' . $tag . '>';
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
