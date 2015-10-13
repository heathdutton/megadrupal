<?php
/**
 * @file
 * crm_core_petition.features.inc
 */

/**
 * Implements hook_default_crm_core_activity_type().
 */
function crm_core_petition_default_crm_core_activity_type() {
  $items = array();
  $items['cmcp_petition_signature'] = entity_import('crm_core_activity_type', '{
    "type" : "cmcp_petition_signature",
    "label" : "Petition Signature",
    "weight" : 0,
    "activity_string" : "signed the petition",
    "description" : "",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function crm_core_petition_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function crm_core_petition_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function crm_core_petition_node_info() {
  $items = array(
    'cmcp_petition' => array(
      'name' => t('On-line petition'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
