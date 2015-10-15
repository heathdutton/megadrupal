<?php
/**
 * @file
 * Contains Database handler.
 */

/**
 * Class Qui_Database_Handler
 */
class Qui_Database_Handler extends Qui_Query_Builder_Handler {

  public $enabled = TRUE;
  public $name = 'database';

  /**
   * Creates the dropdown based on available databases.
   */
  public function build() {
    if (count($this->getDatabases()) > 1) {
      $this->elements['query'][$this->name] = array(
        '#type' => 'select',
        '#title' => t(ucfirst($this->name)),
        '#options' => $this->getDatabases(),
        '#default_value' => $this->defaults,
        '#ajax' => $this->ajaxSettings(),
      );
    }
    else {
      $this->elements['query'][$this->name] = array(
        '#type' => 'hidden',
        '#default_value' => $this->defaults,
      );
    }
  }

  /**
   * Gets available databases.
   */
  private function getDatabases() {
    global $databases;
    $db_options = array();
    foreach ($databases as $db => $types) {
      $db_options[$db] = $db;
    }
    return $db_options;
  }

  /**
   * Set database default.
   */
  public function setDefaults() {
    $this->defaults = $this->getDefaultValues($this->name) ? : 'default';
  }

  /**
   * Save the database.
   */
  public function submit() {
    $this->report->query->database = $this->defaults;
  }

}
