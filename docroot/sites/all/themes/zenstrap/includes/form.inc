<?php

function zenstrap_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));
  $element['#attributes']['class'][] = 'btn';
  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

function zenstrap_form($variables) {
  $element = $variables['element'];
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  element_set_attributes($element, array('method', 'id'));
  if (empty($element['#attributes']['accept-charset'])) {
    $element['#attributes']['accept-charset'] = "UTF-8";
  }
  $horizontal_forms = theme_get_setting('zenstrap_horizontal_forms');
  if ($element['#id'] == 'user-login-form') {
    $element['#attributes']['class'][] = 'form-inline';
  }
  elseif ($horizontal_forms && ( !isset($element['#attributes']['class']) || 
    !in_array('form-inline', $element['#attributes']['class']))) {
    $element['#attributes']['class'][] = 'form-horizontal';
  }
  // Anonymous DIV to satisfy XHTML compliance.
  return '<form' . drupal_attributes($element['#attributes']) . '><div>' . $element['#children'] . '</div></form>';
}

function zenstrap_form_element_label($variables) {
  $horizontal_forms = theme_get_setting('zenstrap_horizontal_forms');
  $element = $variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If title and required marker are both empty, output no label.
  if ( (!$horizontal_forms && isset($element['#type']) && in_array($element['#type'], array('checkbox', 'radio')) ) ||
    ((!isset($element['#title']) || $element['#title'] === '') && empty($element['#required']))) {
    return '';
  }

  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  $title = filter_xss_admin($element['#title']);

  $attributes = array();
  // Style the label as class option to display inline with the element.
  if ($horizontal_forms && $element['#title_display'] == 'after') {
    $attributes['class'] = array('option');
    if ($horizontal_forms) {
      $attributes['class'] = array('control-label');
    }
  }
  // Show label only to screen readers to avoid disruption in visual flows.
  elseif ($element['#title_display'] == 'invisible') {
    $attributes['class'] = array('element-invisible');
  }
  elseif ($horizontal_forms) {
    $attributes['class'] = array('control-label');
  }
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // The leading whitespace helps visually separate fields from inline labels.
  return ' <label' . drupal_attributes($attributes) . '>' . $t('!title !required', array('!title' => $title, '!required' => $required)) . "</label>\n";
}

function zenstrap_form_element($variables) {
  $horizontal_forms = theme_get_setting('zenstrap_horizontal_forms');
  $element = &$variables['element'];
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
  $attributes['class'] = array('form-item');
  if ($horizontal_forms && $element['#title_display'] != 'invisible') {
    $attributes['class'][] = 'control-group';
  }
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
  $control_prefix = '';
  $control_suffix = '';
  $description = '';
  if ($horizontal_forms && !empty($element['#description']) && isset($element['#type']) &&
      !in_array($element['#type'], array('checkbox', 'radio', 'password'))) {
    $description = '<p class="help-block">' . $element['#description'] . "</p>\n";
  }
  switch ($element['#title_display']) {
    case 'before':
    case 'after':
      if ($horizontal_forms) {
        $control_prefix = '<div class="controls">';
        $control_suffix = '</div>';
      }
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $control_prefix . $prefix . $element['#children'] . $suffix;
      $output .= ' ' . $description . $control_suffix . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }
  if (!$horizontal_forms && !empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . "</div>\n";
  }


  $output .= "</div>\n";
  return $output;
}

function zenstrap_textfield($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'text';
  element_set_attributes($element, array('id', 'name', 'value', 'size', 'maxlength'));
  _form_set_class($element, array('form-text'));

  $extra = '';
  if ($element['#autocomplete_path'] && drupal_valid_path($element['#autocomplete_path'])) {
    drupal_add_library('system', 'drupal.autocomplete');
    $element['#attributes']['class'][] = 'form-autocomplete';

    $attributes = array();
    $attributes['type'] = 'hidden';
    $attributes['id'] = $element['#attributes']['id'] . '-autocomplete';
    $attributes['value'] = url($element['#autocomplete_path'], array('absolute' => TRUE));
    $attributes['disabled'] = 'disabled';
    $attributes['class'][] = 'autocomplete';
    $extra = '<input' . drupal_attributes($attributes) . ' />';
  }
  
  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  return $output . $extra;
}

