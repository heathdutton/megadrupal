<?php

/**
 * @file
 * Provides form-related theme overrides and implementations.
 */


/**
 * Implements theme_button().
 */
function bh_bootstrap_button($variables) {
  $variables['element']['#attributes']['class'][] = 'btn';
  return theme_button($variables);
} // bh_bootstrap_button()


/**
 * Implements theme_checkbox().
 */
function bh_bootstrap_checkbox($variables) {
  $element = $variables['element'];
  $t = get_t();
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox'));

  switch ($element['#title_display']) {
    case 'attribute':
      $element['#attributes']['#title'] = $element['#description'];
      $output = '<input' . drupal_attributes($element['#attributes']) . ' />';
      break;
    // Bootstrap's default styles cause 'before' to render exactly like 'after':
    case 'before':
      $output = '<label class="checkbox">' . $element['#description'] . '<input' . drupal_attributes($element['#attributes']) . ' /></label>';
      break;
    // The 'invisible' option really makes no sense in the context of checkboxes
    // or radios, so just treat it like 'after':
    case 'invisible':
    case 'after':
    default:
      // There are some odd cases, such as the module list page, where checkboxes
      // apparently themed with a #title_display value of 'before' when it should
      // really be 'attribute'. The two possible outcomes handle this problem:
      $checkbox = '<input' . drupal_attributes($element['#attributes']) . ' />';
      if (isset($element['#description'])) {
        $output = '<label class="checkbox">' . $checkbox . $element['#description'] . '</label>';
      }
      else {
        $output = $checkbox;
      }
      break;
  }

  return $output;
} // bh_bootstrap_checkbox()


/**
 * Implements theme_checkboxes().
 */
function bh_bootstrap_checkboxes($variables) {
  // Redefine #children:
  $option_children = '';
  foreach ($variables['element']['#options'] as $key => $description) {
    $option_variables = array(
      'element' => $variables['element'][$key],
    );
    $option_variables['element']['#description'] = $option_variables['element']['#title'];
    $option_children .= theme('checkbox', $option_variables) . "\n";
  }
  $variables['element']['#children'] = $option_children;
  // Proceed normally:
  $element = $variables['element'];
  // Attributes usually figured out here, but serve no purpose here (?)
  return !empty($element['#children']) ? $element['#children'] : '';
} // bh_bootstrap_checkboxes()


/**
 * Implements theme_fieldset().
 *
 * Rather unfortunately, the default theme implementation includes some markup
 * (the 'fieldset-wrapper' div) that's (a) indispensible, and (b) untouchable
 * without copying in the entire function. In this case, were we only want to
 * add a single class to the div in question, this seems kind of wasteful.
 */
function bh_bootstrap_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend><span class="fieldset-legend">' . $element['#title'] . '</span></legend>';
  }
  // Add 'row-fluid' class to fieldset wrapper div so that, by adding Bootstrap
  // span classes to the sub-elements, we can theme forms in a very flexible way:
  $output .= '<div class="fieldset-wrapper row-fluid">';
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
} // bh_bootstrap_fieldset()


/**
 * Implements theme_file().
 */
function bh_bootstrap_file($variables) {
  // Assign the Bootstrap class:
  _form_set_class($variables['element'], array('input-file'));
  // Set a reasonably low default size:
  $variables['element']['#attributes']['size'] = '22';
  return theme_file($variables);
} // bh_bootstrap_file()


/**
 * Implements theme_form().
 */
function bh_bootstrap_form($variables) {
  $form_classes = implode(' ', array_map('_bh_bootstrap_css_safe', theme_get_setting('bh_bootstrap_form__classes')));
  $variables['element']['#attributes']['class'] = $form_classes;
  return theme_form($variables);
} // bh_bootstrap_form()


