<?php

/**
 * @file
 * Configuration settings for 'verge_lib_measure'.
 */

/**
 * Custom implementation: verge_lib_measure_settings_form().
 * @see system_settings_form()
 */
function verge_lib_measure_settings_form($form) {
  $form = array();

  // Display DIV option.
  $form['fieldset_div'] = array(
    '#type' => 'fieldset',
    '#title' => t('Measurement results div'),
    '#collapsible' => TRUE,
  );
  $form['fieldset_div']['verge_lib_measure_div_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display viewport dimensions. (Enabled by default)'),
    '#description' => t('If checked a div containing the actual viewport dimensions will be appended through jQuery to the html body. Mind the possible <a href="@permissions">permissions</a>.', array(
      '@permissions' => url('admin/people/permissions', array(
        'alias' => TRUE,
        'fragment' => 'module-verge_lib_measure',
      )
    ))),
    '#default_value' => variable_get('verge_lib_measure_div_enabled', VERGE_LIB_MEASURE_DIV_ENABLED),
  );

  // Enable CSS option.
  $form['fieldset_div']['fieldset_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default css'),
    '#collapsible' => TRUE,
    '#states' => array(
      'visible' => array(
        // Show only if DIV is enabled.
        ':input[name="verge_lib_measure_div_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['fieldset_div']['fieldset_css']['verge_lib_measure_css_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load css from module. (Enabled by default)'),
    '#description' => t('If checked the css file packed with the module will be used. If not checked one can easily set up custom styles.'),
    '#default_value' => variable_get('verge_lib_measure_css_enabled', VERGE_LIB_MEASURE_CSS_ENABLED),
  );

  // jQuery.append TARGET option.
  $form['fieldset_div']['fieldset_append'] = array(
    '#type' => 'fieldset',
    '#title' => t('jQuery.append() target'),
    '#collapsible' => TRUE,
    '#states' => array(
      'visible' => array(
        // Show only if DIV is enabled.
        ':input[name="verge_lib_measure_div_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['fieldset_div']['fieldset_append']['verge_lib_measure_target_id'] = array(
    '#type' => 'textfield',
    '#title' => t("Target ID. (Default: 'page')"),
    '#description' => t('ID of html element the result div shall be appended to. Without hash prefix!'),
    '#default_value' => variable_get('verge_lib_measure_target_id', VERGE_LIB_MEASURE_TARGET_ID),
  );

  // Custom form validation.
  $form['actions']['submit']['#validate'][] = 'verge_lib_measure_settings_form_validate';

  // Add the form reset button known from 6.x.
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('_verge_lib_measure_settings_form_reset'),
    '#weight' => 50,
  );

  return system_settings_form($form);
}

/**
 * Mimes the 6.x system_settings_form_submit() behavior.
 */
function _verge_lib_measure_settings_form_reset($form, &$form_state) {
  // Re-set variables.
  variable_set('verge_lib_measure_div_enabled', VERGE_LIB_MEASURE_DIV_ENABLED);
  variable_set('verge_lib_measure_css_enabled', VERGE_LIB_MEASURE_CSS_ENABLED);
  variable_set('verge_lib_measure_target_id', VERGE_LIB_MEASURE_TARGET_ID);

  // Set message.
  drupal_set_message(t('The configuration options have been reset to their default values.'), 'status', FALSE);
}

/**
 * Validation function for verge_lib_measure_settings_form().
 */
function verge_lib_measure_settings_form_validate($form, $form_state) {
  if ($form_state['values']['verge_lib_measure_target_id'] != drupal_html_class($form_state['values']['verge_lib_measure_target_id'])) {
    form_set_error('verge_lib_measure_target_id', t('Illegal id'));
  }
}