function zenstrap_textarea($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'cols', 'rows'));
  _form_set_class($element, array('form-textarea'));

  $wrapper_attributes = array(
    'class' => array('form-textarea-wrapper', ''),
  );

  // Add resizable behavior.
  if (!empty($element['#resizable'])) {
    drupal_add_library('system', 'drupal.textarea');
    $wrapper_attributes['class'][] = 'resizable';
  }

  $output = '<div' . drupal_attributes($wrapper_attributes) . '>';
  $output .= '<textarea' . drupal_attributes($element['#attributes']) . '>' . check_plain($element['#value']) . '</textarea>';
  $output .= '</div>';
  return $output;
}

function zenstrap_password($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'password';
  element_set_attributes($element, array('id', 'name', 'size', 'maxlength'));
  _form_set_class($element, array('form-text'));
  return '<input'  . drupal_attributes($element['#attributes']) . " />" ;
}

function zenstrap_radio($variables) {
  $horizontal_forms = theme_get_setting('zenstrap_horizontal_forms');
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  if (isset($element['#return_value']) && isset($element['#value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));
  
  if ($horizontal_forms) {
    return '<label class="checkbox"><input' . drupal_attributes($element['#attributes']) . ' />' .
      (isset($element['#description'])? $element['#description'] : '') . '</label>';
  }
  else {
    return '<label class="checkbox"><input' . drupal_attributes($element['#attributes']) . ' />' .
    ((!isset($element['#title_display']) || 
    !in_array($element['#title_display'], array('invisible', 'none')))?
    ' ' . $element['#title'] : '') . '</label>';
  }
}

function zenstrap_checkbox($variables) {
  $horizontal_forms = theme_get_setting('zenstrap_horizontal_forms');
  $element = $variables['element'];
  $t = get_t();
  $element['#attributes']['type'] = 'checkbox';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox'));
  if ($horizontal_forms) {
    return '<label class="checkbox"><input' . drupal_attributes($element['#attributes']) . ' />' .
      (isset($element['#description'])? $element['#description'] : '') . '</label>';
  }
  else {
    return '<label class="checkbox"><input' . drupal_attributes($element['#attributes']) . ' />' .
    (isset($element['#title']) && (!isset($element['#title_display']) || 
      !in_array($element['#title_display'], array('invisible', 'none')))? 
      '<span>' . $element['#title'] . '</span>' : '') . '</label>';
  }
}

function zenstrap_date($variables) {
  $element = $variables['element'];

  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  if (!empty($element['#attributes']['class'])) {
    $attributes['class'] = (array) $element['#attributes']['class'];
  }
  $attributes['class'][] = 'container-inline';
  $attributes['class'][] = '';

  return '<div' . drupal_attributes($attributes) . '>' . drupal_render_children($element) . '</div>';
}

function zenstrap_select($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-select', ''));

  return '<select' . drupal_attributes($element['#attributes']) . '>' . form_select_options($element) . '</select>';
}

function zenstrap_fieldset($variables) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper', ));

  $output = '<fieldset' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    // Always wrap fieldset legends in a SPAN for CSS positioning.
    $output .= '<legend><span class="fieldset-legend">' . $element['#title'] . '</span></legend>';
  }
  $output .= '<div class="fieldset-wrapper">';
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

//TODO
function zenstrap_image_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'image';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['src'] = file_create_url($element['#src']);
  if (!empty($element['#title'])) {
    $element['#attributes']['alt'] = $element['#title'];
    $element['#attributes']['title'] = $element['#title'];
  }

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

