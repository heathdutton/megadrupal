<?php
/**
 * @file
 * geocluster_solr_demo.features.inc
 */

/**
 * Implements hook_views_api().
 */
function geocluster_solr_demo_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_search_api_index().
 */
function geocluster_solr_demo_default_search_api_index() {
  $items = array();
  $items['geocluster_index'] = entity_import('search_api_index', '{
    "name" : "Geocluster Index",
    "machine_name" : "geocluster_index",
    "description" : null,
    "server" : "geocluster_server",
    "item_type" : "node",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "status" : { "type" : "boolean" },
        "search_api_language" : { "type" : "string" },
        "field_place:lat" : { "type" : "decimal" },
        "field_place:lon" : { "type" : "decimal" },
        "field_place:latlon" : { "type" : "string", "real_type" : "location" },
        "field_place:geohash" : { "type" : "string", "real_type" : "geohash" },
        "field_place:geocluster_index_12" : { "type" : "string" },
        "field_place:geocluster_index_11" : { "type" : "string" },
        "field_place:geocluster_index_10" : { "type" : "string" },
        "field_place:geocluster_index_9" : { "type" : "string" },
        "field_place:geocluster_index_8" : { "type" : "string" },
        "field_place:geocluster_index_7" : { "type" : "string" },
        "field_place:geocluster_index_6" : { "type" : "string" },
        "field_place:geocluster_index_5" : { "type" : "string" },
        "field_place:geocluster_index_4" : { "type" : "string" },
        "field_place:geocluster_index_3" : { "type" : "string" },
        "field_place:geocluster_index_2" : { "type" : "string" },
        "field_place:geocluster_index_1" : { "type" : "string" }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_search_api_server().
 */
function geocluster_solr_demo_default_search_api_server() {
  $items = array();
  $items['geocluster_server'] = entity_import('search_api_server', '{
    "name" : "Geocluster Server",
    "machine_name" : "geocluster_server",
    "description" : "",
    "class" : "geocluster_solr_service",
    "options" : {
      "host" : "localhost",
      "port" : "8123",
      "path" : "\\/solr",
      "http_user" : "admin",
      "http_pass" : "admin",
      "excerpt" : 0,
      "retrieve_data" : 0,
      "highlight_data" : 0,
      "http_method" : "POST"
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}
