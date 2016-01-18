<?php

/**
 * @file
 * Gardener class implementation file
 */

/**
 * Gardener class implementation
 */
class Gardener {
  /**
   * Error code INVALID_ID
   */
  const INVALID_ID = 1;

  /**
   * Numeric unique ID
   */
  private $id;

  /**
   * Array with the settings for the gardner
   */
  private $settings;

  /**
   * Name of the gardener
   */
  public $name;

  /**
   * Destination
   *
   * Destination object as defined in backup_migrate
   */
  private $destination;

  /**
   * Created timestamp
   * 
   * @throws ErrorException
   */
  private $created;
  
  /**
   * Time reference
   * 
   * The date relative dates are calculated against. Use common PHP formats
   * @see http://php.net/manual/es/datetime.formats.compound.php
   */
  private $date_reference;

  function __construct($gardener_id = NULL) {
    if (!is_null($gardener_id)) {
      $this->load($gardener_id);
      $this->date_reference = 'now';
    }
  }

  /**
   * Sets the Gardener::date_reference property.
   * 
   * @param $datetime_string
   *   String containing the date to be set
   */
  public function setDateReference($datetime_string) {
    $this->date_reference = $datetime_string;
  }

  /**
   * Load the settings of a gardener and return an instance of it.
   *
   * @throws ErrorException
   */
  public function load($gardener_id) {
    backup_migrate_include('destinations');
    if (!is_numeric($gardener_id)) {
      throw new ErrorException(t('Cannot load a gardener with an invalid ID.'), \GARDENER::INVALID_ID, WATCHDOG_ERROR);
    }
    // Initialize settings & data
    $this->id = $gardener_id;
    $fields = db_select('backup_migrate_gardener', 'bmg')
      ->fields('bmg')
      ->condition('bmg.gardener_id', $gardener_id)
      ->execute()
      ->fetch();
    if (!$fields) {
      throw new ErrorException(t('ID not foud to load gardener.'), \GARDENER::INVALID_ID, WATCHDOG_ERROR);
    }
    $this->setSettings(unserialize($fields->settings));
    $this->setDestination(backup_migrate_get_destination($fields->destination_id));
    $this->setName($fields->name);
    $this->setCreated($fields->created);
  }

  /**
   * Persist the settings for this gardener
   */
  public function save() {
    $record = array(
      'name' => $this->getName(),
      'settings' => $this->getSettings(),
      'created' => $this->getCreated(),
      'destination_id' => $this->getDestination()->get_id(),
    );
    if ($this->isNew()) {
      // Saving a newly created gardener
      drupal_write_record('backup_migrate_gardener', $record);
    }
    else {
      // This means updating the Gardener
      $record['gardener_id'] = $this->getId();
      drupal_write_record('backup_migrate_gardener', $record, 'gardener_id');
    }
  }
  
  /**
   * Delete the gardener from the persistent storage
   */
  public function delete() {
    $deleted = db_delete('backup_migrate_gardener')
      ->condition('gardener_id', $this->getId())
      ->execute();
  	if ($deleted) {
  		drupal_set_message(t('Your gardener has been deleted successfully.'));
  	}
  	else {
  		drupal_set_message(t('There was an error deleting your gardener'), 'error');
  	}
  }

  /**
   * Delete the files that match the criteria
   * 
   * @throws ErrorException
   * 
   * @return int
   *   The number of deleted backups
   */
  public function prune() {
    $destination = $this->getDestination();
    if (!$destination->op('delete')) {
  		throw new ErrorException('This destination cannot be prunned because deleting is not supported.', WATCHDOG_ERROR);
  	}
    $files = $this->arrangeFiles();
    $deleted = 0;
    foreach ($files as $slot => $fileset) {
      // Foreach outer_category we need to prune and select a month
      foreach ($fileset as $outer_category => $inner_fileset) {
        $deleted += $this->processPrune($inner_fileset, $slot);
      }
    }
    return $deleted;
  }

