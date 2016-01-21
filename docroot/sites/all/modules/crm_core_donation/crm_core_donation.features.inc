<?php
/**
 * @file
 * crm_core_donation.features.inc
 */

/**
 * Implements hook_default_crm_core_activity_type().
 */
function crm_core_donation_default_crm_core_activity_type() {
  $items = array();
  $items['cmcd_donation'] = entity_import('crm_core_activity_type', '{
    "type" : "cmcd_donation",
    "label" : "Donation",
    "weight" : 0,
    "activity_string" : "made a donation to",
    "description" : "This is for recording donation activity, online and offline.",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_ctools_plugin_api().
 */
function crm_core_donation_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
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
function crm_core_donation_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function crm_core_donation_node_info() {
  $items = array(
    'cmcd_page' => array(
      'name' => t('Donation Page'),
      'base' => 'node_content',
      'description' => t('This is a donation page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('This is an online donation page. It allows online donations to be received through your website.'),
    ),
  );
  return $items;
}
