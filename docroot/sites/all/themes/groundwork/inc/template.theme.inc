<?php

/**
 * @file
 * Implements hook_theme().
 *
 * Groundwork Framework for Drupal
 * Learn more ~ http://groundwork.noceda.me
 *
 * WARNING: DO NOT MODIFY THIS FILE. Create a subtheme for your customizations.
 *
 */

/**
 * Returns HTML for a fieldset form element and its children.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #attributes, #children, #collapsed, #collapsible,
 *     #description, #id, #title, #value.
 *
 * @ingroup themeable
 */
function groundwork_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  $fieldset_wrapper_class = '';

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend><span class="fieldset-legend">' . $element['#title'] . '</span></legend>';

    // add a class if a legend is being included
    $fieldset_wrapper_class = ' with-legend';
  }
  $output .= '<div class="fieldset-wrapper' . $fieldset_wrapper_class . '">';
  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</fieldset>\n";
  return $output;
}