/**
 * Implements theme_form_element().
 *
 * Original function's notes retained here for reference below. Note that, in
 * the current implementation:
 *
 * - the strategy has been to alter the core function reproduced here as little
 *   as possible;
 * - everything works for all stock Drupal form elements (if it doesn't, it's a
 *   bug!);
 * - no work has yet been done to make sure the Webform module's forms will
 *   render nicely;
 * - we have opted to retain most or all native Drupal classes in addition to
 *   adding Bootstrap classes;
 * - #title_display settings should work but may be fragile;
 *
 *
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
function bh_bootstrap_form_element($variables) {
  $element = &$variables['element'];
  // Make sure no PHP notices are triggered when type is not provided:
  if (!isset($element['#type'])) {
    $element['#type'] = '';
  }
  // This is also used in the installer, pre-database setup.
  $t = get_t();

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
  $attributes['class'] = array(
    'form-item',
    'control-group',
  );
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
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
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';

  // Get the description, if any, before we assemble the rest of the pieces:
  $description = !empty($element['#description']) ? '<p class="description help-block">' . $element['#description'] . "</p>\n" : '';

  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      switch ($element['#type']) {
        case 'password':
        case 'textfield':
        default:
          $output .= theme('form_element_label', $variables);
          $output .= '<div class="controls">' . "\n";
          $output .= $prefix . $element['#children'] . $suffix . "\n";
          $output .= $description . "\n";
          $output .= '</div>' . "\n";
          break;
      }
      break;

    case 'after':
      // Single checkboxes and radios are handled here by default, and need special
      // treatment:
      switch ($element['#type']) {
        case 'checkbox':
        case 'radio':
          $output .= theme('form_element_label', $variables) . "\n";
          $output .= '<div class="controls">' . "\n";
          $output .= $element['#children'];
          $output .= '</div>';
          break;
        default:
          $output .= ' ' . $prefix . $element['#children'] . $suffix;
          $output .= ' ' . theme('form_element_label', $variables) . "\n";
      }
      break;

    case 'none':
    case 'attribute':
      switch ($element['#type']) {
        case 'checkbox':
        case 'radio':
          $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
          break;
        default:
          // Output no label and no required marker, only the children.
          $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
          $output .= $description;
      }
      break;
  }

  $output .= "</div>\n";

  return $output;
} // bh_bootstrap_form_element()


/**
 * Implements theme_form_element_label().
 *
 * Specifically, it entirely duplicates the core function but adds a single
 * class to the attributes array. A little more consistency in how form elements
 * are themed in core would not be unwelcome...
 */
function bh_bootstrap_form_element_label($variables) {
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

  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Add Bootstrap-related class to label:
  if (isset($attributes['class'])) {
    $attributes['class'] .= ' control-label';
  }
  else {
    $attributes['class'] = 'control-label';
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
} // bh_bootstrap_form_element_label()


/**
 * Implements theme_radio():
 */
function bh_bootstrap_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));

  switch ($element['#title_display']) {
    // This *could* make sense e.g. in a table of mutually exclusive options...
    case 'attribute':
      $element['#attributes']['#title'] = $element['#description'];
      $output = '<input' . drupal_attributes($element['#attributes']) . ' />';
      break;
    // Bootstrap's default styles cause 'before' to render exactly like 'after':
    case 'before':
      $output = '<label class="radio">' . $element['#description'] . '<input' . drupal_attributes($element['#attributes']) . ' /></label>';
      break;
    // The 'invisible' option really makes no sense in the context of checkboxes
    // or radios, so just treat it like 'after':
    case 'invisible':
    case 'after':
    default:
      $radio = '<input' . drupal_attributes($element['#attributes']) . ' />';
      if (isset($element['#description'])) {
        $output = '<label class="radio">' . $radio . $element['#description'] . '</label>';
      }
      else {
        $output = $radio;
      }
      break;
  }

  return $output;
} // bh_bootstrap_radio()


/**
 * Implements theme_radios().
 */
function bh_bootstrap_radios($variables) {
  // Redefine #children:
  $option_children = '';
  foreach ($variables['element']['#options'] as $key => $description) {
    $option_variables = array(
      'element' => $variables['element'][$key],
    );
    $option_variables['element']['#description'] = $option_variables['element']['#title'];
    $option_children .= theme('radio', $option_variables) . "\n";
  }
  $variables['element']['#children'] = $option_children;
  $element = $variables['element'];
  // Attributes usually figured out here, but serve no purpose here (?)
  return !empty($element['#children']) ? $element['#children'] : '';
} // bh_bootstrap_radios()


/**
 * Implements theme_textarea().
 */
function bh_bootstrap_textarea($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  // Add non-Drupal class here:
  _form_set_class($element, array('input-xlarge'));

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    drupal_add_library('system', 'drupal.textarea');
    // Don't add usual Drupal wrapper class here:
    $wrapper_attributes['class'][] = 'resizable';
  }
  else {
    $wrapper_attributes = array();
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
} // bh_bootstrap_textarea()

