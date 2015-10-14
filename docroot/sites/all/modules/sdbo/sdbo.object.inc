<?php
/**
 * @file
 *   SDB object
 */

/**
 * Generic class that provides a unified way to handle persisted objects.
 * To use this class an object class needs to implement all abstract
 * methods and define public attributes that correspond with table fields.
 */
abstract class SDBObject implements SDBObjectInterface {

  /**
   * Stands for unique or primary key for most persisted objects.
   *
   * @var int
   */
  public $id;

  private $persisted = FALSE;

  protected $rels = array();

  /**
   * @throws SDBException
   *   In case the getPrimaryKey() implementation does not return
   *   a string or empty value.
   */
  function __construct() {
    $pk = $this->getPrimaryKey();

    if (!is_string($pk) || sizeof($pk) == 0) {
      throw new SDBException('Primary key is not defined or is not a string.');
    }
  }

  /**
   * Implementation should return its unique key definition.
   * Default implementation assumes "id".
   *
   * @return array
   *    In case of combined unique key an array must be returned.
   *    In case only one attribute represents a unique key a
   *    string may be returned.
   */
  function getUniqueKey() {
    return 'id';
  }

  /**
   * It should implement the logic that from provided data
   * initializes unique key instance attributes. In case of separate
   * implementations of getUniqueKey() and getPrimaryKey() the logic
   * must be able to init instance attributes for both methods.
   *
   * @param array $init_data
   * @return void
   */
  function initUniqueKeyValues($init_data) {
    $unique_key = $this->getUniqueKey();

    if (!empty($init_data[$unique_key])) {
      $this->$unique_key = $init_data[$unique_key];
    }
  }

  /**
  * Called before insert/update query is triggered.
  */
  function beforeSave() {

  }

  /**
   * Called after insert/update query is triggered.
   */
  function afterSave() {

  }

  /**
   * Called after object is loaded from db or form provided resource.
   */
  function afterLoad() {

  }

  /**
   * Called after delete query is triggered.
   *
   * @param int $res
   *    Number of affected rows resulted by delete operation.
   */
  function afterDelete($res) {

  }

  /**
   * Interface to operations performed on rel objects of current instance.
   *
   * @param string $class
   * @return SDBRelInterface
   */
  function rel($class) {

    if (!$this->isPersisted()) {
      throw new SDBException('REL operations on non persisted objects are not permitted');
    }

    $rel_def = $this->getRelDefinition();

    if (!isset($rel_def[$class])) {
      throw new SDBException('SDB implementation does not have REL object for class ' . $class);
    }

    if (isset($this->rels[$class])) {
      return $this->rels[$class];
    }

    $type = SDBRel::getType($class, $rel_def);

    if ($type == '1_to_n') {
      $this->rels[$class] = new SDBRel1toN($this, $class);
    }
    elseif ($type == 'n_to_n') {
      $this->rels[$class] = new SDBRelNtoN($this, $class);
    }
    else {
      throw new SDBException('Unable to determine REL type');
    }

    $this->rels[$class]->setDefinition($rel_def);

    return $this->rels[$class];
  }

  /**
   * Provides REL definition based on which REL objects
   * are handled. Example arrays:
   * 1 to 1-n
   *    array(
   *      'SDBRELClass' => array(
   *        'rel_type' => '1_to_n',
   *        'foreign_key' => 'rel_id',
   *        'del_policy' => 'keep|restrict|cascade'
   *       ),
   *    );
   *
   *  n to n
   *    array(
   *      'SDBRELClass' => array(
   *        'rel_type' => 'n_to_n',
   *        'mapping_table' => 'table',
   *        'my_key' => 'my_key',
   *        'foreign_key' => 'rel_id',
   *        'del_policy' => 'keep|restrict|cascade'
   *       ),
   *    );
   *
   * @return array
   */
  function getRelDefinition() {

    /* 1 to n REL definition.
    return array(
      'SDBRELClass' => array(
        'rel_type' => '1_to_n',
        'foreign_key' => 'rel_id',
        'del_policy' => 'keep|restrict|cascade'
      ),
    );
    */

    /* n to n REL definition.
    return array(
      'SDBRELClass' => array(
        'rel_type' => 'n_to_n',
        'mapping_table' => 'table',
        'my_key' => 'my_key',
        'foreign_key' => 'rel_id',
        'del_policy' => 'keep|restrict|cascade'
      ),
    );
    */

    return FALSE;
  }

  /**
   * Provides default method an object is printed for debug purposes.
   *
   * @return string
   */
  function toString() {
    $str = '';

    foreach ($this->getUniqueKey() as $key => $val) {
      $str .= '__' . $key . '_' . $val;
    }

    return $str;
  }

  /**
   * Function that defines primary key based on which the update queries
   * will be built. It internally calls getUniqueKey() as these two are in
   * most cases the same. However, sometimes for more flexibility you
   * need to define separate getUniqueKey() and getPrimaryKey().
   *
   * @throws SDBException
   *   In case the internal logic is not able to determine a key that
   *   is a string type.
   * @return string
   */
  function getPrimaryKey() {
    $unique_key = $this->getUniqueKey();
    if (is_array($unique_key) && count($unique_key) == 1) {
      return array_shift($unique_key);
    }
    elseif (is_string($unique_key)) {
      return $unique_key;
    }
    else {
      throw new SDBException('Default implementation of getPrimaryKey() is not going to return string for primary key: ' . var_export($unique_key, TRUE));
    }
  }

