<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Remove fields which should not be exported.
 */
function contact_remove_fields(array $contact) {
  unset($contact['gender_id']);
  unset($contact['gender']);
  unset($contact['prefix_id']);
  unset($contact['prefix']);
  unset($contact['suffix_id']);
  unset($contact['suffix']);
  unset($contact['current_employer']);
  unset($contact['address_id']);
  unset($contact['street_address']);
  unset($contact['gender_id']);
  unset($contact['supplemental_address_1']);
  unset($contact['supplemental_address_2']);
  unset($contact['city']);
  unset($contact['postal_code_suffix']);
  unset($contact['postal_code']);
  unset($contact['geo_code_1']);
  unset($contact['geo_code_2']);
  unset($contact['state_province_id']);
  unset($contact['state_province_name']);
  unset($contact['state_province']);
  unset($contact['country_id']);
  unset($contact['country']);
  unset($contact['phone_id']);
  unset($contact['phone_type_id']);
  unset($contact['phone']);
  unset($contact['email_id']);
  unset($contact['email']);
  unset($contact['on_hold']);
  unset($contact['im_id']);
  unset($contact['provider_id']);
  unset($contact['im']);
  unset($contact['world_region']);
  unset($contact['worldregion_id']);
  return $contact;
}
/**
 * Implements hook_features_export_options().
 */
function contact_features_export_options() {
  $options = array();
  $api_contact_api = features_civicrm_api('Contact', 'get', array('contact_id' => 1));
  $options['contact_1'] = $api_contact_api['values'][1]['display_name'];
  return $options;
}

/**
 * Implements hook_features_export().
 */
function contact_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  $export['features']['contact']['contact_1'] = 'contact_1';
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function contact_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $civicrm_contact = array();';
  $code[] = '';
  $contact_api_data = features_civicrm_api('Contact', 'get', array('contact_id' => 1));
  $contact_api_data = contact_remove_fields($contact_api_data['values'][1]);
  $contact_code = features_var_export($contact_api_data);
  $code[] = "  \$civicrm_contact['contact_1'] = {$contact_code};";
  $code[] = '  return $civicrm_contact;';
  $code = implode("\n", $code);
  return array('civicrm_default_contact' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function contact_features_rebuild($module) {
  contact_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function contact_features_revert($module) {
  $civicrm_contacts = module_invoke($module, 'civicrm_default_contact');
  $contact = reset($civicrm_contacts);
  $existing_contact = features_civicrm_api('contact', 'get', array('id' => '1'));
  // Merge with code_contact.
  $merged_contact = array_merge_recursive_distinct($existing_contact['values'][$existing_contact['id']], $contact);
  $api_created_contact = features_civicrm_api('Contact', 'create', $merged_contact);
}
