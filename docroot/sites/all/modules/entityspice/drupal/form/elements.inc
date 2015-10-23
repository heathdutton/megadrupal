<?php

/**
 * @file
 * Instead of using this definition for form elements:
 * form['some_element'] = array(
 *   'title' => t('Some title'),
 *   'type' => 'checkboxes',
 *   .
 *   .
 *   .
 * );
 *
 * Developers can use a function call like _entityspice_f_checkbox($args...)
 * where $arg0, $arg1 will be value of 'title', 'type', and ...
 *
 * I am really doubting now if this was actually a good idea...
 */

/**
 * Defines a base form element, including type, title, and if it's required.
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_base($type, $title, $req, $desc = NULL, $default = NULL) {
  $element = array(
    '#type' => $type,
    '#title' => $title,
    '#required' => (bool) $req,
  );

  if($desc) {
    $element['#description'] = $desc;
  }

  if($default !== NULL) {
    $element['#default_value'] = $default;
  }

  return $element;
}

/**
 * Defines a form element with options property.
 *
 * extends _entityspice_f_base() to add '#options'.
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_base_options(array $options, $type, $title, $req, $desc = NULL, $default = NULL) {
  $base = _entityspice_f_base($type, $title, $req, $desc, $default);
  $base['#options'] = $options;

  return $base;
}

/**
 * Defines a fieldset form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_fieldset($title, $collapsible = NULL, $collapsed = NULL, $desc = NULL) {
  $element = _entityspice_f_base('fieldset', $title, $desc);
  if($collapsible !== NULL) {
    $element['#collapsible'] = $collapsible;
  }
  if($collapsed !== NULL) {
    $element['#collapsed'] = $collapsed;
  }

  return $element;
}

/**
 * Defines a select form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_select($title, $def = NULL, $req = FALSE, array $options, $desc = NULL) {
  $element = _entityspice_f_base_options($options, 'select', $title, $req, $desc, $def);

  return $element;
}

/**
 * Defines a select form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_text($title, $def = NULL, $req = FALSE, $size = NULL, $maxlen = NULL, $desc = NULL) {
  $element = _entityspice_f_base('textfield', $title, $req, $desc, $def);

  if($size !== NULL) {
    $element['#size'] = $size;
  }
  if($maxlen !== NULL) {
    $element['#maxlength'] = $maxlen;
  }

  return $element;
}

/**
 * Defines a checkbox form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_checkbox($title, $def = NULL, $req = FALSE, $desc = NULL) {
  $element = _entityspice_f_base('checkbox', $title, $req, $desc, $def);

  return $element;
}

/**
 * Defines a checkboxes form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_checkboxes($title, $def = NULL, $req = FALSE, array $options, $desc = NULL) {
  $element = _entityspice_f_base_options($options ,'checkboxes', $title, $req, $desc, $def);
  return $element;
}

function _entityspice_f_radios($title, $def = NULL, $req = FALSE, array $options, $desc = NULL) {
  $element = _entityspice_f_base_options($options, 'radios', $title, $req, $desc, $def);

  return $element;
}

/**
 * Defines an actions form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_actions() {
  return array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );
}

/**
 * Defines a submit form element
 *
 * For more info see the file form/elements.inc in entityspice module.
 */
function _entityspice_f_submit($value = '', $name = NULL, array $submit = array()) {
  $value = $value ?: t('Submit');
  $element = array(
    '#type' => 'submit',
    '#value' => $value,
  );

  if ($name) {
    $element['#name'] = $name;
  }
  if ($submit) {
    $element['#submit'] = $submit;
  }

  return $element;
}
