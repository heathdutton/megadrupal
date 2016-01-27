<?php

/**
 * @file
 * Code responsible for checking and synchronizing labels with Ooyala.
 */

/**
 * Retrieve a list of all labels from Ooyala and check them for changes.
 *
 * Note that the Ooyala module immediately notifies Ooyala of changes on the
 * Drupal side through hook_taxonomy() and hook_nodeapi(). Therfor we only need
 * to pull from Ooyala on cron jobs, bulk sending of information is only
 * necessary when initially setting up a vocabulary.
 *
 * @see ooyala_taxonomy_term_sync_push_labels()
 */
function ooyala_taxonomy_term_sync_pull_labels() {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  $ooyala_taxonomy = variable_get('ooyala_taxonomy', array());
  $complete_map = array();
  $vocabularies = taxonomy_get_vocabularies();

  // Update label information.
  foreach ($ooyala_taxonomy as $vid => $sync_settings) {
    if ($sync_settings['type'] !== 'labels' || empty($sync_settings['olid'])) {
      continue;
    }

    // Enable hierarchies on this vocabulary in case hierarchies are needed.
    if (!$vocabularies[$vid]->hierarchy) {
      $vocabularies[$vid]->hierarchy = 1;
      db_merge('taxonomy_vocabulary')
        ->key(array('vid' => $vocabularies[$vid]->vid))
        ->fields(array(
          'name' => $vocabularies[$vid]->name,
          'machine_name' => $vocabularies[$vid]->machine_name,
          'description' => $vocabularies[$vid]->description,
          'hierarchy' => $vocabularies[$vid]->hierarchy,
          'module' => $vocabularies[$vid]->module,
          'weight' => $vocabularies[$vid]->weight,
        ))
        ->execute();
    }

    // Get the list of labels, terms, and the term to label ID map.
    $labels = ooyala_api_label_list($sync_settings['olid']);

    // If there are no labels available, bail on any kind of syncing. This
    // prevents the deletion of labels in the event that the Ooyala API is
    // unavailable.
    if (!$labels) {
      return FALSE;
    }

    $terms[$vid] =& ooyala_taxonomy_term_list($vid);
    $map[$vid] =& ooyala_taxonomy_term_map($vid);

    // Synchronize information from Ooyala into Drupal.
    foreach ($labels as $olid => $label_path) {
      $tid = array_search($olid, $map[$vid]);
      $term = $tid ? $terms[$vid][$tid] : NULL;
      $parent_names = explode('/', substr($label_path, 1));
      $label_name = array_pop($parent_names);
      $parent_path = $parent_names ? '/' . implode('/', $parent_names) : '/';
      $parent_olid = $parent_path ? array_search($parent_path, $labels) : '';
      $parent_tid = $parent_olid ? array_search($parent_olid, $map[$vid]) : 0;

      // If the label does not yet have a taxonomy term, create one.
      if ($tid == FALSE) {
        $term = ooyala_taxonomy_create_term($olid, $label_path, $vid, $sync_settings['olid']);
        $tid = $term->tid;
        $complete_map[$tid] = $olid;
        watchdog('ooyala', 'Imported new term %name from Ooyala.', array('%name' => $term->name), WATCHDOG_NOTICE);
      }
      // The label already has a taxonomy term, check for changes.
      else {
        $original_term = clone($term);

        // If the label has been renamed, update it on the Drupal side.
        if ($label_name != $term->name) {
          $term->name = $label_name;
        }

        // If the label has been moved, update the parent on the Drupal side.
        if ($parent_tid != $term->parent) {
          $term->parent = $parent_tid;
        }

        // Save any changes.
        if ($original_term != $term) {
          $term->pulled_from_ooyala = TRUE;
          taxonomy_term_save($term);
          $terms[$vid][$term->tid] = $term;
        }
        $complete_map[$tid] = $olid;
      }
    }

    // Check for deleted labels and delete corresponding terms.
    foreach ($map[$vid] as $tid => $olid) {
      if (!isset($complete_map[$tid])) {
        taxonomy_term_delete($tid);
        watchdog('ooyala', 'Deleted term %name to match Ooyala.', array('%name' => $term->name), WATCHDOG_NOTICE);
      }
    }
  }
}

