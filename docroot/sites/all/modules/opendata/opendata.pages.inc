<?php

/**
 * @file
 * Contains page callback functions for opendata module.
 */

/**
 * Outputs dataset nodes into data.json.
 */
function opendata_data_json() {

  $nodes = opendata_get_datasets();
  $rows = opendata_build_rows($nodes);

  drupal_json_output($rows);
}

/**
 * Retrieves public, published dataset nodes.
 *
 * @return array
 *   An array of dataset nodes.
 */
function opendata_get_datasets() {
  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'opendata_dataset')
    ->propertyCondition('status', 1)
    ->propertyOrderBy('created', 'DESC')
    ->fieldCondition('field_opendata_access_level', 'value', 'public', '=')
    // Run the query as user 1.
    ->addMetaData('account', user_load(1));
    $entities = $query->execute();
    $nids = array_keys($entities['node']);
    $nodes = node_load_multiple($nids);

  return $nodes;
}

/**
 * Build PHP array and object structure in preparation for JSON output.
 *
 * @param array $nodes
 *   An array of opendata_dataset nodes.
 *
 * @return array
 *   An array of objects formatted as per Open Data schema specifications.
 */
function opendata_build_rows($nodes) {

  foreach ($nodes as $node) {

    $row = new stdClass();

    // Common Core fields. These are required, so we don't check for empty.
    $row->title = $node->title;
    $row->description = opendata_get_field_value($node, 'field_opendata_description', 'value');
    $row->keyword = opendata_get_field_value($node, 'field_opendata_keyword', 'value');
    $row->modified = opendata_get_field_value($node, 'field_opendata_modified', 'value');
    $row->publisher = opendata_get_field_value($node, 'field_opendata_publisher', 'value');
    $row->person = opendata_get_field_value($node, 'field_opendata_person', 'value');
    $row->mbox = opendata_get_field_value($node, 'field_opendata_mbox', 'email');
    $row->identifier = opendata_get_field_value($node, 'field_opendata_identifier', 'value');
    $row->accessLevel = opendata_get_field_value($node, 'field_opendata_access_level', 'value');

    // Common Core (Required if Applicable) fields.
    $row->dataDictionary = opendata_get_field_value($node, 'field_opendata_data_dictionary', 'value');
    $row->distribution = opendata_build_distributions($node);
    $row->webService = opendata_get_field_value($node, 'field_opendata_web_service', 'value');
    $row->license = opendata_get_field_value($node, 'field_opendata_license', 'value');
    $row->spatial = opendata_get_field_value($node, 'field_opendata_spatial', 'value');
    $row->temporal = opendata_get_field_value($node, 'field_opendata_temporal', 'value');

    // Extended fields.
    $row->issued = opendata_get_field_value($node, 'field_opendata_release_date', 'value');
    $row->accrualPeriodicity = opendata_get_field_value($node, 'field_opendata_frequency', 'value');
    $row->language = opendata_get_field_value($node, 'field_opendata_language', 'value');
    $row->granularity = opendata_get_field_value($node, 'field_opendata_granularity', 'value');
    $row->boolean = opendata_get_field_value($node, 'field_opendata_data_quality', 'value');
    $row->theme = opendata_get_field_value($node, 'field_opendata_theme', 'value');
    $row->references = opendata_get_field_value($node, 'field_opendata_references', 'value');
    $row->size = opendata_get_field_value($node, 'field_opendata_size', 'value');
    $row->landingPage = opendata_get_field_value($node, 'field_opendata_landing_page', 'value');
    $row->feed = opendata_get_field_value($node, 'field_opendata_feed', 'value');

    $rows[] = $row;
    unset($row);
  }

  return $rows;
}

/**
 * Sets a JSON row property given a node field name.
 *
 * @param object $node
 *   The node containing the field value.
 *
 * @param string $field_name
 *   The machine name of the field.
 *
 * @param string $column_name
 *   The column containing the primary value for a given field.
 *
 */
function opendata_get_field_value($node, $field_name, $column_name) {
  if (!empty($node->{$field_name}[LANGUAGE_NONE][0][$column_name])) {
    return $node->{$field_name}[LANGUAGE_NONE][0][$column_name];
  }
}

/**
 * Build the distributions property for a JSON row.
 *
 * @param object $node
 *   An opendata_dataset node.
 *
 * @return array
 *   An array of distribution objects.
 */
function opendata_build_distributions($node) {
  $distributions = array();

  if (isset($node->field_opendata_distribution[LANGUAGE_NONE])) {
    foreach ($node->field_opendata_distribution[LANGUAGE_NONE] as $delta => $value) {
      $distribution = new stdClass();
      $distribution->accessURL = $value['access_url'];
      if (!empty($value['size'])) {
        $distribution->size = $value['size'];
      }
      $distribution->format = $value['format'];
      $distributions[] = $distribution;
      unset($distribution);
    }
  }

  return $distributions;
}
