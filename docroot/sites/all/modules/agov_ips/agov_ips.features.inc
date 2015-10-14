<?php
/**
 * @file
 * agov_ips.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function agov_ips_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function agov_ips_node_info() {
  $items = array(
    'information_publication_scheme' => array(
      'name' => t('Information Publication Scheme'),
      'base' => 'node_content',
      'description' => t('Amendments to the Freedom of Information Act 1982 (Cth) introduced an Information Publication Scheme (IPS) which requires Australian Government agencies to publish certain information on their websites. The purpose of this content type is to allow for the construction and publishing of this information.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