/**
 * Initial sync of Drupal taxonomy terms to Ooyala.
 *
 * This function is called when taxonomy syncing is first enabled. The normal
 * sync routine assumes that Ooyala is always the most up-to-date source of
 * labels (since Drupal immediately informs Ooyala of changes). However when
 * first enabling a vocabulary for syncing, Drupal needs to populate Ooyala
 * with labels.
 *
 * @param $vid
 *   The Drupal vocabulary to send to Ooyala as labels.
 * @param $olid
 *   The Ooyala label ID for the vocabulary.
 * @param $context
 *   The Batch API variable containing information about how many terms and
 *   nodes remain to be processed.
 *
 * @see ooyala_taxonomy_term_sync_pull()
 */
function ooyala_taxonomy_term_sync_push_labels($vid, $olid, &$context) {
  module_load_include('inc', 'ooyala', 'includes/ooyala.api');

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['vocabulary'] = taxonomy_vocabulary_load($vid);
    $context['sandbox']['terms'] = ooyala_taxonomy_term_list($vid);
    $context['sandbox']['tids'] = array_keys($context['sandbox']['terms']);
    $context['sandbox']['max'] = count($context['sandbox']['terms']);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['term_map'] = array();
    foreach ($context['sandbox']['terms'] as $index => $term) {
      $context['sandbox']['term_map'][$term->tid] = $index;
    }

    // Add this vocabulary to the list of results.
    $context['results']['vocabularies'][$vid] = $olid;
  }

  // Process terms by groups of 10.
  $count = 10;
  $tids = $context['sandbox']['tids'];
  $terms = $context['sandbox']['terms'];
  $term_map = $context['sandbox']['term_map'];
  $current = $context['sandbox']['progress'];
  $vocabulary = $context['sandbox']['vocabulary'];
  $end = min($current + $count, $context['sandbox']['max']);
  for ($current; $current < $end; $current++) {
    $tid = $tids[$current];
    $term = $terms[$tid];

    // Assemble the term parents as expected by ooyala_taxonomy_create_label().
    // This mimics the same format as taxonomy_get_parents().
    $pids = array_reverse($term->parents);
    $term->parents = array();
    foreach ($pids as $pid) {
      $term->parents[] = $terms[$pid];
    }

    // Generate a label path.
    $label = ooyala_taxonomy_create_label($term, $vid, $olid);

    // Send the term to Ooyala and retrieve a label ID.
    $term->olid = ooyala_api_label_add($label);
    ooyala_taxonomy_term_save($term);

    // Store result for post-processing in the finished callback.
    $context['results']['terms'][] = $term->tid;

    // Update our progress information.
    $context['sandbox']['progress'] = $current + 1;
    $context['message'] = t('Processing terms... %name', array('%name' => $term->name));
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch API finished callback. Clean up the push operation and enable.
 */
function ooyala_taxonomy_term_sync_push_finished($success, $results, $operations) {
  if (isset($results['terms'])) {
    $message = format_plural(count($results['terms']), '1 taxonomy term sent to Ooyala.', '@count taxonomy terms sent to Ooyala.');
  }

  if ($success) {
    // Enable syncing for the vocabulary.
    $ooyala_taxonomy = variable_get('ooyala_taxonomy', array());
    foreach ($results['vocabularies'] as $vid => $olid) {
      $ooyala_taxonomy[$vid] = array(
        'type' => 'labels',
        'olid' => $olid,
        'metadata_key' => '',
      );
    }
    variable_set('ooyala_taxonomy', $ooyala_taxonomy);
    drupal_set_message($message);
  }
  else {
    $message = t('An error occurred and synchronization did not complete.') . ' ' . $message;
    drupal_set_message($message, 'error');
  }
}

/**
 * Load a list of Drupal taxonomy terms within a vocabulary.
 *
 * Note that this function returns its value by reference. This makes it so that
 * newly added terms can be added to this list without doing the original query
 * to build the whole list over again.
 */
function &ooyala_taxonomy_term_list($vid, $reset = FALSE) {
  static $terms;

  if (!isset($terms) || $reset) {
    $terms = array();
  }

  if (!isset($terms[$vid])) {
    $query = db_select('taxonomy_term_data', 't');
    $query->join('taxonomy_term_hierarchy', 'th', 't.tid = th.tid');
    $query->leftJoin('ooyala_taxonomy', 'ot', 't.tid = ot.tid');
    $result = $query->fields('t')
      ->fields('th', array('parent'))
      ->fields('ot', array('olid', 'updated'))
      ->condition('t.vid', $vid)
      ->execute();
    $terms[$vid] = array();
    foreach ($result as $term) {
      $term->parent = isset($term->parent) ? $term->parent : 0;
      $terms[$vid][$term->tid] = $term;
    }

    // Add parents to each term after building the full list.
    foreach ($terms[$vid] as $tid => $term) {
      if ($term->parent && isset($terms[$vid][$term->parent])) {
        $parent = $terms[$vid][$term->parent];
        $terms[$vid][$tid]->parents = array($parent->tid);
        while ($parent->parent) {
          if ($terms[$vid][$parent->parent]) {
            $parent = $terms[$vid][$parent->parent];
            $terms[$vid][$tid]->parents[] = $parent->tid;
          }
        }
      }
    }
  }
  return $terms[$vid];
}

/**
 * Provide a term ID to Ooyala ID map.
 *
 * Note that this function returns its value by reference. This makes it so that
 * newly added terms can be added to this list without doing the original query
 * to build the whole list over again.
 */
function &ooyala_taxonomy_term_map($vid) {
  static $map;
  if (!isset($map[$vid])) {
    $terms = ooyala_taxonomy_term_list($vid);
    $map[$vid] = array();
    foreach ($terms as $tid => $term) {
      $map[$vid][$tid] = $term->olid;
    }
  }
  return $map[$vid];
}

/**
 * Load an individual Drupal taxonomy term, including Ooyala Label ID (olid).
 */
function _ooyala_taxonomy_term_load($tid = NULL, $olid = NULL) {
  $field = isset($tid) ? 't.tid' : "ot.olid";
  $arg = isset($tid) ? $tid : $olid;
  $query = db_select('taxonomy_term_data', 't');
  $query->leftJoin('ooyala_taxonomy', 'ot', 't.tid = ot.tid');
  $result = $query->fields('t')
    ->fields('ot', array('olid', 'updated'))
    ->condition($field, $arg)
    ->execute()
    ->fetch();
  return $result;
}

/**
 * Load all the terms related to a node in a particular vocabulary.
 *
 * @param $node
 *   The node whose terms are being checked.
 * @param $vid
 *   The vocabulary ID for which terms should be retrieved.
 * @return array|FALSE
 *   Returns an array of terms (even if empty) for the given node. If the
 *   vocabulary is not present on this node at all, FALSE is returned.
 */
function _ooyala_taxonomy_node_terms($node, $vid) {
  $fields_to_check = ooyala_taxonomy_get_taxonomy_fields();
  $terms = FALSE;
  foreach ($fields_to_check as $field_name => $field_vid) {
    if ($vid == $field_vid && isset($node->{$field_name}[LANGUAGE_NONE])) {
      if ($terms === FALSE) {
        $terms = array();
      }
      foreach ($node->{$field_name}[LANGUAGE_NONE] as $term) {
        // Since sometimes we are dealing with a taxonomy_term_reference and
        // other times we are dealing with an entityreference that
        // references a taxonomy_term, the keys for this array can vary.
        $tid = isset($term['tid']) ? $term['tid'] : $term['target_id'];
        $term = _ooyala_taxonomy_term_load($tid);
        $terms[$term->tid] = $term;
      }
    }
  }
  return $terms;
}

/**
 * Given an Ooyala label ID, create a Drupal taxonomy term.
 */
function ooyala_taxonomy_create_term($olid, $label_path, $vid, $vocabulary_olid = NULL) {
  // Load in our static variables.
  $labels = ooyala_api_label_list($vocabulary_olid);
  $terms =& ooyala_taxonomy_term_list($vid);
  $map =& ooyala_taxonomy_term_map($vid);

  // Check if this term already exists in Drupal.
  $tid = array_search($olid, $map);
  if ($tid && ($term = _ooyala_taxonomy_term_load($tid))) {
    return $term;
  }

  // If an Ooyala label ID is not known for the vocabulary, look it up from the
  // saved Ooyala taxonomy settings.
  if (!isset($vocabulary_olid)) {
    $ooyala_taxonomy = variable_get('ooyala_taxonomy', array());
    $vocabulary_olid = isset($ooyala_taxonomy[$vid]['olid']) ? $ooyala_taxonomy[$vid]['olid'] : NULL;
  }

  // Build a list of parents for this term and ensure they all exist.
  $parent_names = explode('/', substr($label_path, 1));
  $label_name = array_pop($parent_names);
  $parent_path = $parent_names ? '/' . implode('/', $parent_names) : '/';
  $parent_olid = $parent_path ? array_search($parent_path, $labels) : '';
  $parent_tid = $parent_olid ? array_search($parent_olid, $map) : 0;
  if ($parent_tid === FALSE) {
    $parent_term = ooyala_taxonomy_create_term($parent_olid, $label_path, $vid, $vocabulary_olid);
    $parent_tid = $parent_term->tid;
  }

  $term = array(
    'vid' => $vid,
    'name' => $label_name,
    'parent' => $parent_tid,
    'olid' => $olid,
  );

  $term = (object) $term;
  taxonomy_term_save($term);
  $terms[$term->tid] = $term;
  $map[$term->tid] = $olid;
  return $term;
}

/**
 * Given a taxonomy term or vocabulary, create an Ooyala label.
 */
function ooyala_taxonomy_create_label($term = NULL, $vid = NULL, $vocabulary_olid = NULL) {
  static $vocabulary_labels;

  // Get the VID from the term if needed.
  if (!isset($vid)) {
    $vid = $term->vid;
  }

  // If an Ooyala label ID is not known for the vocabulary, look it up from the
  // saved Ooyala taxonomy settings.
  if (!isset($vocabulary_olid)) {
    $ooyala_taxonomy = variable_get('ooyala_taxonomy', array());
    $vocabulary_olid = isset($ooyala_taxonomy[$vid]['olid']) ? $ooyala_taxonomy[$vid]['olid'] : NULL;
  }

  // Get the root label list from Ooyala, where we can lookup the label name.
  if (!isset($vocabulary_labels)) {
    module_load_include('inc', 'ooyala', 'includes/ooyala.api');
    $vocabulary_labels = ooyala_api_label_list('<root>');
  }

  // Set the label name based on the Ooyala label ID.
  $label = $vocabulary_labels[$vocabulary_olid];

  if (empty($label)) {
    return FALSE;
  }

  if (isset($term)) {
    // Build a list of parents for this term if needed.
    $parents = isset($term->parents) ? $term->parents : taxonomy_get_parents($term->tid);

    // Tag vocabularies for some reason are given a single parent of "0". Remove.
    if (isset($parents[0]) && $parents[0] === '0') {
      $parents = array();
    }

    // Assemble the rest of the label name from the parents and term name.
    foreach ($parents as $parent_term) {
      $label .= '/' . $parent_term->name;
    }
    $label .= '/' . $term->name;
  }

  return $label;
}
