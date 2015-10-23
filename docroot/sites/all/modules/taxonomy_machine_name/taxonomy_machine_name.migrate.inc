<?php

/**
 * @file
 * Taxonomy Machine Name Migrate Module File.
 *
 * https://www.drupal.org/node/2124685#comment-9341209
 */

/**
 * Class MigrationTaxonomyTermMachineName
 */
class MigrationTaxonomyTermMachineName extends Migration {
  /**
   * Constructor.
   *
   * @param array $arguments
   *   Arguments.
   */
  public function __construct($arguments = array()) {
    parent::__construct($arguments);

    // 'bundle' should contain the vocabulary machine name.
    // $this->destination = new MigrateDestinationTermMachineName($arguments['bundle']);
  }

  /**
   * Handle deferred terms.
   */
  protected function postImport() {
    /* @var $destination MigrateDestinationTermMachineName */
    $destination = $this->destination;

    // Links terms if parent not created before child.
    $limit = 100;

    // Keep going until we have no more orphan terms, or we're stuck in a
    // seemingly infinite loop (only happens with bad data).
    while ($destination->hasDeferredImports() && ($limit > 0)) {
      $destination->runDeferredImports();
      --$limit;
    }

    // Display an error if we still have orphans left over.
    if ($destination->hasDeferredImports()) {
      drupal_set_message(
        t('One or more terms could not be imported because their parent terms could not be found: (%terms)',
          array('%terms' => implode(', ', $destination->getDeferredImportKeys()))),
        'warning');
    }
  }
}

/**
 * Class MigrateTaxonomyTermMachineNameHandler
 */
class MigrateTaxonomyTermMachineNameHandler extends MigrateDestinationHandler {
  /**
   * Specify list of types supports.
   */
  public function __construct() {
    $this->registerTypes(array('taxonomy_term'));
  }

  /**
   * List of fields supported.
   *
   * @return array
   *   List.
   */
  public function fields() {
    return array(
      'machine_name' => t('Machine name'),
    );
  }
}

/**
 * Class MigrateDestinationTermMachineName
 */
class MigrateDestinationTermMachineName extends MigrateDestinationTerm {
  /**
   * @var array
   */
  protected $deferred;

  /**
   * Constructor for MigrateDestinationTermMachineName.
   *
   * @param string $bundle
   *   The machine name of the taxonomy vocabulary.
   *
   * @param array  $options
   *   An optional array of options to control the import process.
   */
  public function __construct($bundle, array $options = array()) {
    parent::__construct($bundle, $options);

    $this->deferred = array();
  }

  /**
   * Returns the fields that are exposed by this migration destination.
   *
   * The only additional field is "Parent (by machine name)".
   *
   * @param Migration $migration
   *   Migration instance.
   *
   * @return array
   *   Destination fields.
   */
  public function fields($migration = NULL) {
    $fields = parent::fields();

    $fields['parent_machine_name'] = t('Parent (by machine name).');

    return $fields;
  }

