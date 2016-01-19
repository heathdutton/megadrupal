<?php
/**
 * @file
 * Module file for the features_cicivrm module, which enables
 * the capture and management of civicrm entities in Drupal.
 */

/**
 * Remove fields which should not be exported.
 */
function domain_remove_fields(array $domain) {
  unset($domain['contact_id']);
  unset($domain['domain_email']);
  unset($domain['domain_phone']);
  unset($domain['domain_address']);
  unset($domain['from_email']);
  unset($domain['from_name']);
  return $domain;
}

/**
 * Implements hook_features_export_options().
 */
function domain_features_export_options() {
  $options = array();
  $api_domains_api = features_civicrm_api('Domain', 'get');
  foreach ($api_domains_api['values'] as $domain) {
    $options['domain_' . $domain['id']] = $domain['name'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function domain_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['civicrm'] = 'civicrm';
  $export['dependencies']['features_civicrm'] = 'features_civicrm';
  $export['dependencies']['features'] = 'features';
  $options = domain_features_export_options();
  foreach ($data as $domain_name) {
    $export['features']['domain'][$domain_name] = $domain_name;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function domain_features_export_render($module_name, $data, $export = NULL) {

  $domains = array();

  $code = array();
  $code[] = '  $civicrm_domain = array();';
  $code[] = '';
  foreach ($data as $domain_id_string) {
    $domain_id = substr($domain_id_string, 7);
    $domain_api_result = features_civicrm_api('Domain', 'get', array('id' => $domain_id));
    $domain_api_result = $domain_api_result['values'][$domain_id];
    $domain_api_result = domain_remove_fields($domain_api_result);
    $domain_code = var_export($domain_api_result, TRUE);
    $code[] = "  \$civicrm_domain['{$domain_id_string}'] = {$domain_code};";
  }
  $code[] = '  return $civicrm_domain;';
  $code = implode("\n", $code);
  return array('civicrm_default_domain' => $code);
}

/**
 * Implements hook_features_rebuild().
 */
function domain_features_rebuild($module) {
  domain_features_revert($module);
}

/**
 * Implements hook_features_revert().
 */
function domain_features_revert($module) {
  $civicrm_domains = module_invoke($module, 'civicrm_default_domain');
  foreach ($civicrm_domains as $domain) {
    $existing_domain = features_civicrm_api('Domain', 'get', array('id' => $domain['id']));
    // Merge with code_domain.
    $merged_domain = array_merge_recursive_distinct($existing_domain['values'][$existing_domain['id']], $domain);
    $api_domains_api = features_civicrm_api('Domain', 'create', $merged_domain);
  }
}
