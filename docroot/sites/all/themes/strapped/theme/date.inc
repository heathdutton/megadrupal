<?php
/**
 * @file
 * Date related theme functions.
 */



/**
 * Overrides theme_date().
 */
function strapped_date($variables) {

  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'form-inline';

  return '<div' . drupal_attributes($attributes) . '>' . drupal_render_children($element) . '</div>';
}

/**
 * Overrides theme_date_combo().
 */
function strapped_date_combo($variables) {
  $element = $variables['element'];



  $field = field_info_field($element['#field_name']);
  $instance = field_info_instance($element['#entity_type'], $element['#field_name'], $element['#bundle']);

  // Group start/end items together in fieldset.
  $fieldset = array(
    '#title' => t($element['#title']) . ' ' . ($element['#delta'] > 0 ? intval($element['#delta'] + 1) : ''),
    '#value' => '',
    '#description' => !empty($element['#fieldset_description']) ? $element['#fieldset_description'] : '',
    '#attributes' => array(),
    '#children' => $element['#children'],
  );

  // If there are any title dispaly settings then apply them to the fieldset.
  if (isset($element['#form_type'])) {
    $fieldset['#form_type'] = $element['#form_type'];
  }
  if (isset($element['#title_display'])) {
    $fieldset['#title_display'] = $element['#title_display'];
  }
  if (isset($element['#title_width'])) {
    $fieldset['#title_width'] = $element['#title_width'];
  }
  if (isset($element['#title_grid'])) {
    $fieldset['#title_grid'] = $element['#title_grid'];
  }
  if (isset($element['#element_grid'])) {
    $fieldset['#element_grid'] = $element['#element_grid'];
  }

  // Screw #date_combo_fieldset and just use a form element always.
  return theme('form_element', array('element' => $fieldset));

}


/**
 * Overides theme_date_select().
 */
function strapped_date_select($variables) {
  $element = $variables['element'];
  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : array('class' => array());

  $wrapper_attributes['class'][] = 'clearfix';
  // Add an wrapper to mimic the way a single value field works, for ease in
  // using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) . '>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) . '>' . theme('form_element', $element) . '</div>';
}



/**
 * Returns HTML for a date text element.
 */
function strapped_date_text($variables) {
  $element = $variables['element'];





  // Remove the 'date' subtitle @todo why isnt invisible working


  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : array('class' => array());

  // @todo Inline dates container is causing a pain so its gone for now.
  // $attributes['class'][] = 'container-inline-date';

  // If there is no description, the floating date elements need some extra
  // padding below them.
  $wrapper_attributes = array('class' => array('date-padding'));
  if (empty($element['date']['#description'])) {
    $wrapper_attributes['class'][] = 'clearfix';
  }
  // Add an wrapper to mimic the way a single value field works, for ease in
  // using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) . '>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) . '>' . theme('form_element', $element) . '</div>';
}



/**
 * Overides theme_date_select_element().
 */
function strapped_date_select_element($variables) {
  $element = $variables['element'];
  $parents = $element['#parents'];
  $part = array_pop($parents);

  return '<div class="date-' . $part . '">' . theme('select', $element) . '</div>';
}


/**
 * Implements hook_date_select_process_alter().
 */
function strapped_date_select_process_alter(&$element, &$form_state, $context) {
  // Date selects are horizontal forms
  $element['#form_type'] = 'inline';

  foreach (element_children($element) as $key => $value) {

    $element[$value]['#form_type'] = 'basic';
    $element[$value]['#form_type_wrapper'] = FALSE;
    $element[$value]['#form_field_wrapper'] = FALSE;
    // $element[$value]['#form_group_wrapper'] = FALSE;
  }

}


/**
 * Format a date popup element.
 *
 * Use a class that will float date and time next to each other.
 */
function strapped_date_popup($vars) {
  $element = $vars['element'];
  $attributes = !empty($element['#wrapper_attributes']) ? $element['#wrapper_attributes'] : array('class' => array());
  $attributes['class'][] = 'container-inline-date';
  // If there is no description, the floating date elements need some extra padding below them.
  $wrapper_attributes = array('class' => array('date-padding'));
  if (empty($element['date']['#description'])) {
    $wrapper_attributes['class'][] = 'clearfix';
  }
  // Add an wrapper to mimic the way a single value field works, for ease in using #states.
  if (isset($element['#children'])) {
    $element['#children'] = '<div id="' . $element['#id'] . '" ' . drupal_attributes($wrapper_attributes) .'>' . $element['#children'] . '</div>';
  }
  return '<div ' . drupal_attributes($attributes) .'>' . theme('form_element', $element) . '</div>';
}


/**
 * Returns HTML for a date_select 'date' label.
 */
function strapped_date_part_label_date($variables) {
  return t('Date', array(), array('context' => 'datetime'));
}


function strapped_process_date_popup($element) {
  // Hide the date label on popups
  $element['date']['#title_display'] = 'invisible';
  return $element;
}