  /**
   * Attempt to find a term that has the same machine name.
   *
   * @param object $term
   *   A taxonomy term object with at least the machine
   *   name and vid properties defined.
   *
   * @return object
   *   A matching taxonomy term object if found, otherwise FALSE.
   */
  public function findMatchingTerm($term) {
    $candidate = taxonomy_term_machine_name_load(trim($term->machine_name), $term->vid);

    if (is_object($candidate)) {
      return $candidate;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Import the provided term, using information from the provided row.
   *
   * @param stdclass $term
   *   Term object to build. Prefilled with any fields mapped in the Migration.
   * @param stdclass $row
   *   Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *   Array of key fields (tid only in this case) of the term that was saved if
   *   successful. FALSE on failure.
   *
   * @throws \MigrateException
   */
  public function import(stdClass $term, stdClass $row) {
    // Look up parent name if provided.
    if (isset($term->parent_machine_name)) {
      $parent_machine_name = trim($term->parent_machine_name);

      if (!empty($parent_machine_name)) {
        // Default to bundle if no vocabulary machine name provided.
        if (!isset($term->vocabulary_machine_name)) {
          $term->vocabulary_machine_name = $this->bundle;
        }

        $parent_tid = taxonomy_term_machine_name_load($parent_machine_name, $term->vocabulary_machine_name);

        if (!empty($parent_tid)) {
          $term->parent = $parent_tid->tid;
        }
        else {
          // Store aside for post import process once parent will be created.
          $deferred_import = new stdClass();

          $deferred_import->term = clone $term;
          $deferred_import->row  = clone $row;

          $this->deferred[] = $deferred_import;
        }
      }
    }

    $tid = parent::import($term, $row);

    return $tid;
  }

  /**
   * Check whether or not this destination has terms for which the import was.
   *
   * "deferred" because their parent term could not be found.
   *
   * @return bool
   *   TRUE if there are terms for which import was deferred, FALSE otherwise.
   */
  public function hasDeferredImports() {
    return (count($this->deferred) > 0);
  }

  /**
   * Return the machine names of any terms for which import was deferred.
   *
   * @return array
   *   An array of machine names for all deferred term imports.
   */
  public function getDeferredImportKeys() {
    $keys = array();

    foreach ($this->deferred as $import) {
      $keys[] = $import->term->machine_name;
    }

    return $keys;
  }

  /**
   * Attempt to import any terms for which import was previously deferred.
   *
   * This will run import() again for each term and row, which will again
   * attempt to match up each term with its parent term.
   *
   * Any term for which a parent term is still not found will be returned to
   * the set of deferred imports. If the import is a multi-level hierarchy,
   * this method can be invoked multiple times until every level of the
   * hierarchy has been successfully imported.
   */
  public function runDeferredImports() {
    $current = $this->deferred;

    // Clear the list of deferred imports, to be rebuilt during the import.
    $this->deferred = array();

    foreach ($current as $import) {
      // This will automatically re-queue any imports that
      // still need to be deferred.
      $this->import($import->term, $import->row);
    }
  }
}

/**
 * Class MigrateTaxonomyTermReferenceMachineNameFieldHandler
 */
class MigrateTaxonomyTermReferenceMachineNameFieldHandler extends MigrateTaxonomyTermReferenceFieldHandler {
  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param string $type
   *   The field type.
   * @param mixed $instance
   *   Instance info for the field.
   * @param Migration $migration
   *   The migration context for the parent field. We can look at the mappings
   *   and determine which subfields are relevant.
   *
   * @return array
   *   Fields.
   */
  public function fields($type, $instance, $migration = NULL) {
    $fields = parent::fields($type, $instance, $migration);

    $fields['machine_name'] = t('Option: Set to TRUE to use machine_name instead of name to determinate source ID');

    return $fields;
  }

  /**
   * Prepare destination field.
   *
   * @param stdclass $entity
   *   Entitiy.
   * @param array    $field_info
   *   Field info.
   * @param array    $instance
   *   Field instance.
   * @param array    $values
   *   Values.
   *
   * @return array
   *   Result.
   *
   * @throws \FieldValidationException
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }
    if (empty($values[0])) {
      $values = array();
    }

    $tids = array();
    if (isset($arguments['source_type']) && $arguments['source_type'] == 'tid') {
      // Nothing to do. We have tids already.
      $tids = $values;
    }
    elseif ($values) {
      $vocab_name = $field_info['settings']['allowed_values'][0]['vocabulary'];
      $names      = taxonomy_vocabulary_get_names();

      // Get the vocabulary for this term.
      if (isset($field_info['settings']['allowed_values'][0]['vid'])) {
        $vid = $field_info['settings']['allowed_values'][0]['vid'];
      }
      else {
        $vid = $names[$vocab_name]->vid;
      }

      // Remove leading and trailing spaces in term names.
      $values = array_map('trim', $values);

      // Clean machine names if not matching default rule naming.
      if (!empty($arguments['machine_name'])) {
        $values = array_map('taxonomy_machine_name_clean_name', $values);
      }

      // Cannot use taxonomy_term_load_multiple() since we have an
      // array of names. It wants a singular value. This query may return
      // case-insensitive matches.
      $field_name     = (empty($arguments['machine_name']) ? 'name' : 'machine_name');
      $existing_terms = db_select('taxonomy_term_data', 'td')
        ->fields('td', array('tid', $field_name))
        ->condition('td.' . $field_name, $values, 'IN')
        ->condition('td.vid', $vid)
        ->execute()
        ->fetchAllKeyed(1, 0);
      // If we're ignoring case, change both the matched term name keys and the
      // source values to lowercase.
      if (isset($arguments['ignore_case']) && $arguments['ignore_case']) {
        $ignore_case    = TRUE;
        $existing_terms = array_change_key_case($existing_terms);
        foreach ($values as $value) {
          $lower_values[$value] = strtolower($value);
        }
      }
      else {
        $ignore_case = FALSE;
      }
      foreach ($values as $value) {
        if (isset($existing_terms[$value])) {
          $tids[] = $existing_terms[$value];
        }
        elseif ($ignore_case && isset($existing_terms[$lower_values[$value]])) {
          $tids[] = $existing_terms[$lower_values[$value]];
        }
        elseif (!empty($arguments['create_term'])) {
          $new_term       = new stdClass();
          $new_term->vid  = $vid;
          $new_term->name = $value;
          if (!empty($arguments['machine_name'])) {
            $new_term->machine_name = $value;
          }
          $new_term->vocabulary_machine_name = $vocab_name;

          // This term is being created with no fields, but we should still call
          // field_attach_validate() before saving, as that invokes
          // hook_field_attach_validate().
          field_attach_validate('taxonomy_term', $new_term);

          taxonomy_term_save($new_term);
          $tids[] = $new_term->tid;
          // Add newly created term to existing array.
          $existing_terms[$value] = $new_term->tid;
        }
        else {
          // No term is found for the source value and none is set to be
          // created: warn that data has not been imported.
          $migration = Migration::currentMigration();
          $migration->saveMessage(
            t(
              "No matching taxonomy term found for source value '@value' in vocabulary %vocab.",
              array(
                '@value' => $value,
                '%vocab' => $names[$vocab_name]->name,
              )
            ),
            MigrationBase::MESSAGE_INFORMATIONAL
          );
        }
      }
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $result   = array();
    $delta    = 0;

    foreach ($tids as $tid) {
      if (is_array($language)) {
        $current_language = $language[$delta];
      }
      else {
        $current_language = $language;
      }
      $result[$current_language][] = array('tid' => $tid);
      $delta++;
    }

    return $result;
  }
}
