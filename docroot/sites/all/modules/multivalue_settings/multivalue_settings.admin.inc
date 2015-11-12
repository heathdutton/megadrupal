<?php

/**
 * @file
 * Contains the settings administration forms for this module
 */

/**
 * Implements hook_form_FORM_ID_alter().
 * Adds the settings to the Field Settings form
 */
function multivalue_settings_form_field_ui_field_edit_form_alter(&$form, $form_state, $form_id) {
  // change the regular multiple values form element to an internal value
  unset($form['field']['cardinality']);
  $form['field']['cardinality'] = array(
    '#type'   => 'value',
    '#value'  => NULL,
  );

  // add a validate handler for this form
  $form['#validate'][] = 'multivalue_settings_validate_settings';

  // create the new settings form elements
  $allowed_max = 100;
  $initial_max = 30;
  $extra_max = 10;
  $allowed = $allowed_max;
  /* check if there is already a number of allowed values in form_state (which
   * means this is running from an AJAX call) or otherwise, if there is a value
   * that has been previously selected in this form */
  if (!empty($form_state['values']['field']['settings']['multivalue_settings']['allowed'])) {
    $allowed = $form_state['values']['field']['settings']['multivalue_settings']['allowed'];
  }
  elseif (!empty($form['#field']['settings']['multivalue_settings']['allowed'])) {
    $allowed = $form['#field']['settings']['multivalue_settings']['allowed'];
  }

  // get default values
  if (isset($form['#field']['settings']['multivalue_settings'])) {
    $values = $form['#field']['settings']['multivalue_settings'];
  }
  else $values = multivalue_settings_defaults();
  // the fieldset
  $form['field']['settings']['multivalue_settings'] = array(
    '#type'   => 'fieldset',
    '#title'  => 'Multivalue Settings',
  );

  // the checkbox to enable these settings and show the field
  $form['field']['settings']['multivalue_settings']['enable'] = array(
    '#type'   => 'checkbox',
    '#title'  => t('This field can contain more than one value'),
    '#default_value'  => $values['enable'],
  );

  // number of allowed values
  $options = array(FIELD_CARDINALITY_UNLIMITED => t('Unlimited')) + drupal_map_assoc(range(2, $allowed_max));
  $form['field']['settings']['multivalue_settings']['allowed'] = array(
    '#type'       => 'select',
    '#title'      => t('Number of values allowed'),
    '#default_value'  => $values['allowed'],
    '#description'    => t('This is the maximum number of values this field may have'),
    '#options'    => $options,
    '#ajax'       => array(
      'callback'  => 'multivalue_settings_required_callback',
      'wrapper'   => 'multivalue-settings-wrapper',
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="field[settings][multivalue_settings][enable]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // number of required values
  $options = array('0' => t('None'));
  if ($allowed != -1) $options += drupal_map_assoc(range(1, $allowed));
  else $options += drupal_map_assoc(range(1, $allowed_max));
  $form['field']['settings']['multivalue_settings']['required'] = array(
    '#type'       => 'select',
    '#title'      => t('Number of values required'),
    '#default_value'  => $values['required'],
    '#description'    => t('This is the minimum number of values this field is required to have. The usual required setting for the whole field is automatically turned off when Multivalue Settings is in use.'),
    '#options'    => $options,
    '#prefix'     => '<div id="multivalue-settings-wrapper">',
    '#states' => array(
      'visible' => array(
        ':input[name="field[settings][multivalue_settings][enable]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // number of values to show initially
  if ($allowed != -1) $options = drupal_map_assoc(range(1, min($allowed, $initial_max)));
  else $options = drupal_map_assoc(range(1, $initial_max));
  $form['field']['settings']['multivalue_settings']['initial'] = array(
    '#type'       => 'select',
    '#title'      => t('Number of initial fields'),
    '#default_value'  => $values['initial'],
    '#description'    => t('This is the number of field instances to show if no values are saved to this field yet'),
    '#options'    => $options,
    '#states' => array(
      'visible' => array(
        ':input[name="field[settings][multivalue_settings][enable]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // number of extra empty fields to show on edit
  $options = array('0' => t('None'));
  if ($allowed != -1) {
    $options += array('-1' => t('Show full number of allowed fields'));
    $options += drupal_map_assoc(range(1, min($allowed - 1, $extra_max)));
  }
  else $options += drupal_map_assoc(range(1, $extra_max));
  $form['field']['settings']['multivalue_settings']['extra'] = array(
    '#type'       => 'select',
    '#title'      => t('Number of extra empty fields'),
    '#default_value'  => $values['extra'],
    '#description'    => t('This is the maximum number of extra empty field instances to show when the field already has some values'),
    '#options'    => $options,
    '#suffix'     => '</div>',
    '#states' => array(
      'visible' => array(
        ':input[name="field[settings][multivalue_settings][enable]"]' => array('checked' => TRUE),
      ),
    ),
  );
}

/**
 * Ajax callback for the admin form
 */
function multivalue_settings_required_callback($form, &$form_state) {
  $return = array('required' => $form['field']['settings']['multivalue_settings']['required'],
                  'initial' => $form['field']['settings']['multivalue_settings']['initial'],
                  'extra' => $form['field']['settings']['multivalue_settings']['extra'],
            );
  return $return;
}

/**
 * Validate handler for the field settings
 */
function multivalue_settings_validate_settings($form, &$form_state) {
  // fix cardinality
  $settings = $form_state['values']['field']['settings']['multivalue_settings'];
  if ($settings['enable'] == 0) {
    $form_state['values']['field']['cardinality'] = 1;
  }
  else {
    // set cardinality always to 'Unlimited' so that we get the add more button
    $form_state['values']['field']['cardinality'] = FIELD_CARDINALITY_UNLIMITED;
  }
  // set field required to FALSE
  $form_state['values']['instance']['required'] = FALSE;
}

/**
 * Returns an array of default values to use for the admin form
 */
function multivalue_settings_defaults() {
  return array(
    'enable'   => 0,
    'allowed'   => 2,
    'required'  => 1,
    'initial'   => 1,
    'extra'     => 0,
  );
}