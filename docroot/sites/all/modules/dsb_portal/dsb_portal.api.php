<?php

/**
 * @file
 * Module API documentation.
 */

/**
 * @defgroup dsb_portal_api dsb Portal
 * @{
 * dsb Portal allows Drupal sites to connect to the national catalog of the
 * Swiss Digital School Library. Communication with the national catalog happens
 * through a REST API. The module relies on the dsb Client PHP library for
 * communicating with this REST API. This library can be found here:
 * https://github.com/educach/dsb-client
 *
 * In order to use this module, users need to register with educa.ch to become
 * a content partner. Content partner receive a username (usually an email),
 * a private RSA key and a passphrase. This information can then be used by the
 * dsb Client library to communicate with the REST API.
 *
 * The dsb Portal module provides a basic search form. This form is highly
 * flexible, and tries to be as user-friendly as possible. However, many content
 * partners will probably want to customize the search behavior. dsb Portal
 * exposes many hooks that modules can implement to tweak certain aspects of
 * the system.
 *
 * @section search_form Search form
 *
 * Many content partners have different requirements for the search form. The
 * dsb Portal module provides a decent search form out of the box, but it can
 * easily be altered using different hooks.
 *
 * The most basic one would be the Drupal Core hook_form_alter() hook, to alter
 * dsb_portal_rest_api_search_form(). This allows content partners to tweak the
 * form definition itself.
 *
 * Another important aspect of the search form are the facets. The facets are a
 * tree of checkboxes that allow users to filter search results. The actual
 * facets are computed and returned by the REST API, but modules can decide
 * which facets the REST API should compute by implementing
 * hook_dsb_portal_rest_api_facets_alter().
 *
 * Facet names are also important. The default way to render facet names is
 * using data from the Ontology Server, but this is not always desirable. For
 * example, even the dsb Portal itself alters the name of language facets to
 * make them more user-friendly. To alter facet names, modules should implement
 * hook_dsb_portal_facet_name_alter().
 *
 * Make sure to read
 * @link https://dsb-api.educa.ch/latest/doc/#api-Search the official API documentation @endlink
 * for more information.
 *
 * @section search_behavior Search behavior
 *
 * Many content partners want to tweak the search behavior, for example by only
 * showing descriptions from specific content partners. This can be achieved
 * by altering the filters before they are sent to the REST API. Modules can
 * alter filters by implementing hook_dsb_portal_rest_api_filters_alter().
 *
 * By default, the REST API returns very few fields for each search result. It
 * is possible to alter the list of fields returned by implementing
 * hook_dsb_portal_rest_api_fields_alter().
 *
 * Make sure to read
 * @link https://dsb-api.educa.ch/latest/doc/#api-Search the official API documentation @endlink
 * for more information.
 *
 * @section description_altering Altering descriptions
 *
 * Loaded descriptions can be altered in numerous ways. The most obvious would
 * be at the theme layer: themes can override default templates and theme
 * callbacks to render descriptions differently. See dsb_portal_theme() for more
 * information.
 *
 * Other ways include altering a loaded LomDescription object by implementing
 * hook_dsb_portal_lom_description_load_alter(), as well as changing the way
 * language-aware properties are treated by altering the language fallback list,
 * thanks for hook_dsb_portal_lom_language_fallback_list_alter(). Finally,
 * some logos may not be adapted for use on your site. Logo paths can be
 * altered by implementing hook_dsb_portal_owner_logo_path_alter().
 *
 * @section example_module Example module
 *
 * dsb Portal ships with a sub-module called dsb Portal Customization Example.
 * This module tweaks the search form and search behavior. It is very well
 * commented and documented. Check out its source code for more information and
 * concrete examples on how to change the dsb Portal's behavior.
 *
 * @see https://github.com/educach/dsb-client
 * @see https://dsb-api.educa.ch/latest/doc
 * @}
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implements hook_dsb_portal_lom_language_fallback_list_alter().
 *
 * Some LOM-CH data fields are multilingual. By default, the current session
 * language will be searched for first, followed by:
 * - de
 * - fr
 * - it
 * - rm
 * - en
 *
 * It is possible to alter this list by implementing this hook.
 *
 * @param array &$languages
 *    The list of languages, in order of precedence.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_lom_language_fallback_list_alter(&$languages) {
  $languages[] = 'ja';
}

/**
 * Implements hook_dsb_portal_lom_description_load_alter().
 *
 * When a description is loaded from the REST API, it is returned wrapped in
 * a Educa\DSB\Client\Lom\LomDescription class. This class can be altered,
 * if needed. One example would be making sure the preview image is not
 * the full resolution one, but a smaller variant.
 *
 * @param Educa\DSB\Client\Lom\LomDescription $lom
 *    The LOM-CH description class about to be used.
 * @param array $data
 *    The data returned by the API, in array form.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_lom_description_load_alter(&$lom, $data) {

}

/**
 * Implements hook_dsb_portal_rest_api_facets_alter().
 *
 * The search form includes a tree of facets available for the search. The list
 * of requested facets can be altered by implementing this hook. Check the API
 * documentation to see what facets are available.
 *
 * @param array &$facets
 *    The list of facets to use.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_rest_api_facets_alter(&$facets) {
  $facets[] = 'keyword';
}

/**
 * Implements hook_dsb_portal_rest_api_query_alter().
 *
 * Alter the full text search query passed to the REST API.
 *
 * @param array &$query
 *    The full text search query that will be passed to the REST API.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_rest_api_query_alter(&$query) {
  $query = str_replace('rabbit', 'dog', $query);
}

/**
 * Implements hook_dsb_portal_rest_api_filters_alter().
 *
 * Alter the filters passed to the REST API. A filter must be keyed by a filter
 * name the REST API understands, and its value must be an array of values.
 * Multiple filter values are treated as OR values, not AND. Check the API
 * documentation to see what filters are available.
 *
 * @param array &$filters
 *    The list of filters to use, keyed by filter name. Each key is an array of
 *    filter values.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_rest_api_filters_alter(&$filters) {
  $filters['keyword'] = array('Math');
}

/**
 * Implements hook_dsb_portal_rest_api_fields_alter().
 *
 * Alter the fields the REST API should return for each result. Check the API
 * documentation to see what fields are available, and which ones are returned
 * by default.
 *
 * @param array &$fields
 *    The list of fields to use.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_rest_api_fields_alter(&$fields) {
  $fields[] = 'keyword';
}

/**
 * Implements hook_dsb_portal_owner_logo_path_alter().
 *
 * Alter the logo path of the content partner.
 *
 * @param string &$path
 *    The logo path.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_owner_logo_path_alter(&$path) {

}

/**
 * Implements hook_dsb_portal_facet_name_alter().
 *
 * Alter the human-readable name of a facet.
 *
 * @param string &$facet_name
 *    The name of the facet, to be altered.
 * @param array $facet
 *    The actual facet data, as returned by the API.
 *
 * @ingroup dsb_portal_api
 */
function hook_dsb_portal_facet_name_alter(&$facet_name, $facet) {

}

/**
 * @} End of "addtogroup hooks".
 */
