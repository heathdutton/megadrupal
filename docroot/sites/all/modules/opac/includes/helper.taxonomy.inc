<?php

/**
 * @file
 * Helper functions related to taxonomy.
 */


/**
 * Get term id for a given vocabulary, node field name  and node field value.
 *
 * @param string $vid
 *   Vocabulary identifier.
 *
 * @param string $field
 *   The node field which is tied to the vocabulary.
 *
 * @param string $value
 *   Value to compare with vocabulary terms.
 *
 * @return int
 *   Term identifier or NULL.
 */
function _opac_taxonomy_get_term($vid, $field, $value) {
  $query = new EntityFieldQuery();

  if ($field == 'name') {
    $query->entityCondition('entity_type', 'taxonomy_term', '=')
      ->propertyCondition('name', $value, '=')
      ->propertyCondition('vid', $vid, '=');
  }
  else {
    $query->entityCondition('entity_type', 'taxonomy_term', '=')
      ->propertyCondition('vid', $vid, '=')
      ->fieldCondition($field, 'value', $value, '=');
  }

  $terms = $query->execute();
  if (!isset($terms['taxonomy_term'])) {
    return 0;
  }
  $tid = key($terms['taxonomy_term']);
  return $tid;
}

/**
 * Create a new term for a given vocabulary.
 *
 * @param int $vid
 *   The vocabulary identifier for which create teh term.
 *
 * @param string $value
 *   The value of term name field to create.
 *
 * @param string $field
 *   The vocabulary field name to insert value in, if wanted.
 *
 * @return int
 *   Term identifier newly created.
 */
function _opac_taxonomy_new_term($vid, $value, $field = NULL) {
  $term = array(
    'name' => $value,
    'vid' => $vid,
  );
  if (isset($field)) {
    $term[$field] = array(
      'und' => array(
        array('value' => $value),
      ),
    );
  }
  $term = (object) $term;

  taxonomy_term_save($term);

  return $term->tid;
}

/**
 * Return vid for a given vocabulary machine name.
 *
 * @param string $name
 *   The vocabulary_machine_name.
 *
 * @return int
 *   Vocabulary id.
 */
function _opac_taxonomy_get_vid($name) {
  $vids = &drupal_static(__FUNCTION__ . $name);

  if (!isset($vids)) {
    // Check opac_taxonomy_vids cache.
    if ($cache = cache_get('opac_taxonomy_vids')) {
      $vids = $cache->data;
      // May be a new vocabulary has been created
      // and cache is not up to date.
      if (!isset($vids[$name])) {
        // Empty $vids array to pass in
        // the next condition.
        $vids = array();
      }
    }
    // Cache exists but $name is not in the array.
    // This could mean you need to rebuild the cache.
    if (!count($vids)) {
      // Load vocabulary objects.
      $vocabularies = taxonomy_get_vocabularies();
      // Build an array keyed by machine_name;
      foreach ($vocabularies as $vid => $vocabulary) {
        $vids[$vocabulary->machine_name] = $vid;
      }
      // Rebuild the cache.
      cache_set('opac_taxonomy_vids', $vids, 'cache');
    }
  }

  // Test once again if $name is set.
  if (isset($vids[$name])) {
    return $vids[$name];
  }
  return 0;
}
