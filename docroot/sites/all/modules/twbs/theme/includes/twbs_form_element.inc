<?php

/**
 * Returns HTML for a form element.
 *
 * Each form element is wrapped in a DIV container having the following CSS
 * classes:
 * - form-item: Generic for all form elements.
 * - form-type-#type: The internal element #type.
 * - form-item-#name: The internal form element #name (usually derived from the
 *   $form structure and set via form_builder()).
 * - form-disabled: Only set if the form element is #disabled.
 *
 * In addition to the element itself, the DIV contains a label for the element
 * based on the optional #title_display property, and an optional #description.
 *
 * The optional #title_display property can have these values:
 * - before: The label is output before the element. This is the default.
 *   The label includes the #title and the required marker, if #required.
 * - after: The label is output after the element. For example, this is used
 *   for radio and checkbox #type elements as set in system_element_info().
 *   If the #title is empty but the field is #required, the label will
 *   contain only the required marker.
 * - invisible: Labels are critical for screen readers to enable them to
 *   properly navigate through forms but can be visually distracting. This
 *   property hides the label for everyone except screen readers.
 * - attribute: Set the title attribute on the element to create a tooltip
 *   but output no label element. This is supported only for checkboxes
 *   and radios in form_pre_render_conditional_form_element(). It is used
 *   where a visual label is not needed, such as a table of checkboxes where
 *   the row and column provide the context. The tooltip will include the
 *   title and required marker.
 *
 * If the #title property is not set, then the label and any required marker
 * will not be output, regardless of the #title_display or #required values.
 * This can be useful in cases such as the password_confirm element, which
 * creates children elements that have their own labels and required markers,
 * but the parent element should have neither. Use this carefully because a
 * field without an associated label can cause accessibility challenges.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #title_display, #description, #id, #required,
 *     #children, #type, #name.
 *
 * @ingroup themeable
 */
function twbs_form_element($variables) {
  $element = &$variables['element'];

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item', 'form-group');
  if (!empty($element['#type'])) {
    $attributes['class'][] = strtr($element['#type'], '_', '-');
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(
      ' ' => '-',
      '_' => '-',
      '[' => '-',
      ']' => '',
    ));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }
  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? $element['#field_prefix'] : '';
  $prefix = $prefix ? '<span class="input-group-addon">' . $prefix . '</span>' : '';
  $suffix = isset($element['#field_suffix']) ? $element['#field_suffix'] : '';
  $suffix = $suffix ? '<span class="input-group-addon">' . $suffix . '</span>' : '';
  if ($prefix || $suffix) {
    $prefix = '<div class="input-group">' . $prefix;
    $suffix = $suffix . '</div>';
  }

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<p class="help-block">' . $element['#description'] . "</p>\n";
  }

  $output .= "</div>\n";

  return $output;
}
