<?php
/**
 * @file
 * Freebase API utility routines.
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 * @version 2010
 */


/**
 * Convert 'freebase:a.type_name' to '/a/type_name'.
 *
 * Freebase RDF predicates are a bit different from the ones that freebase JSON
 * expects.
 *
 * eg from 'fb:en.bob_dylan' get '/en/bob_dylan'
 */
function freebase_api_rdf_curie_to_freebase_id($rdf_curie) {
  if (substr($rdf_curie, 0, 9) == FB_PREFIX . ':') {
    return '/' . str_replace('.', '/', substr($rdf_curie, 9));
  }
  return '';
}

/**
 * Given a Freebase ID, return an RDF CURIE.
 *
 * eg from '/en/bob_dylan' get 'fb:en.bob_dylan'
 */
function freebase_api_freebase_id_to_rdf_curie($freebase_id) {
  return FB_PREFIX . ':' . str_replace('/', '.', trim($freebase_id, '/'));
}


/**
 * Create Drupal machine-names from freebase element names.
 *
 * When working with cck, Field names are limited to 32 characters.
 * This MUST be kept to or things like tablenames break.
 *
 * @param string $name
 *   Freebase style identifier.
 *
 * @return string
 *   Appropriate to be used as a fieldname.
 */
function freebase_api_machine_name($name) {
  while (strlen($name) > 32 && strstr($name, '/')) {
    // Slice leftmost fragments off until it's short enough.
    $name = preg_replace('|^[^/]*/|', '', $name);
  }
  return str_replace('/', '_', ltrim($name, '/'));
}


/**
 * Retrieve text or binary values of blobs not actually in the data graph.
 *
 * Use the 'trans' api.
 * See http://mql.freebaseapps.com/ch04.html#transservice
 *
 * Freebase does not actually store big lumps of text in the DB, just ID
 * references to it. This function dereferences those ids and returns the actual
 * data.
 *
 * DEPRECATED.
 */
function freebase_api_fetch_trans($translation, $id) {
  // 2014 this has gone away and I can't find where to get it back from.
  // The only way I can locate the text is from a topic lookup.
  // Use freebase_api_fetch_text() instead.
  return NULL;

  // Previously it was:
  // $uri = FB_API_URI . "api/trans/$translation$id";
  // return file_get_contents($uri);
}

/**
 * Return the full text of an article.
 *
 * It can be tricky to find how to get at this through the MQL.
 * The only way I have found is to identify the article ID, then do a topic
 * lookup on it.
 *
 * @param string $id
 *   FB ID.
 *
 * @return string
 *   Full text. NULL if unavailable.
 */
function freebase_api_fetch_text($id) {
  // 2014 this has gone away and I can't find where to get it back from.
  // The only way I can locate the text is from a topic lookup.
  // EG https://www.googleapis.com/freebase/v1/topic/book/book
  $result = freebase_api_run_topic_lookup($id);
  // If data is there, return it, otherwise NULL is fine.
  return @$result['property']['/common/document/text']['values'][0]['value'];
}
