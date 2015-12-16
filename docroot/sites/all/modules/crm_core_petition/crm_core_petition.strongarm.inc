<?php
/**
 * @file
 * crm_core_petition.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function crm_core_petition_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_cmcp_petition';
  $strongarm->value = '1';
  $export['comment_cmcp_petition'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'crm_core_profile_node_type_enabled_cmcp_petition';
  $strongarm->value = 1;
  $export['crm_core_profile_node_type_enabled_cmcp_petition'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'crm_core_profile_node_type_label_cmcp_petition';
  $strongarm->value = 'Petition form';
  $export['crm_core_profile_node_type_label_cmcp_petition'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'crm_core_profile_node_type_profile_cmcp_petition';
  $strongarm->value = '';
  $export['crm_core_profile_node_type_profile_cmcp_petition'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_cmcp_petition';
  $strongarm->value = 0;
  $export['node_submitted_cmcp_petition'] = $strongarm;

  return $export;
}
