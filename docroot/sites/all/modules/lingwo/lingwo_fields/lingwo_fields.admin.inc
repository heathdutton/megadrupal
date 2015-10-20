<?php

/**
 * @file
 * For the admin pages.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function lingwo_fields_admin_settings_form($form, &$form_state) {
  $form = _lingwo_language_form('_lingwo_fields_lingwo_language_form');
  $form = system_settings_form($form);
  if (!module_exists('lingwo_language')) {
    $form['#submit'][] = '_lingwo_fields_admin_generate_javascript';
  }
  return $form;
}

function _lingwo_fields_lingwo_language_form($language) {
  $v = $language->getValue('lingwo_fields_fields', LingwoFields::$settings->fields);

  $form = array();
  $form['lingwo_fields_fields'] = array(
    '#title' => t('Fields'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $form['lingwo_fields_fields']['javascript'] = array(
    '#title' => t('JavaScript'),
    '#type' => 'textarea',
    '#default_value' => $v['javascript'],
    '#description' => t('JavaScript code for the language definition.  This should atleast define the language\'s alphabet!'),
    '#access' => user_access('edit Lingwo fields JavaScript'),
  );

  // make a list of all the pos/field collections
  $pos = array();
  $pos_labels = array();
  $pos_options = LingwoEntry::getPosOptions($language, TRUE);
  foreach ($pos_options as $key => $value) {
    if (!empty($v['pos'][$key])) {
      $pos[$key] = $v['pos'][$key];
      unset($v['pos'][$key]);
    }
    $pos_labels[$key] = $value;
  }
  if (isset($v['pos'])) {
    foreach ($v['pos'] as $key => $value) {
      if (!empty($value)) {
        $pos[$key] = $value;
      }
    }
  }

  // loop through the pos/field collections and build the form
  foreach ($pos as $pos_name => $fields) {
    $form['lingwo_fields_fields']['pos'][$pos_name] = array(
      '#type' => 'fieldset',
      '#title' => isset($pos_labels[$pos_name]) ? check_plain($pos_labels[$pos_name]) : check_plain($pos_name),
    );
    foreach ($fields as $name => $field) {
      $element = _lingwo_fields_admin_field_form($name, $field);
      $element['#collapsible'] = TRUE;
      $element['#collapsed'] = TRUE;
      $element['remove'] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove this field'),
      );
      $form['lingwo_fields_fields']['pos'][$pos_name][$name] = $element;
    }
  }

  // form for adding a new field
  $add_field = _lingwo_fields_admin_field_form(t('Add field'), array());
  $add_field['pos'] = array(
    '#type' => 'select',
    '#title' => t('Part of speech'),
    '#options' => $pos_options,
    '#weight' => -11,
  );
  $add_field['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#weight' => -10,
    '#description' => t('The machine-readable name of this field. This will be used in the database and JavaScript. This name must contain only lowercase letters, numbers, and underscores. This name must be unique.'),
  );
  $add_field['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add field'),
    '#weight' => 10,
  );
  $form['lingwo_fields_fields']['add_field'] = $add_field;

  return $form;
}

function _lingwo_fields_admin_field_form($title, $field) {
  $element = array(
    '#type' => 'fieldset',
    '#title' => check_plain($title),
  );
  $element['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($field['label']) ? $field['label'] : '',
    '#description' => t('The human-readable name of this field. This will be displayed when editing this field on an entry. It is recommended that this name begin with a capital letter an contain only letters, numbers and spaces.'),
  );
  $element['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#options' => array(
      'form' => 'Form',
      'option' => 'Option',
      'class' => 'Class',
    ),
    '#default_value' => isset($field['type']) ? $field['type'] : '',
    '#description' => t('The type of field.  <em>Form</em> is a string which represents another form of the headword (ex. plural like "cars" on the entry "car").  <em>Option</em> is value which has one of a fixed set of values (ex. gender of nouns, could be "Masculine" or "Feminine".  <em>Class</em> is a boolean value that states this word belongs to a certain class (ex. some adverbs are "Comparable" which some aren\'t).'),
  );
  $element['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($field['description']) ? $field['description'] : '',
    '#description' => t('Help text to show the user when they are creating an entry with this form.'),
  );
  $element['options'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => isset($field['options']) ? $field['options'] : '',
    '#description' => t('The list of options when the field <em>Type</em> is <em>Option</em>.  Put one option on each line.  Use the pipe symbol if you want the option to have an alternate display name, for example: <em>m|Masculine</em>.'),
  );
  $element['automatic'] = array(
    '#type' => 'textarea',
    '#title' => t('Automatic JavaScript function'),
    '#default_value' => isset($field['automatic']) ? $field['automatic'] : '',
    '#description' => t('JavaScript function which get an <em>Entry</em> object as its only argument and should return the value automatically calculated for this field.  The return value depends on the type of the field: <em>Form</em> = <em>Word</em> object, <em>Option</em> = string, <em>Class</em> = boolean.'),
    '#access' => user_access('edit Lingwo fields JavaScript'),
  );
  $element['show'] = array(
    '#type' => 'textarea',
    '#title' => t('Show JavaScript function'),
    '#default_value' => isset($field['show']) ? $field['show'] : '',
    '#description' => t('JavaScript function which gets an <em>Entry</em> object as its only argument and should return a boolean, which indicates if this field should be shown or not.  This allows some fields to only be shown, depending on the values of other fields.'),
    '#access' => user_access('edit Lingwo fields JavaScript'),
  );

  return $element;
}

function _lingwo_fields_lingwo_language_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Add field')) {
    $values = $form_state['values']['lingwo_fields_fields']['add_field'];
    $name = $values['name'];
    $pos = $values['pos'];

    if (empty($name)) {
      form_set_error('lingwo_fields_fields][add_field][name', t('!name field is required.',
        array('!name' => t('Name'))));
    }
    elseif (!preg_match('!^[a-z0-9_]+$!', $name)) {
      form_set_error('lingwo_field_fields][add_field][name', t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
    }
    elseif (isset($form_state['values']['lingwo_fields_fields']['pos'][$pos][$name])) {
      form_set_error('lingwo_fields_fields][add_field][name', t('We already have a field named @name for @pos',
        array('@name' => $name, '@pos' => $pos)));
      form_set_error('lingwo_fields_fields][add_field][pos');
    }
  }
}

function _lingwo_fields_lingwo_language_form_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Add field')) {
    $values = $form_state['values']['lingwo_fields_fields']['add_field'];
    unset($values['submit']);

    $pos =  $values['pos'];
    $name = $values['name'];
    unset($values['pos']);
    unset($values['name']);

    $form_state['values']['lingwo_fields_fields']['pos'][$pos][$name] = $values;
  }
  unset($form_state['values']['lingwo_fields_fields']['add_field']);

  // process field removals
  if (isset($form_state['values']['lingwo_fields_fields']['pos'])) {
    foreach (array_keys((array) $form_state['values']['lingwo_fields_fields']['pos']) as $pos) {
      foreach ($form_state['values']['lingwo_fields_fields']['pos'][$pos] as $name => $field) {
        if (isset($field['remove']) && $field['remove']) {
          unset($form_state['values']['lingwo_fields_fields']['pos'][$pos][$name]);
        }
        else {
          unset($form_state['values']['lingwo_fields_fields']['pos'][$pos][$name]['remove']);
        }
      }
    }
  }
}

function _lingwo_fields_admin_generate_javascript($form, &$form_state) {
  LingwoFields::generateJavascript(language_default()->language, new _LingwoLanguageNull());
}

