<?php
/**
 * @file
 * party_dashboard_example.facetapi_defaults.inc
 */

/**
 * Implements hook_facetapi_default_facet_settings().
 */
function party_dashboard_example_facetapi_default_facet_settings() {
  $export = array();

  /* Search API @ Party Index: Party Hat */
  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@party_index::party_hat';
  $facet->searcher = 'search_api@party_index';
  $facet->realm = '';
  $facet->facet = 'party_hat';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '50',
    'dependencies' => array(
      'roles' => array(),
    ),
    'facet_mincount' => 1,
    'facet_missing' => FALSE,
    'flatten' => FALSE,
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
  );
  $export[$facet->name] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@party_index:block:party_hat';
  $facet->searcher = 'search_api@party_index';
  $facet->realm = 'block';
  $facet->facet = 'party_hat';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => FALSE,
    ),
    'sort_weight' => array(
      'active' => -51,
      'count' => -50,
      'display' => -49,
      'indexed' => -48,
    ),
    'sort_order' => array(
      'active' => 3,
      'count' => 3,
      'display' => 4,
      'indexed' => 4,
    ),
    'empty_behavior' => 'none',
    'soft_limit' => 5,
    'nofollow' => TRUE,
    'show_expanded' => FALSE,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
  );
  $export[$facet->name] = $facet;

  return $export;
}