  /**
   * Organizes the files in the time slots according to the timestamp
   * 
   * @return
   *   Keyed array containing backup_file object keyed by the time slot
   */
  public function arrangeFiles() {
    $files = array(
      'thisweek_slot' => array(),
      'thismonth_slot' => array(),
      'thisyear_slot' => array(),
      'pastyears_slot' => array(),
    );
    // These files should not be taken into account
    $protected_filenames = array(
      '.',
      '..',
      '.htaccess',
      'test.txt',
    );
    // Instead of using 'now' an arbitrary reference is used in order to allow
    // a much comprehensive testing
    $reference = new \DateTime($this->date_reference);
    foreach ($this->getDestination()->list_files() as $file) {
      if (!in_array($file->filename(), $protected_filenames)) {
        $filedate = new \DateTime();
        $filedate->setTimestamp($file->info('filetime'));
        $timelapse = $filedate->diff($reference);
        $week_num = (int)($filedate->format('j') / 7) + 1;
        if ($timelapse->days > 0) {
          if ($timelapse->days < 7) {
            if (!isset($files['thisweek_slot']['day-' . $timelapse->d])) {
              $files['thisweek_slot']['day-' . $timelapse->d] = array();
            }
            $files['thisweek_slot']['day-' . $timelapse->d][$filedate->format('G')][] = $file;
          }
          elseif ($timelapse->m < 1 && $timelapse->y < 1) {
            if (!isset($files['thismonth_slot']['week-' . $week_num])) {
              $files['thismonth_slot']['week-' . $week_num] = array();
            }
            $files['thismonth_slot']['week-' . $week_num][$filedate->format('N')][] = $file;
          }
          elseif ($timelapse->y < 1) {
            if (!isset($files['thisyear_slot']['month-' . $timelapse->m])) {
              $files['thisyear_slot']['month-' . $timelapse->m] = array();
            }
            $files['thisyear_slot']['month-' . $timelapse->m][$week_num][] = $file;
          }
          else {
            if (!isset($files['pastyears_slot']['year-' . $timelapse->y])) {
              $files['pastyears_slot']['year-' . $timelapse->y] = array();
            }
            $files['pastyears_slot']['year-' . $timelapse->y][$filedate->format('n')][] = $file;
          }
        }
      }
    }
    return $files;
  }

  /**
   * Tests if the file is elegible to be prunned and deletes it if necessary
   * 
   * @param $fileset
   *   File object to be tested an deleted
   * @param $slot
   *   The time slot being processed
   * 
   * @return
   *   The total number of deleted backups
   */
  private function processPrune($fileset, $slot) {
    $settings = $this->getSettings();
    $prunned_files = 0;
    $destination = $this->getDestination();
    // If there are settings for this time slot AND is active AND there are more
    // than one file.
    
    // We have a list of all files in $fileset we are going to:
    //   1. Remove the files from $fileset we want to save
    //   2. Delete all remaining files
    if (isset($settings[$slot]) && $settings[$slot]['active']) {
      $category_value = NULL;
      // This gardener has to prune these backups
      if (isset($settings[$slot]['keep'])) {
        // Keep the last backup for the indicated value
        $category_value = empty($fileset[$settings[$slot]['keep']]) ? NULL : $settings[$slot]['keep'];
      }
      // If there is no selected value to keep or the selected value has no
      // backups then select any value with backups
      if (is_null($category_value)) {
        // Keep anyone and extract the last backup from the last
        // category (Monday, January, 3rd week, …)
        foreach ($fileset as $category_value => $files) {
          // Iterate over all category_values and leave $category_value to the
          // first one that has files in it
          if (!empty($files)) {
            break;
          }
        }
      }
      // Remove the last value from the selected category_value to prevent
      // being erased. If there is no backups in the selected category_value
      // preserve any backup.
      if (!empty($fileset[$category_value])) {
        $fileset[$category_value] = array_slice($fileset[$category_value], 0, -1);
      }
      // Iterate over all remaining files to delete them.
      foreach ($fileset as $category_value => $files) {
        foreach ($files as $file) {
          // Delete the file
          $destination->delete_file($file->file_id());
					$prunned_files++;
        }
      }
    }
    return $prunned_files;
  }

