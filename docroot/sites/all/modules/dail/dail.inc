<?php

/**
 * @file: dail.inc
 *
 * The base AI class.
 * TODO: Make this useful. ;)
 */

class dail {

  /**
   * A logging object for this dail instance.
   * TODO:
   */
  protected $logger = array();

  function __construct($collection = 'default') {
    $this->type = 'dail';
    $this->collection = $collection;
  }

  function collection() {
    return $this->collection;
  }

  /**
   * Parameters:
   *  library: (required) name of the library saving state
   *  key:     (optional) unique key for identifying state
   *  version: (optional) int version id of state
   *  state:   (optional) the object or array to save
   */
  function save_state($parameters = array()) {
    $error = FALSE;
    // Library
    if (!isset($parameters['library'])) {
      drupal_set_message(t('Failed to save state, no library defined.'), 'error');
      $error = TRUE;
    }
    else {
      $library = $parameters['library'];
    }
    // Key
    if (!isset($parameters['key'])) {
      // Default key
      $key = t('none');
    }
    else {
      $key = $parameters['key'];
    }
    // Version
    if (!isset($parameters['version'])) {
      // Not required
      $version = 0;
    }
    else if (is_int($parameters['version'])) {
      $version = $parameters['version'];
    }
    else {
      drupal_set_message(t('Failed to save state, version %version is not an integer.', array('%version' => $version)), 'error');
    }
    // State
    $state = array();
    $state['state'] = isset($parameters['state']) ? $parameters['state'] : '';
    $state['timestamp'] = time();

    if (!$error) {
      $query = db_update('dail_states')
        ->fields(array('state' => serialize($state)))
        ->condition('library', $library)
        ->condition('state_key', $key)
        ->condition('state_version', intval($version));
      if (!$query->execute()) {
        // TODO: Why is the update failing when there's data in the db?
        $result = db_select('dail_states')
        ->fields('dail_states', array('sid'))
        ->condition('library', $library)
        ->condition('state_key', $key)
        ->condition('state_version', intval($version))
        ->execute();
        foreach ($result as $row) {
          if (isset($row->sid) && $row->sid) {
            //drupal_set_message(t('Failed update, ignored.'));
            return;
          }
        }
        // First time we've saved this state, insert it.
        db_insert('dail_states')
          ->fields(array(
            'library' => $library,
            'state_key' => $key,
            'state_version' => intval($version),
            'state' => serialize($state)))
          ->execute();
      }
    }
  }

  /**
   * Parameters:
   *  library: (required) name of the library saving state
   *  key:     (optional) unique key for identifying state
   *  version: (optional) int version id of state
   */
  function load_state($parameters = array()) {
    $error = FALSE;
    // Library
    if (!isset($parameters['library'])) {
      drupal_set_message(t('Failed to load state, no library defined.'), 'error');
      $error = TRUE;
    }
    else {
      $library = $parameters['library'];
    }
    // Key
    if (!isset($parameters['key'])) {
      // Default key
      $key = t('none');
    }
    else {
      $key = $parameters['key'];
    }
    // Version
    if (!isset($parameters['version'])) {
      // Not required
      $version = 0;
    }
    else if (is_int($parameters['version'])) {
      $version = $parameters['version'];
    }
    else {
      drupal_set_message(t('Failed to load state, version %version is not an integer.', array('%version' => $version)), 'error');
    }

    if (!$error) {
      $states = db_select('dail_states')
        ->fields('dail_states', array('state'))
        ->condition('library', $library)
        ->condition('state_key', $key)
        ->condition('state_version', intval($version))
        ->execute();
      if (!empty($states)) {
        // TODO: Properly error check, return this.
        foreach ($states as $state) {
          $state = unserialize($state->state);
          return $state['state'];
        }
      }
    }
  }
}
