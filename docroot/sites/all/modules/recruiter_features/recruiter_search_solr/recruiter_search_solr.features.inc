<?php
/**
 * @file
 * recruiter_search_solr.features.inc
 */

/**
 * Implements hook_default_search_api_server().
 */
function recruiter_search_solr_default_search_api_server() {
  $items = array();
  $items['solr_server'] = entity_import('search_api_server', '{
    "name" : "Solr Server",
    "machine_name" : "solr_server",
    "description" : "",
    "class" : "search_api_solr_service",
    "options" : {
      "http_user" : "",
      "http_pass" : "",
      "excerpt" : 0,
      "retrieve_data" : 0,
      "highlight_data" : 0,
      "name" : "Solr Server",
      "description" : "",
      "host" : "localhost",
      "port" : "' . variable_get('recruiter_solr_port', 8080) . '",
      "path" : "\\/' . variable_get('recruiter_solr_path', 'solr') .'"
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}
