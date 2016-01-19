<?php
/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_views_data_alter().
 */
function apachesolr_image_views_data_alter(&$data) {
  if (!module_exists('apachesolr_views')) {
    return;
  }

  foreach (apachesolr_load_all_environments() as $env_id => $environment) {
    $apachesolr_base_table = 'apachesolr__' . $env_id;

    // Get the list of the fields in index directly from Solr.
    try {
      $solr = apachesolr_get_solr($env_id);
      $solr_fields = $solr->getFields(0);
    }
    catch (Exception $e) {
      $solr_fields = array();
      watchdog('Apache Solr Image', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_WARNING);
    }

    foreach ($solr_fields as $solr_field_name => $solr_field) {
      if (0 === strpos($solr_field_name, 'sm_thumb_') && isset($data[$apachesolr_base_table][$solr_field_name])) {
        // Alter existing image fields, created by apachesolr_views module.
        $data_field = &$data[$apachesolr_base_table][$solr_field_name];
        $data_field['field']['handler'] = 'apachesolr_image_handler_field';
      }
    }
  }
}
