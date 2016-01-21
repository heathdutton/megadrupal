<?php
/**
 * @file
 * form-element-label.theme.inc
 */


/**
 * Theme the 'form label'.
 *
 * @param array $variables
 *   The array used to modify the panels ipe placeholder pane.
 */
function wetkit_omega_form_element_label($variables) {
  $element = $variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If title and required marker are both empty, output no label.
  if ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required'])) {
    return '';
  }

  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);

  $attributes = array();
  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after') {
    $attributes['class'] = 'option';
  }
  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $attributes['class'] = 'element-invisible';
  }

  // Accessibility fix:
  // See: https://www.drupal.org/node/2279111
  // and https://www.drupal.org/node/504962
  // Remove 'for' attribute from known drupal-specific unlabelable elements.
  $unlabelable_elements = array('radios', 'checkboxes', 'date');
  if (!empty($element['#id']) && !in_array($variables['element']['#type'], $unlabelable_elements)) {
    // labelable element: add a 'for' attribute
    $attributes['for'] = $element['#id'];
  } else {
    // label this element as a composite form
    $variables['#composite'] = TRUE;
  }

  // The leading whitespace helps visually separate fields from inline labels.
  if (isset($variables['#composite']) && $variables['#composite'] == TRUE) {
    return ' <span class="composite-form-label">' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</span>\n";
  }
  else {
    return ' <label' . drupal_attributes($attributes) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
  }
}
