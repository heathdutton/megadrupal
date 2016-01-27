<?php
/**
 * @file
 * Admin page callbacks for the uc_disable_zones module.
 */

/**
 * Form used on admin settings page to select which zones should be disabled.
 *
 * @ingroup forms
 */
function uc_disable_zones_settings_form($form, $form_state, $section = 'delivery') {
  $countries = _uc_disable_zones_get_country_zone_list();
  $currently_disabled = variable_get('uc_disable_zones_' . $section, array());

  $form['text'] = array(
    '#value' => t('Select zones to disable for @section.', array('@section' => $section)),
  );

  foreach ($countries as $country_id => $country_data) {
    $zones = $country_data['zones'];

    $form[$country_id] = array(
      '#title' => check_plain($country_data['name']),
      '#type' => 'fieldset',
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $form[$country_id][$country_id] = array(
      '#type' => 'checkboxes',
      '#options' => $zones,
      '#default_value' => isset($currently_disabled[$country_id]) ?  $currently_disabled[$country_id] : array(),
    );
  }

  $form['section'] = array(
    '#type' => 'hidden',
    '#value' => $section,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for uc_disable_zones_settings_form().
 *
 * @see uc_disable_zones_settings_form()
 */
function uc_disable_zones_settings_form_submit($form, &$form_state) {
  $disabled = array();

  foreach ($form_state['values'] as $country => $zones) {
    if (!is_numeric($country)) {
      continue;
    }

    foreach ($zones as $checked) {
      if ($checked) {
        $disabled[$country][] = $checked;
      }
    }
  }

  $section = isset($form_state['values']['section']) ? $form_state['values']['section'] : 'delivery';

  variable_set('uc_disable_zones_' . $section, $disabled);
  drupal_set_message(t('@section settings saved.', array('@section' => $section)));
}
