<?php

/**
 * @file
 * Contains the theme function overrides for forms.
 */

/**
 * Implements hook_form_alter().
 */
function circle_form_alter(&$form, &$form_state, $form_id) {
  // Duplicate the form ID as a class so we can reduce specificity in our CSS.
  $form['#attributes']['class'][] = drupal_clean_css_identifier($form['#id']);
}

/**
 * Rewrite form element markup.
 */
function circle_form_element($variables) {

  $element = &$variables['element'];

  $attributes = isset($element['#attributes']) ? $element['#attributes'] : array();

  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'],
      array(' ' => '-', '_' => '-', '[' => '-', ']' => '')
    );
  }

  // Add disabled class.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }

  // Add required class.
  if (isset($element['#title']) && $element['#title'] != 'Language' && !empty($element['#required'])) {
    $attributes['class'][] = 'form-required';
  }

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }

  if (isset($attributes['class'])) {
    $output = '<div' . drupal_attributes($attributes) . '>';
  }
  else {
    $output = '<div>';
  }

  // Prefix and suffix markup improvements.
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  if (isset($element['#title_display']) && !empty($element['#title_display'])) {
    switch ($element['#title_display']) {
      case 'before':
      case 'invisible':
        $output .= ' ' . theme('form_element_label', $variables);
        $output .= ' ' . $prefix . $element['#children'] . $suffix;
        break;

      case 'after':
        $output .= ' ' . $prefix . $element['#children'] . $suffix;
        $output .= ' ' . theme('form_element_label', $variables);
        break;

      case 'none':
      case 'attribute':
        // Output no label and no required marker, only the children.
        $output .= ' ' . $prefix . $element['#children'] . $suffix;
        break;

    }
  }

  if (!empty($element['#description'])) {
    // Changes the description <div class="description"> to <small>.
    $output .= '<small class="description">' . $element['#description'] . '</small>';
  }

  $output .= '</div>';

  return $output;
}

/**
 * Rewrite the from required label symbol.
 */
function circle_form_required_marker($variables) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();
  $attributes = array(
    'class' => 'form-required',
    'title' => $t('This field is required.'),
  );
  return '<span' . drupal_attributes($attributes) . '></span>';
}

/**
 * Override form item label to include parent name as class.
 */
function circle_form_element_label($variables) {
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
  $attributes['class'] = '';
  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after') {
    $attributes['class'] .= 'option';
  }
  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $attributes['class'] .= 'element-invisible ';
  }
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }
  if (!empty($element['#parents'])) {
    $attributes['class'] .= 'field-' . $element['#parents'][0] . '-label';
  }
  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
}

/**
 * Implements theme_textfield().
 */
function circle_preprocess_textfield(&$variables) {
  // Check if textfield has parents.
  if (!empty($variables['element']['#parents'])) {
    // Add class attribute if it does not exist.
    if (!isset($variables['element']['#attributes']['class'])) {
      $variables['element']['#attributes']['class'] = array();
    }
    // Add parent name as textfield class.
    $variables['element']['#attributes']['class'][] = 'field-' . $variables['element']['#parents'][0] . '-textfield';
  }
  // Add label to data-label attribute for input.
  if (isset($variables['element']['#title'])) {
    $variables['element']['#attributes']['data-label'] = $variables['element']['#title'];
  }
}
