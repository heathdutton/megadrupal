<?php

/**
 * @file
 * Current Search default hooks.
 */

/**
 * Implements hook_current_search_default_items().
 */
function party_dashboard_example_current_search_default_items() {
  $items = array();

  $item = new stdClass();
  $item->disabled = FALSE; /* Edit this to true to make a default item disabled initially */
  $item->api_version = 1;
  $item->name = 'party_dashboard';
  $item->label = 'Party Dashboard';
  $item->settings = array(
    'items' => array(
      'results' => array(
        'id' => 'text',
        'label' => 'Results',
        'text' => 'One match found',
        'plural' => 1,
        'text_plural' => 'Showing results [facetapi_results:start-count] to [facetapi_results:end-count] of [facetapi_results:result-count]',
        'plural_condition' => 'facetapi_results:result-count',
        'wrapper' => 1,
        'element' => 'em',
        'css' => 1,
        'classes' => 'current-search-item-counts',
        'weight' => '-50',
      ),
      'active_items' => array(
        'id' => 'active',
        'label' => 'Active items',
        'pattern' => '[facetapi_active:active-value]',
        'keys' => 1,
        'css' => 0,
        'classes' => '',
        'nofollow' => 1,
        'weight' => '-49',
      ),
    ),
    'advanced' => array(
      'empty_searches' => 0,
    ),
  );
  $items[$item->name] = $item;

  return $items;
}
