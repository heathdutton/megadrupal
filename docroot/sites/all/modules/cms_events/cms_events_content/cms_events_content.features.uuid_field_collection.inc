<?php
/**
 * @file
 * cms_events_content.features.uuid_field_collection.inc
 */

/**
 * Implements hook_uuid_features_default_field_collections().
 */
function cms_events_content_uuid_features_default_field_collections() {
  $field_collections = array();

  $field_collections[] = array(
    'field_name' => 'field_glazed_content_design',
    'default_revision' => 1,
    'archived' => 0,
    'uuid' => 'f8b8d853-eca8-4aef-b4d2-00e09fbfed66',
    'field_glazed_content_region' => array(
      'und' => array(
        0 => array(
          'value' => 'glazed-util-full-width-contentt',
        ),
      ),
    ),
    'field_glazed_header_image' => array(),
    'field_glazed_hide_regions' => array(
      'und' => array(
        0 => array(
          'value' => 'page_title',
        ),
      ),
    ),
    'rdf_mapping' => array(),
    'url_alias' => array(),
  );
  return $field_collections;
}
