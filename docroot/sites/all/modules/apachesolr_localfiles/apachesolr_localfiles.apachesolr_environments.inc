<?php

/**
 * @file
 * apachesolr_environments objects based on apachesolr_external_index module.
 */

/**
 * Implements hook_apachesolr_environments().
 */
function apachesolr_localfiles_apachesolr_environments() {
  $environments = array();

  $environment = new stdClass();
  $environment->api_version = 1;
  $environment->env_id = APACHESOLR_LOCALFILES_ID; // defined in the .module
  $environment->name = t('Solr server with Files Schema');
  $environment->url = 'http://localhost:8983/solr';
  $environment->service_class = '';
  $environment->conf = array(
    'apachesolr_read_only' => '0',
    'apachesolr_search_facet_pages' => '',
    'apachesolr_search_show_facets' => 0,
    'field_bias' => array(
      'content' => '1.0',
      'id' => '0',
      'label' => '0',
      'other_text' => '0.1',
      'tags_a' => '0',
      'tags_h1' => '0',
      'tags_h2_h3' => '0',
      'tags_h4_h5_h6' => '0',
      'tags_inline' => '0',
      'taxonomy_names' => '0',
      'timestamp' => '0',
      'tos_content_extra' => '0',
      'tos_name' => '0',
      'ts_comments' => '0',
      'url' => '0',
    ),
  );
  $environment->index_bundles = array();
  $environments[$environment->env_id] = $environment;

  return $environments;
}