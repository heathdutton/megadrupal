<?php

/**
 * @file
 * Admin page callbacks for the Nimble CRM Contact module.
 */

/**
 * Settings form for Nimble CRM Contact module.
 */
function nimble_crm_contact_settings_form($form, &$form_state) {

  $form_state['default_values'] = array(
    'nimble_crm_send' => variable_get('nimble_crm_contact_send', 0),
    'nimble_crm_tags' => variable_get('nimble_crm_contact_tags', ''),
    'nimble_crm_custom_fields' => variable_get('nimble_crm_contact_custom_fields', array()),
  );

  module_load_include('inc', 'nimble_crm', 'nimble_crm.admin');
  $form += drupal_retrieve_form('nimble_crm_common_contact_settings_form', $form_state);

  return $form;
}

/**
 * Validate handler for Nimble CRM Contact settings form().
 */
function nimble_crm_contact_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $tags = preg_split("/\w,/", $values['nimble_crm_tags']);
  if (count($tags) > 5) {
    form_set_error('nimble_crm_tags', t('Maximum 5 tags are allowed during contact creation.'));
  }
}

/**
 * Validate handler for Nimble CRM Contact settings form().
 */
function nimble_crm_contact_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('nimble_crm_contact_send', $values['nimble_crm_send']);
  variable_set('nimble_crm_contact_tags', $values['nimble_crm_tags']);
  // Custom fields processing.
  unset($values['nimble_crm_custom_fields']['add_field']);
  unset($values['nimble_crm_custom_fields']['remove_field']);
  variable_set('nimble_crm_contact_custom_fields', $values['nimble_crm_custom_fields']);
}