  /**
   * Returns the settings for the gardener
   *
   * @return
   *   Array containig the settings
   */
  public function getSettings() {
    // Get cached settings
    if (empty($this->settings)) {
      $settings = db_select('backup_migrate_gardener', 'bmg')
        ->fields('bmg', array('settings'))
        ->condition('bmg.gardener_id', $this->getId())
        ->execute()
        ->fetchField();
      if (isset($settings)) {
        $this->setSettings(unserialize($settings));
      }
    }
    return $this->settings;
  }

  /**
   * Set the settings for a gardener
   *
   * @param $settings
   *   Array containing the settings to be written
   */
  public function setSettings($settings = array()) {
    $this->settings = $settings;
  }

  /**
   * Get all gardeners associated with the destination of the
   * current gardener
   *
   * @param $exclude
   *   Exclude the current gardener
   */
  public function getCoworkers($exclude = FALSE) {
    $destination_id = $this->destination->get_id();
    $results = db_select('backup_migrate_gardener', 'bmg')
      ->fields('bmg', array('gardener_id'))
      ->condition('bmg.destination_id', $destination_id)
      ->execute();
    foreach ($results as $result) {
      if (!$exclude || $result->gardener_id != $this->getId()) {
        $coworkers[$result->gardener_id] = new Gardener($result->gardener_id);
      }
    }
    return $coworkers;
  }

  /**
   * Get the destination for the gardener
   *
   * @return
   *   The destination object associated with this gardener
   */
  public function getDestination() {
    // Get cached settings
    if (empty($this->destination)) {
      // Load the record from the database
      $destination_id = db_select('backup_migrate_gardener', 'bmg')
        ->fields('bmg', array('destination_id'))
        ->condition('bmg.gardener_id', $this->getId())
        ->execute()
        ->fetchField();
      if (isset($destination_id) && is_numeric($destination_id)) {
        $this->setDestination(backup_migrate_get_destination($destination_id));
      }
    }
    return $this->destination;
  }

  /**
   * Set the destination for the gardener
   *
   * @param $destination
   *   The destination object associated with this gardener
   */
  public function setDestination($destination = NULL) {
    $this->destination = $destination;
  }

  /**
   * Get the name for the gardener
   *
   * @return
   *   The name object associated with this gardener
   */
  public function getName() {
    // Get cached settings
    if (empty($this->name)) {
      // Load the record from the database
      $name = db_select('backup_migrate_gardener', 'bmg')
        ->fields('bmg', array('name'))
        ->condition('bmg.gardener_id', $this->getId())
        ->execute()
        ->fetchField();
      if (isset($name)) {
        $this->setName($name);
      }
    }
    return $this->name;
  }

  /**
   * Set the name for the gardener
   *
   * @param $name
   *   The name associated with this gardener
   */
  public function setName($name = '') {
    $this->name = $name;
  }

  /**
   * Get the creation date for the gardener
   *
   * @return
   *   The name object associated with this gardener
   */
  public function getCreated() {
    // Get cached settings
    if (empty($this->created)) {
      // Load the record from the database
      $created = db_select('backup_migrate_gardener', 'bmg')
        ->fields('bmg', array('created'))
        ->condition('bmg.gardener_id', $this->getId())
        ->execute()
        ->fetchField();
      if (isset($created)) {
        $this->setCreated($created);
      }
    }
    return $this->created;
  }

  /**
   * Set the creation date for the gardener
   *
   * @param $created
   *   The created date associated with this gardener
   */
  public function setCreated($created = '') {
    $this->created = $created;
  }

  /**
   * Get the id for the current gardener
   *
   * @return
   *   The numeric id
   */
  public function getId() {
    return $this->id;
  }
  
  /**
   * This method tells if the gardener is new and has not
   * yet been persisted in the database
   * 
   * @return
   *   Boolean indicating if the gardener is new.
   */
  protected function isNew() {
    return !is_numeric($this->getId());
  }
}
