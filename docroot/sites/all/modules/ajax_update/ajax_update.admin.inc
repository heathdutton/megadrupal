<?php

/**
 * @file
 * Administrative include file for AJAX Update.
 */

/**
 * Administrative form for AJAX Update.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state, passed by reference.
 *
 * @return array
 *   The form array.
 */
function ajax_update_admin_form(array $form, array &$form_state) {
  $settings = variable_get('ajax_update');
  $form['ajax_update'] = array(
    '#tree' => TRUE,
  );
  $form['ajax_update']['ajax_html_id_exclude'] = array(
    '#type' => 'fieldset',
    '#description' => t('Exclude the sending of HTML IDs via ajax on these forms'),
    '#tree' => TRUE,
  );
  $form['ajax_update']['ajax_html_id_exclude']['options'] = array(
    '#title' => t('Forms to process'),
    '#type' => 'radios',
    '#default_value' => isset($settings['ajax_html_id_exclude']['options']) ? $settings['ajax_html_id_exclude']['options'] : 'select_forms',
    '#options' => array(
      'all_forms' => t('All forms'),
      'ajax_forms' => t("AJAXified forms (may be very slow, and probably doesn't serve a purpose)"),
      'select_forms' => t('Only forms with #ajax_html_id_exclude set to true'),
      'disable' => t('Disable on all forms, no matter what.'),
    ),
  );

  return system_settings_form($form);
}
