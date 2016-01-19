<?php
/**
 * @file
 * commerce_search_api_et.facetapi_defaults.inc
 */

/**
 * Implements hook_facetapi_default_facet_settings().
 *
 * @see commerce_search_api_facetapi_default_facet_settings()
 */
function commerce_search_api_et_facetapi_default_facet_settings() {
  return commerce_search_api_generate_facets('multilingual_product_display');
}
