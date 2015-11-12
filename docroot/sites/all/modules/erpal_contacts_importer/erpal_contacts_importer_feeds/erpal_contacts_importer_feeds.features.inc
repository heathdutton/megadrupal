<?php
/**
 * @file
 * erpal_contacts_importer_feeds.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function erpal_contacts_importer_feeds_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  if ($module == "feeds_tamper" && $api == "feeds_tamper_default") {
    return array("version" => "2");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_cat_items_settings_info_alter().
 */
function erpal_contacts_importer_feeds_cat_items_settings_info_alter(&$data) {
  if (isset($data['erpal_contacts'])) {
    $data['erpal_contacts']->data['cat_tabs']['1'] = array(
      'path' => 'import/erpal-contacts-import',
      'path_router' => 'import/erpal-contacts-import',
      'path_original' => 'import/erpal-contacts-import',
      'path_pattern' => 'import/%',
      'path_query' => array(),
      'path_destination' => FALSE,
      'title' => 'Contacts importer',
      'css_id' => '',
      'css_class' => '',
      'active' => FALSE,
      'reusable' => FALSE,
      'machine_name' => '',
      'reusabled' => FALSE,
      'reusabled_machine_name' => NULL,
    ); /* WAS: '' */
  }
}
