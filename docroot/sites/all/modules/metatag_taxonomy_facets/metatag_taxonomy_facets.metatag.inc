<?php

/**
 * @file
 * Metatag integration for the metatag_taxonomy_facets module.
 */

/**
 * Implements hook_metatag_config_instance_info().
 */
function metatag_taxonomy_facets_metatag_config_instance_info() {
  $info = array();

  $single_as_combination = variable_get('metatag_taxonomy_facets_single_facets');
  $facets_selected = variable_get('metatag_taxonomy_facets_selected');
  $searchers = facetapi_get_searcher_info();
  foreach ($searchers as $searcher_name => $searcher_info) {
    $facets = facetapi_get_facet_info($searcher_name);
    $searcher_selected_facets = array();
    foreach ($facets as $facet_name => $facet_info) {
      if (metatag_taxonomy_facets_is_taxonomy_facet($facet_info) && isset($facets_selected[$searcher_name][$facet_name])) {
        $searcher_selected_facets[$facet_name] = $facet_info;
      }
    }
    if (!empty($searcher_selected_facets)) {
      $info += metatag_taxonomy_facets_get_combinations_info($searcher_info, $searcher_selected_facets, isset($single_as_combination[$searcher_name]));
    }
  }

  return $info;
}

/**
 * Implements hook_metatag_config_default().
 */
function metatag_taxonomy_facets_metatag_config_default() {
  $configs = array();

  $config = new stdClass();
  $config->instance = 'taxonomy_facets';
  $config->api_version = 1;
  $config->disabled = FALSE;
  $config->config = array(
    'title' => array('value' => ' | [site:name]'),
  );
  $configs[$config->instance] = $config;

  return $configs;
}
