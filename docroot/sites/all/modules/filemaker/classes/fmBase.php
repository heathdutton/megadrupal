<?php

/**
 * @file
 * Base class for all classes in the FileMaker module.
 */

abstract class FmBase {

  /**
   * True if object is confirmed to work with FileMaker.
   *
   * @var bool $is_valid_in_filemaker
   */
  public $is_valid_in_filemaker = NULL;

  /**
   * True if object has a record in the Drupal database matching the primary key.
   *
   * @var bool $is_valid_in_drupal.
   */
  public $is_valid_in_drupal = NULL;

  /**
   * An array of strings. Each string contains an error message, if there 
   *  were errors. If all is well, this will be an empty array.
   *
   * @var array $errors
   */
  public $errors = array();

/**********************************************************************************
 * The contract. Gotta obey the rules, yo.
 *********************************************************************************/

  /**
   * Silly PHP doesn't allow us to declare abstract consts, but this class assumes,
   *  meaning will not work, without the following consts in any non-abstract child class:
   *
   *  const ID_FIELD_NAME;
   *  const TABLE_NAME;
   *
   *  EXAMPLES:
   *  const ID_FIELD_NAME = 'fmcid';
   *  const TABLE_NAME = 'filemaker_connection';
   */

/**********************************************************************************
 * Public functions.
 *********************************************************************************/


  public function __construct($id = 0) {
    if ($id) {
      $this->build($id);
    }
  }

  /**
   * Builds an object that inherits this class, grabbing data from Drupal DB.
   */
  public function build($id) {

    $class = get_class($this);
    $this->set_id($id);

    $query = db_select($class::TABLE_NAME, 't');
    $query
      ->condition($class::ID_FIELD_NAME, $this->id())
      ->fields('t')
      ->range(0, 1);

    $result = $query->execute();
    $records = $result->fetchAllAssoc($class::ID_FIELD_NAME);
    
    if ( ! $records) {

      $this->errors[] = t('No record found in @table_name for @id_field_name: @id', array(
        '@table_name' => $class::TABLE_NAME, 
        '@id_field_name' => $class::ID_FIELD_NAME, 
        '@id' => $this->id(),
      ));

      $this->is_valid_in_drupal = FALSE;
      return FALSE;
    }

    else {
      $this->set_values($records[$this->id()]);
      $this->is_valid_in_drupal = TRUE;
      return TRUE;
    }
  }

  /**
   * Returns the id of an object based on this class (we don't know the name of the id field).
   */
  public function id() {
    $class = get_class($this);

    if (isset($this->{$class::ID_FIELD_NAME})) {
      return $this->{$class::ID_FIELD_NAME};
    }
    return false;
  }

  /**
   * Persists object: either inserts or updates.
   */
  public function save() {
    
    $class = get_class($this);
    
    // Update?
    if ($this->id() > 0) {
      return drupal_write_record($class::TABLE_NAME, $this, $class::ID_FIELD_NAME);
    }
    
    // Insert.
    else {
      return drupal_write_record($class::TABLE_NAME, $this);
    }
    
  }

  /**
   * Deletes a single record from the Drupal DB.
   */
  public function delete() {

    $class = get_class($this);
    
    $sql = "DELETE FROM " . $class::TABLE_NAME . " WHERE " . $class::ID_FIELD_NAME . " = :id";
    $result = db_query($sql, array(':id' => $this->{$class::ID_FIELD_NAME})); 
  }

  /**
   * Returns all of the records from the table of the class that is based on this.
   */
  public function get_all() {
    $records = $this->select_all();
    return $this->build_multiple($records);
  }

  /**
   * Creates fully formed objects from database results.
   */
  public function build_multiple(array $db_results) {

    $class = get_class($this);

    $ret = array();
    foreach ($db_results as $id => $std_object) {
      $ret[$id] = new $class(); 
      $ret[$id]->set_values($std_object);
      $ret[$id]->is_valid_in_drupal = true;
    }

    return $ret;
  }

/**********************************************************************************
 * Protected functions.
 *********************************************************************************/

  /**
   * Helper function for $this->get_all(). Does the db work.
   */
  protected function select_all() {

    $class = get_class($this);

    $query = db_select($class::TABLE_NAME, 't');
    $query->fields('t');
    $result = $query->execute();

    return $result->fetchAllAssoc($class::ID_FIELD_NAME);
  }

  /**
   * Sets the id field (which we don't know the name of).
   */
  protected function set_id($id) {
    $class = get_class($this);
    $this->{$class::ID_FIELD_NAME} = $id;
  }

}
