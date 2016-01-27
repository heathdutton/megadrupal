<?php

/**
 * @file
 * Admin settings
 */

require_once 'pinit.class.inc';

/**
 * Menu callback: settings form.
 */
function pinit_settings_form() {

  $types = PinItDrupalLayer::getNodeTypes();

  $type_options = array();
  foreach ($types as $id => $data) {
    $type_options[$id] = $data->name;
  }

  $sources = array(
    'title' => t('Title'),
    'body' => t('Body'),
  );

  $pin_count = array(
    'horizontal' => t('Horizontal'),
    'vertical' => t('Vertical'),
    'none' => t('No count'),
  );

  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => 'Global values for Pin It Button',
    '#collapsible' => TRUE,
  );
  $form['global'][PINIT_VAR_SOURCE] = array(
    '#title' => t('Source for the description:'),
    '#type' => 'radios',
    '#options' => $sources,
    '#default_value' => variable_get(PINIT_VAR_SOURCE, 'title'),
  );

  $form['global'][PINIT_VAR_CONTENT_TYPES_ENABLED] = array(
    '#title' => t('Enable for the following content types:'),
    '#type' => 'checkboxes',
    '#options' => $type_options,
    '#description' => t('The content types for which to enable Pin It Button. If none are selected then Pint It will be enabled for ALL content types.'),
    '#default_value' => variable_get(PINIT_VAR_CONTENT_TYPES_ENABLED, array()),
  );

  $form['defaults'] = array(
    '#type' => 'fieldset',
    '#title' => 'Default values for Pin It Button',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['defaults'][PINIT_VAR_DEFAULT_PINCOUNT] = array(
    '#title' => t('Default Pin Count'),
    '#type' => 'select',
    '#options' => $pin_count,
    '#default_value' => variable_get(PINIT_VAR_DEFAULT_PINCOUNT, ''),
  );
  $form['defaults'][PINIT_VAR_DEFAULT_DESCRIPTION] = array(
    '#title' => t('Default description'),
    '#type' => 'textarea',
    '#description' => t('The default description for Pin.'),
    '#default_value' => variable_get(PINIT_VAR_DEFAULT_DESCRIPTION, ''),
  );
  $form['defaults'][PINIT_VAR_DEFAULT_IMG] = array(
    '#title' => t('Default image'),
    '#type' => 'textfield',
    '#description' => t('Absolute or site-relative URL to an image to use for Pin It Button'),
    '#default_value' => variable_get(PINIT_VAR_DEFAULT_IMG, ''),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );

  return $form;
}

/**
 * Saving changes.
 */
function pinit_settings_form_submit($form, &$form_state) {

  $values = $form_state['values'];

  // Save source.
  variable_set(PINIT_VAR_SOURCE, $values[PINIT_VAR_SOURCE]);

  // Save content types enabled.
  variable_set(PINIT_VAR_CONTENT_TYPES_ENABLED, $values[PINIT_VAR_CONTENT_TYPES_ENABLED]);

  // Save default vars.
  variable_set(PINIT_VAR_DEFAULT_PINCOUNT, $values[PINIT_VAR_DEFAULT_PINCOUNT]);
  variable_set(PINIT_VAR_DEFAULT_DESCRIPTION, $values[PINIT_VAR_DEFAULT_DESCRIPTION]);
  variable_set(PINIT_VAR_DEFAULT_IMG, $values[PINIT_VAR_DEFAULT_IMG]);
  // Success.
  drupal_set_message(t('Options has been updated.'));
}
