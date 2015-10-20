<?php

/**
 * @file
 * This file contains the API of the Fast Autocomplete module.
 */

/**
 * Implements hook_fac_service_info().
 *
 * Use this hook to add search service backends.
 * See fac_fac_service_info for more information.
 */
function hook_fac_service_info() {}

/**
 * Implements hook_fac_basic_title_search_query_alter().
 *
 * Use this hook to alter the query that is used to get the results using the
 * Basic title search backend service.
 *
 * @param  object $query
 *   The query that is used to get the results.
 */
function hook_fac_basic_title_search_query_alter(&$query) {}

/**
 * Implements hook_fac_search_api_query_alter().
 *
 * Use this hook to alter the query that is used to get the results using the
 * Search API backend service.
 *
 * @param  object $query
 *   The query that is used to get the results.
 */
function hook_fac_search_api_query_alter(&$query) {}

/**
 * Implements hook_fac_empty_result_alter().
 *
 * Use this hook to alter the empty result HTML. You can set the empty result
 * in the module settings form, but you can use this hook if you want to use a
 * quick links menu for instance.
 *
 * @param string $empty_result
 *   The HTML to use as the empty result.
 */
function hook_fac_empty_result_alter($empty_result) {}