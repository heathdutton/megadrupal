<?php

/**
 * @file
 *
 */

/**
 * Override of theme_breadcrumb().
 */
function stack_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];

  $i = 0;
  foreach ($breadcrumb as $k => $link) {
    $breadcrumb[$k] = "<span class='link link-{$i}'>{$link}</span>";
    $i++;
  }
  $breadcrumb = implode("<span class='divider'>&raquo;</span>", $breadcrumb);

  return "<div class='breadcrumb'>{$breadcrumb}</div>";
}

/**
 * Override of theme('filter_guidelines').
 */
function stack_filter_guidelines($vars) {
  return '';
}

/**
 * Override of theme('textarea').
 * Deprecate misc/textarea.js in favor of using the 'resize' CSS3 property.
 */
function stack_textarea($vars) {
  $element = $vars['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper'),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    $wrapper_attributes['class'][] = 'resizable';
  }

  if (!isset($element['#value'])) {
    $element['#value'] = '';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';

  return $output;
}
