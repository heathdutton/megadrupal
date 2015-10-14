<?php

/**
 * @file
 * CRUD functions for the Solr Best Bets module.
 */

/**
 * Saves a best bet entry to the database.
 *
 * @param array $values
 *   An associative array of the values being saved containing:
 *     - content_id:
 *     - query_text:
 *     - environment:
 *     - exclude: (optional)
 *     - weight: (optional)
 *
 * @return bool
 *   The success of the operation.
 */
function solr_best_bets_save(array $values) {
   $defaults = array(
     'environment' => FALSE,
     'content_id' => FALSE,
     'query_text' => FALSE,
     'exclude' => 0,
     'weight' => 0,
   );
   $keys = array(
     'environment' => TRUE,
     'content_id' => TRUE,
     'query_text' => TRUE,
   );

   // Merge in defaults, strip out all of the other stuff.
   $values = array_intersect_key($values + $defaults, $defaults);

   // Ensure we have all required values.
   if (!in_array(FALSE, $values, TRUE)) {
     $values['query_text'] = drupal_strtolower($values['query_text']);
     db_merge('solr_best_bets')
       ->key(array_intersect_key($values, $keys))
       ->fields(array_diff_key($values, $keys))
       ->execute();
     return TRUE;
   }

   return FALSE;
}

/**
 * Deletes best bet entries from the database.
 *
 * @param array $records
 *   An associative array of the values being saved containing:
 *     - content_id:
 *     - query_text:
 *     - environment:
 */
function solr_best_bets_delete(array $records) {
  $delete = db_delete('solr_best_bets');
  $or = db_or();
  foreach ($records as $values) {
    $or->condition(
      db_and()
        ->condition('environment', $values['environment'])
        ->condition('content_id', $values['content_id'])
        ->condition('query_text', $values['query_text'])
    );
  }
  $delete->condition($or);
  $delete->execute();
}

/**
 * Deletes best bet entries from the database.
 *
 * @param array $records
 *   An associative array of the values being saved containing:
 *     - content_id:
 *     - query_text:
 *     - environment:
 * @param bool $exclude
 *   Whether or not to exclude the content from the search results.
 */
function solr_best_bets_update_exclude(array $records, $exclude) {
  $update = db_update('solr_best_bets')->fields(array('exclude' => (int) $exclude));
  $or = db_or();
  foreach ($records as $values) {
    $or->condition(
      db_and()
        ->condition('environment', $values['environment'])
        ->condition('content_id', $values['content_id'])
        ->condition('query_text', $values['query_text'])
    );
  }
  $update->condition($or);
  $update->execute();
}

/**
 * Builds the elevate.xml markup.
 *
 * @param array $environment
 *   The environment definition.
 *
 * @return string
 *   The elevate.xml document.
 */
function solr_best_bets_get_xml(array $environment) {
  // Initialize XML document containing them elevate.xml config.
  $xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<elevate>\n";
  $field = check_plain($environment['unique field']);

  $sql = '
    SELECT content_id, query_text, exclude
    FROM {solr_best_bets}
    WHERE environment = :environment
    ORDER BY query_text, weight
  ';
  $result = db_query($sql, array(':environment' => $environment['name']));

  $text = FALSE;
  foreach ($result as $record) {

    // Contine to the next value if the ID cannot be resolved.
    // @todo Call watchdog?
    if (FALSE === ($transformed_id = solr_best_bets_transform_id($record->content_id, $environment))) {
      watchdog('solr_best_bets', 'ID not transformed to index value: @id', array('@id' => $record->content_id), WATCHDOG_WARNING);
      continue;
    }

    // Check whether we are moving on to a new query text.
    if ($record->query_text !== $text) {

      // Close the previous query element if one is open.
      if (FALSE !== $text) {
        $xml .= "  </query>\n";
      }

      // Open a new query element, save text for comparison in next row.
      $text = $record->query_text;
      $xml .= '  <query text="' . check_plain($text) . "\">\n";
    }

    // Transform the ID stored in the index, adds document to XML.
    $exclude = ($record->exclude) ? ' exclude="true"' : '';
    $xml .= '    <doc ' . $field . '="' . check_plain($transformed_id) . '"' . $exclude . "/>\n";
  }

  // Close the last query element if one is open.
  if (FALSE !== $text) {
    $xml .= "  </query>\n";
  }

  // Close the elevate element and return the XML document.
  $xml .= "</elevate>\n";
  return $xml;
}
