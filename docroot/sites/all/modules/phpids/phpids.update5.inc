<?php
/**
 * @file
 * hook_update_N() for Drupal 5.x
 */

/**
 * Implementation of hook_update_N().
 *
 * Update from 5.x-2.0 to 5.x-3.0
 */ 
function phpids_update_5300() {
  $ret = array();
  variable_set('phpids_path', realpath(dirname(__FILE__)));
  variable_set('phpids_tmppath', ini_get('upload_tmp_dir') .'phpids');
  variable_set('phpids_blocklevel', variable_get('phpids_warnlevel', 27));
  variable_set('phpids_warnlevel', variable_get('phpids_maillevel', 9));
  variable_set('phpids_loglevel', 1);
  variable_set('phpids_html_fields', '');
  variable_set('phpids_json_fields', '');
  variable_set('phpids_excl_fields', '');
  variable_set('phpids_warn_title', 'Security warning');
  variable_set('phpids_warn_text', 'We have detected malicious input and blocked your attempt.<br />If you keep experiencing problems but feel like you are doing nothing wrong, please contact the site administrator.');
  variable_set('phpids_testonly_activated', FALSE);
  variable_set('phpids_testonly_withmail', FALSE);
  variable_del('phpids_maillevel');
  $ret[] = array(
    'success' => TRUE,
    'query' => 'UPDATE MODULE SETTINGS',
  );    
  return $ret;
}