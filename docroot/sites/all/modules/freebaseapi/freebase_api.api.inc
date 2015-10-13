<?php
/**
 * @file
 * Usage docs.
 */


/**
 * Return IDs of predicates used in bundle mapping.
 *
 * Util to return an array of type IDs matching the freebase versions of any
 * known rdf mapping for a named entity type.
 *
 * Currently unused, but here for reference to illustrate three gotchas in the
 * process.
 * array_map() to convert rdf CURIE to freebase slash notation.
 * array_filter() to remove non-freebase items.
 * and array_values() to remap to a zero-based array.
 *
 * @param string $type
 *   Entity type.
 * @param string $bundle
 *   Bundle name.
 *
 * @return array
 *   A zero-numbered array of freebase-style types that match the
 *   named bundle definition. Empty array if none, or the rdf types are not
 *   freebase.
 */
function freebase_api_schema_ids_for_bundle($type = 'node', $bundle = 'page') {
  if (!module_exists('rdf_mapping')) {
    watchdog('freebase_api', 'Cannot load schema IDs without an rdf_mapping utility, returning empty set.', array(), WATCHDOG_WARNING);
    return array();
  }
  $bundle_info = rdf_mapping_bundle_load($type, $bundle);
  if (empty($bundle_info['rdf_mapping'])) {
    return array();
  }
  $rdf_mapping = $bundle_info['rdf_mapping'];
  $rdf_schema_ids = (array) $rdf_mapping['rdftype'];
  // Convert the rdf curies to freebase keys.
  $freebase_schema_ids = array_filter(array_map('freebase_api_rdf_curie_to_freebase_id', $rdf_schema_ids));
  // Renumber it, as freebase_suggest requires a zero-numbered simple array,
  // not an obj.
  $freebase_schema_ids = array_values($freebase_schema_ids);
  return $freebase_schema_ids;
}
