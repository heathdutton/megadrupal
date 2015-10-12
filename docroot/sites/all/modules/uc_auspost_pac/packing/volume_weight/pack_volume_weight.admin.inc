<?php
/**
 * @file
 * Administration functions of the Volume and Weight packing algorithm.
 */

/**
 * Configuration form for settings Volume and Weight limits used in the algo.
 */
function pack_volume_weight_config($form, &$form_state, $packing_method, $quote_method) {

  // Load our settings.
  $settings = pack_volume_weight_get_settings($quote_method['id']);

  $form = array();

  $form['method_id'] = array(
    '#type' => 'value',
    '#value' => $quote_method['id'],
  );

  $form['volume'] = array(
    '#type' => 'fieldset',
    '#title' => t('Volume'),

    'volume_disabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Do <strong>not</strong> limit based on Volume.'),
      '#description' => t('AusPost International shipping methods are only weight based, which means that the volume can be ignored if desired.'),
      '#default_value' => $settings['volume_disabled'],
    ),

    'volume_limit' => array(
      '#type' => 'textfield',
      '#title' => t('Maximum Volume per Package'),
      '#field_suffix' => t('millimeters cubed'),
      '#size' => 15,
      '#default_value' => $settings['volume_limit'],
      '#states' => array(
        'required' => array(
          ':input[name="volume_disabled"]' => array('checked' => FALSE),
        ),
      ),
    ),
  );

  $form['weight'] = array(
    '#type' => 'fieldset',
    '#title' => t('Weight'),

    'weight_limit' => array(
      '#type' => 'textfield',
      '#title' => t('Maximum Weight per Package'),
      '#field_suffix' => t('grams'),
      '#size' => 15,
      '#default_value' => $settings['weight_limit'],
      '#required' => TRUE,
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',

    'save' => array(
      '#type' => 'submit',
      '#value' => t('Save settings'),
    ),

    'clear' => array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#validate' => array(),
      '#submit' => array('pack_volume_weight_config_clear'),
    ),

    'cancel' => array(
      '#markup' => l(t('Cancel and return to @method', array('@method' => $quote_method['title'])), 'admin/store/settings/quotes/auspost-pac/edit/' . $quote_method['id']),
    ),
  );

  return $form;
}

/**
 * Validate the volume and weight limits.
 *
 * @see pack_volume_weight_config().
 */
function pack_volume_weight_config_validate($form, &$form_state) {

  $values = $form_state['values'];

  // Volume must be numeric, preferrably an integer. If it's invalid, but the
  // volume limit is disabled, then just store 0.
  if (!ctype_digit($values['volume_limit'])) {
    if ($values['volume_disabled']) {
      form_set_value($form['volume']['volume_limit'], 0, $form_state);
    }
    else {
      form_set_error('volume_limit', t('Maximum Volume per Package must be a positive integer.'));
    }
  }

  // Weight must always be present and a positive integer.
  if (!ctype_digit($values['weight_limit'])) {
    form_set_error('weight_limit', t('Maximum Weight per Package must be a positive integer.'));
  }
}

/**
 * Save the volume and weight settings.
 *
 * @see pack_volume_weight_config().
 */
function pack_volume_weight_config_submit($form, &$form_state) {

  $values = $form_state['values'];
  $settings = array(
    'volume_disabled' => $values['volume_disabled'],
    'volume_limit' => $values['volume_limit'],
    'weight_limit' => $values['weight_limit'],
  );

  $variable_name = 'pack_volume_weight_' . $values['method_id'];
  variable_set($variable_name, $settings);

  // Keep track of all variables that have been saved by this module.
  $vars = variable_get('pack_volume_weight_vars', array());
  if (!in_array($variable_name, $vars)) {
    $vars[] = $variable_name;
    variable_set('pack_volume_weight_vars', $vars);
  }

  drupal_set_message(t('Settings have been saved.'));
}

/**
 * Clear any saved settings, meaning that the default will be used now.
 *
 * @see pack_volume_weight_config().
 */
function pack_volume_weight_config_clear($form, &$form_state) {

  $values = $form_state['values'];
  $variable_name = 'pack_volume_weight_' . $values['method_id'];
  variable_del($variable_name);

  $vars = variable_get('pack_volume_weight_vars', array());
  if (in_array($variable_name, $vars)) {
    $vars = array_diff($vars, array($variable_name));
    variable_set('pack_volume_weight_vars', $vars);
  }

  drupal_set_message(t('Settings reset to defaults.'));
}
