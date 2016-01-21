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

  // Remove the form type wrappers from the child elements so form-inline works.
  $element['year']['#form_type_wrapper'] = FALSE;
  $element['month']['#form_type_wrapper'] = FALSE;
  $element['day']['#form_type_wrapper'] = FALSE;

  // Remove the form field wrappers from the child elements so form-inline works.
  $element['year']['#form_field_wrapper'] = FALSE;
  $element['month']['#form_field_wrapper'] = FALSE;
  $element['day']['#form_field_wrapper'] = FALSE;

  // Add a space between each child element so that they do not touch.
  $element['year']['#suffix'] = ' ';
  $element['month']['#suffix'] = ' ';
  $element['day']['#suffix'] = ' ';

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
    // Add a little space after each dropdown
    $element[$value]['#suffix'] = ' ';
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


/**
 * Theme a webform date element.
 */
function strapped_webform_date($variables) {
  $element = $variables['element'];

  // Remove the form type wrappers from the child elements so form-inline works.
  $element['year']['#form_type_wrapper'] = FALSE;
  $element['month']['#form_type_wrapper'] = FALSE;
  $element['day']['#form_type_wrapper'] = FALSE;

  // Remove the form field wrappers from the child elements so form-inline works.
  $element['year']['#form_field_wrapper'] = FALSE;
  $element['month']['#form_field_wrapper'] = FALSE;
  $element['day']['#form_field_wrapper'] = FALSE;

  // Add a space between each child element so that they do not touch.
  $element['year']['#suffix'] = ' ';
  $element['month']['#suffix'] = ' ';
  $element['day']['#suffix'] = ' ';

  $element['year']['#attributes']['class'][] = 'year';
  $element['month']['#attributes']['class'][] = 'month';
  $element['day']['#attributes']['class'][] = 'day';

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['year']['#attributes']['class'][] = 'error';
    $element['month']['#attributes']['class'][] = 'error';
    $element['day']['#attributes']['class'][] = 'error';
  }

  // Add HTML5 required attribute, if needed.
  if ($element['#required']) {
    $element['year']['#attributes']['required'] = 'required';
    $element['month']['#attributes']['required'] = 'required';
    $element['day']['#attributes']['required'] = 'required';
  }

  // Changed from 'webform-container-inline' to 'form-inline'
  $class = array('form-inline');

  // Add the JavaScript calendar if available (provided by Date module package).
  if (!empty($element['#datepicker'])) {
    $class[] = 'webform-datepicker';
    $calendar_class = array('webform-calendar');
    if ($element['#start_date']) {
      $calendar_class[] = 'webform-calendar-start-' . $element['#start_date'];
    }
    if ($element['#end_date']) {
      $calendar_class[] = 'webform-calendar-end-' . $element['#end_date'];
    }
    $calendar_class[] ='webform-calendar-day-' . variable_get('date_first_day', 0);

    $calendar =   theme('webform_calendar', array('component' => $element['#webform_component'], 'calendar_classes' => $calendar_class));
  }

  $output = '';
  $output .= '<div class="' . implode(' ', $class) . '">';
  $output .= drupal_render_children($element);
  $output .= isset($calendar) ? $calendar : '';
  $output .= '</div>';

  return $output;
}


/**
 * Theme a webform time element.
 */
function strapped_webform_time($variables) {
  $element = $variables['element'];

  // Remove the form type wrappers from the child elements so form-inline works.
  $element['hour']['#form_type_wrapper'] = FALSE;
  $element['minute']['#form_type_wrapper'] = FALSE;

  // Remove the form field wrappers from the child elements so form-inline works.
  $element['hour']['#form_field_wrapper'] = FALSE;
  $element['minute']['#form_field_wrapper'] = FALSE;


    // Add a space between each child element so that they do not touch.

    $element['minute']['#prefix'] = ' : ';
    $element['minute']['#suffix'] = ' ';
    $element['ampm']['am']['#suffix'] = ' ';
    $element['ampm']['pm']['#suffix'] = '  ';

    if (!empty($element['ampm'])) {
    //$element['ampm']['#element_inline'] = TRUE; @todo too late for the process funtiopn that cpies this to its child radio's
        $element['ampm']['am']['#element_inline'] = TRUE;
        $element['ampm']['pm']['#element_inline'] = TRUE;
    $element['ampm']['#form_field_wrapper'] = FALSE;
    $element['ampm']['#form_type_wrapper'] = FALSE;
    $element['ampm']['#form_group_wrapper'] = FALSE;
      // Sneaky hack to get the am/pm radios to line up with the time.
      $element['ampm']['#attributes']['class'][] = 'form-group';
  }

  $element['ampm']['am']['#form_group_wrapper'] = FALSE;
  $element['ampm']['am']['#form_type_wrapper'] = FALSE;
  $element['ampm']['am']['#form_field_wrapper'] = FALSE;

  $element['ampm']['pm']['#form_group_wrapper'] = FALSE;
  $element['ampm']['pm']['#form_type_wrapper'] = FALSE;
  $element['ampm']['pm']['#form_field_wrapper'] = FALSE;


  $element['hour']['#attributes']['class'][] = 'hour';
  $element['minute']['#attributes']['class'][] = 'minute';

  // Add error classes to all items within the element.
  if (form_get_error($element)) {
    $element['hour']['#attributes']['class'][] = 'error';
    $element['minute']['#attributes']['class'][] = 'error';
  }

  // Add HTML5 required attribute, if needed.
  if ($element['#required']) {
    $element['hour']['#attributes']['required'] = 'required';
    $element['minute']['#attributes']['required'] = 'required';
    if (!empty($element['ampm'])) {
      $element['ampm']['am']['#attributes']['required'] = 'required';
      $element['ampm']['pm']['#attributes']['required'] = 'required';
    }
  }

  $output = '<div class="form-inline">' . drupal_render($element['hour']) . drupal_render($element['minute']) . drupal_render($element['ampm']) . '</div>';

  return $output;
}


