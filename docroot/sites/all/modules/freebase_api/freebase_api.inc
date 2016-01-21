<?php
/**
 * @file Freebase API utility routines.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 * @version 2010
 */

define('FB_URI', 'http://www.freebase.com/');
define('FB_NS', 'http://rdf.freebase.com/ns/');
define('FB_SEARCH_URI', FB_URI . 'api/service/search');
define('FB_API_URI', 'http://api.freebase.com/');


/**
 * Convert 'freebase:a.type_name' to '/a/type_name'
 *
 * Freebase RDF predicates are a bit different from the ones that freebase JSON
 * expects.
 *
 * eg from 'fb:en.bob_dylan' get '/en/bob_dylan'
 */
function freebase_api_rdf_curie_to_freebase_id($rdf_curie) {
  if (substr($rdf_curie, 0, 9) == 'freebase:') {
    return '/' . str_replace('.', '/', substr($rdf_curie, 9));
  }
  return '';
}
/**
 * Given a Freebase ID, return an RDF CURIE
 *
 * eg from '/en/bob_dylan' get 'fb:en.bob_dylan'
 *
 * @param string $freebase_id
 */
function freebase_api_freebase_id_to_rdf_curie($freebase_id) {
  return 'freebase:' . str_replace('/', '.', trim($freebase_id, '/'));
}


/**
 * Utility to create machine-names that Drupal is more comfortable with from
 * freebase element names
 *
 * When working with cck, Field names are limited to 32 characters
 * This MUST be kept to or things like tablenames break
 *
 * @return string appropriate to be used as a fieldname
 */
function freebase_api_machine_name($name) {
  while (strlen($name) > 32 && strstr($name, '/')) {
    // Slice leftmost fragments off until it's short enough.
    $name = preg_replace('|^[^/]*/|', '', $name);
  }
  return str_replace('/', '_', ltrim($name, '/'));
}


/**
 * Use the 'trans' api to retrieve text or binary values of blobs nod actually
 * in the data graph.
 * @see http://mql.freebaseapps.com/ch04.html#transservice
 *
 * Freebase does not actually store big lumps of text in the DB, just ID
 * references to it. This function dereferences those ids and returns the actual
 * data.
 */
function freebase_api_fetch_trans($translation, $id) {
  $uri = FB_API_URI . "api/trans/$translation/$id";
  return file_get_contents($uri);
}

