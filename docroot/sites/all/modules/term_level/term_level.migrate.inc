<?php
/**
 * @file
 * Term Level Migrate hooks and implementation.
 */

/*
 * Implements hook_migrate_api().
 */
function term_level_migrate_api() {
  return array(
    'api' => 2,
    'field handlers' => array(
      'MigrateTermLevelFieldHandler',
    )
  );
}

/**
 * Primary value passed to this field must be tid of the referenced term.
 *
 * Subfields are used to specify the level.
 * Works similar to term field migration handler, so you can either pass term
 * ids as source types, or term names, which can be auto-created if they do not
 * exist yet.
 *
 * @code
 *   $arguments = array('level' => array('source_field' => 'term_level'));
 *   $this->addFieldMapping('field_term_level', 'term name')
 *        ->arguments($arguments);
 * @endcode
 */
class MigrateTermLevelFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('term_level'));
  }

  public function fields($type) {
    return array(
      'level' => t("Subfield: The term's level"),
    );
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }

    $tids = array();
    if (isset($arguments['source_type']) && $arguments['source_type'] == 'tid') {
      // Nothing to do. We have tids already.
      $tids = $values;
    }
    elseif ($values) {
      $voc = taxonomy_vocabulary_machine_name_load($field_info['settings']['voc']);

      // Cannot use taxonomy_term_load_multiple() since we have an array of names.
      // It wants a singular value. This query may return case-insensitive
      // matches.
      $existing_terms = db_select('taxonomy_term_data', 'td')
        ->fields('td', array('tid', 'name'))
        ->condition('td.name', $values, 'IN')
        ->condition('td.vid', $voc->vid)
        ->execute()
        ->fetchAllKeyed(1, 0);
      // If we're ignoring case, change both the matched term name keys and the
      // source values to lowercase.
      if (isset($arguments['ignore_case']) && $arguments['ignore_case']) {
        $existing_terms = array_change_key_case($existing_terms);
        $values = array_map('strtolower', $values);
      }
      foreach ($values as $value) {
        if (isset($existing_terms[$value])) {
          $tids[] = $existing_terms[$value];
        }
        elseif (!empty($arguments['create_term'])) {
          $new_term = new stdClass();
          $new_term->vid = $voc->vid;
          $new_term->name = $value;
          taxonomy_term_save($new_term);
          $tids[] = $new_term->tid;
          // Add newly created term to existing array.
          $existing_terms[$value] = $new_term->tid;
        }
        else {
          // No term is found for the source value and none is set to be
          // created: warn that data has not been imported.
          $migration = Migration::currentMigration();
          $migration->saveMessage(t("No matching taxonomy term found for source value '@value' in vocabulary %vocab.", array(
            '@value' => $value,
            '%vocab' => $voc->name,
          )), MigrationBase::MESSAGE_INFORMATIONAL);
        }
      }
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    // Setup the standard Field API array for saving.
    $delta = 0;
    foreach ($tids as $tid) {
      $return[$language][$delta] = array('tid' => $tid);
      if (isset($arguments['level'])) {
        if (is_array($arguments['level'])) {
          $return[$language][$delta]['level'] = $arguments['level'][$delta];
        }
        else {
          $return[$language][$delta]['level'] = $arguments['level'];
        }
      }
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