  /**
   * Gets primary key value.
   *
   * @return string
   *   Primary key value.
   */
  final function getPrimaryKeyValue() {
    $primary_key = $this->getPrimaryKey();
    return $this->{$primary_key};
  }

  /**
   * Loads object from db. Instead of calling load() on sdb object
   * it is recommended to use the SDBFactory::loadInstanceOf().
   *
   * @return boolean
   * 	True if load form db was successful.
   */
  function load() {

    $res = $this->getResource();

    if (!empty($res)) {
      $this->init($res, TRUE);
    }

    return $this->persisted;
  }

  /**
   * Provides info whether the object is persisted in database or not.
   *
   * @return boolean
   */
  function isPersisted() {
    return $this->persisted;
  }

  /**
   * Setter to be used in case when the object itself cannot determine
   * if it is persisted or not.
   */
  function setPersisted() {
    $this->persisted = TRUE;
  }

  /**
   * Inits instance from provided resource.
   *
   * @param object|array $res
   * @param boolean $persisted
   * 		Set this flag to FALSE if such object does not exists in database.
   *    To set this flag right is a vital on db updates and inserts as this
   *    flag determines if a new entry is going to be inserted or an old
   *    one updated.
   */
  function init($res, $persisted = FALSE) {

    if (is_array($res)) {
      $res = (object) $res;
    }

    foreach ($this as $key => $value) {
      if (isset($res->$key)) {
        $this->$key = $res->$key;
      }
    }
    $this->persisted = $persisted;

    $this->afterLoad();
  }

  /**
   * Loads object resource from database. Use only if you want to
   * get raw data. Otherwise use load() method so that the instance
   * gets instantiated with values from database.
   *
   * @return object
   */
  function getResource() {
    $unique_key = $this->getUniqueKey();
    $table = $this->getTable();

    if (is_string($unique_key)) {
      $unique_key = array($unique_key);
    }

    // Push in the unique key as that is the most used key for search.
    if (!in_array($this->getPrimaryKey(), $unique_key)) {
      array_push($unique_key, $this->getPrimaryKey());
    }

    $select = db_select($table)->fields($table);

    $condition_set = FALSE;

    foreach ($unique_key as $key) {
      if (isset($this->{$key})) {
        $select->condition($key, $this->{$key});
        $condition_set = TRUE;
      }
    }

    if (!$condition_set) {
      return NULL;
    }

    return $select->execute()->fetchObject();
  }

  /**
   * Saves the object into db. Internally it uses drupal_write_record()
   * so the logic is dependant on the drupal schema.
   *
   * @throws PDOException
   * @return SDBObject
   */
  function save() {

    $primary_key = $this->getPrimaryKey();
    $table = $this->getTable();

    $this->beforeSave();

    if ($this->persisted) {
      drupal_write_record($table, $this, $primary_key);
    }
    else {

      $res = drupal_write_record($table, $this);

      if ($res == SAVED_NEW) {
        $this->persisted = TRUE;
      }
    }

    $this->afterSave();
    return $this;
  }

  /**
  * Deletes object from database
  *
  * @return int
  * 	Number of affected rows, or boolean false on error
  * @throws PDOException
  */
  function delete() {
    $res = 0;

    $delete = db_delete($this->getTable());
    $delete->condition($this->getPrimaryKey(), $this->getPrimaryKeyValue());

    // Delete REL objects before deleting this instance so that in case of error
    // or restrict policy we do not execute delete query for current instance.
    $this->delRelObjects();

    $res = $delete->execute();

    $this->afterDelete($res);


    if ($res) {
      // Remove instance from registry in case it is present there.
      $object_key = array($this->getPrimaryKey() => $this->getPrimaryKeyValue());
      SDBFactory::removeInstanceFromRegistry($this, $object_key);

      // Set persisted to false.
      $this->persisted = FALSE;
    }

    return $res;
  }

  /**
   * Helper method to deal with deletion of REL objects on deletion of current instance.
   *
   * @throws SDBException
   * @return void
   */
  private function delRelObjects() {
    $rel_definition = $this->getRelDefinition();
    $has_restrict_rel_objects = FALSE;

    // Find if we have rel definition, if not, nothing to do.
    if ($rel_definition != FALSE) {
      $rel_classes = array_keys($rel_definition);

      foreach ($rel_classes as $rel_class) {

        $rel = $this->rel($rel_class);
        $del_policy = $rel->getDelPolicy();

        // Restrict policy - set the has restrict flag, so other REL objects with cascade policy will get removed
        // and throw exception after the deletion cycle.
        if ($del_policy == 'restrict') {
          if (count($this->rel($rel_class)->getAll(NULL, NULL, TRUE)) > 0) {
            $has_restrict_rel_objects = TRUE;
          }
        }
        // Cascade policy - delete all objects
        elseif ($del_policy == 'cascade') {
          $this->rel($rel_class)->delAll();
        }
        // Keep policy - do not remove child objects.
        elseif ($del_policy == 'keep') {
          // In case of n to n relation we remove mappings.
          if (is_a($rel, 'SDBRelNtoN')) {
            $rel->delMappings();
          }
          continue;
        }
        // Unknown policy.
        else {
          throw new SDBException('Invalid delete policy provided: ' . $del_policy);
        }
      }

      if ($has_restrict_rel_objects) {
        throw new SDBException('Cannot delete instance having REL objects with restrict delete policy');
      }
    }
  }
}

/**
 * SDB Exception class.
 */
class SDBException extends PDOException {
  function __construct($message, $code = NULL, $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }
}
