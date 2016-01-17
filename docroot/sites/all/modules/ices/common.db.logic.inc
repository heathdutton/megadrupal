<?php
/**
 * @file
 * This file contains the "active record" (approximately) pattern for loading
 * and saving objects.
 */

/**
 * @defgroup ces_common_db Base de datos
 * @{
 */

/**
 * Loads and saves to db objects of class IcesDBObject. It gives a unified and
 * drupal-indevendent way for bank logic objects to be loaded and saved.
 */
class IcesSerializer {

  protected $classname;
  protected $table;
  /**
   * cache array. Srtucture:
   *  class name
   *    unique key
   *      value => record
   */
  static protected $cache = array();
  /**
   * Creates a new serializer for a specific object class.
   */
  public function __construct($classname) {
    $this->classname = $classname;
    $class = new ReflectionClass($this->classname);
    $this->table = $class->getStaticPropertyValue('table');
  }
  /**
   * Loads an object given its identifier.
   * @return IcesDBObject|boolean
   *   The loaded object on success or FALSE if object can not be loaded.
   */
  public function load($id) {
    if (!isset(IcesSerializer::$cache[$this->classname])) {
      $cache[$this->classname] = array();
    }
    if (!isset(IcesSerializer::$cache[$this->classname]['id'])) {
      $cache[$this->classname]['id'] = array();
    }
    if (!isset(IcesSerializer::$cache[$this->classname]['id'][$id])) {
      $record = db_select($this->table, 't')->fields('t')->condition('id', $id)->execute()->fetchAssoc();
      if ($record === FALSE) {
        return FALSE;
      }
      IcesSerializer::$cache[$this->classname]['id'][$id] = $this->loadFromRecord($record);
    }
    return IcesSerializer::$cache[$this->classname]['id'][$id];
  }
  /**
   * Count entries.
   *
   * @return int
   *   The number of objects that have value $value in field $key.
   */
  public function count($key, $value) {
    $query = db_select($this->table, 't')->condition($key, $value)->countQuery();
    $result = $query->execute()->fetchField();
    return $result;
  }
  /**
   * Loads a IcesDBObject from it record.
   *
   * The record may come from the database (when retrieving an existing
   * instance) or from user input (when creating a new instance).
   *
   * @param array $record
   *   An associative array with the parameters of the object.
   *
   * @return IcesDBObject
   *   A loaded object from its database record.
   */
  public function loadFromRecord($record) {
    $this->handleSerialized($record);
    if (isset($record['classname']) && class_exists($record['classname']) && is_subclass_of($record['classname'], $this->classname)) {
      $obj = new $record['classname']($record);
    }
    else {
      $obj = new $this->classname($record);
    }
    if (!empty($record['decoration']) && is_array($record['decoration'])) {
      // Load decorator pattern.
      foreach ($record['decoration'] as $value) {
        if (isset($value['classname']) && class_exists($value['classname'])) {
          $obj = new $value['classname']($obj, $value['data']);
        }
      }
    }
    return $obj;
  }
  /**
   * Saves an object to database.
   *
   * This function will create a new entry either if the object has a field
   * named is_new and set to bool TRUE or if it hasn't the id field. Otherwise
   * it will update the record matching the id field.
   */
  public function save(IcesDBObject $object) {
    $record = $object->getRecord();
    // Call save extra data to all implied objects.
    // Don't loose the main reference!
    $auxobject = $object;
    $inner = NULL;
    do {
      $auxobject->saveExtraData();
      $inner = $auxobject;
      $auxobject = $auxobject->getParent();
    } while ($auxobject != NULL && ($auxobject instanceof IcesDBObject));
    // Decide between insert and update, and automatically populate temporal
    // fields.
    $record['modified'] = REQUEST_TIME;
    if (empty($record['id']) || isset($object->is_new) && $object->is_new == TRUE) {
      // Sometimes, like in exchange import, we are creating new objects
      // with past dates.
      if (empty($record['created'])) {
        $record['created'] = REQUEST_TIME;
      }
      drupal_write_record($this->table, $record);
      // Update the inserted object with the id parameter.
      if (isset($record['id'])) {
        $inner->id = $record['id'];
      }
    }
    else {
      drupal_write_record($this->table, $record, 'id');
    }
    $this->deleteRecordFromCache($record);
  }
  /**
   * Updates a single database field from a single row.
   */
  public function updateField(IcesDBObject $object, $key) {
    $id = $object->id;
    $value = $object->{$key};
    $this->deleteCache();
    return db_update($this->table)->condition('id', $id)->fields(array($key => $value))->execute();
  }
  /**
   * Select function.
   *
   * Performs a SQL function such as COUNT(*), SUM(*), ... on a table field for
   * those rows meeting conditions.
   *
   * @param string $key
   *   The field name. Must be SQL safe.
   * @param array $conditions
   *   Each entry is a condition. A condition is an array of three therms.
   */
  public function selectFunctionField($function, $key, array $conditions) {
    $functions = array('COUNT', 'SUM');
    if (!in_array($function, $functions)) {
      throw new Exception('Invalid database function.');
    }
    $query = db_select($this->table, 't');
    $query->addExpression($function . '(' . $key . ')');
    foreach ($conditions as $condition) {
      if (count($condition) == 3) {
        $query->condition($condition[0], $condition[1], $condition[2]);
      }
      elseif (count($condition) == 2) {
        $query->condition($condition[0], $condition[1]);
      }
    }
    return $query->execute()->fetchField();
  }
  /**
   * Load many objects.
   *
   * @param array $conditions
   *   Associative array with field names as keys. There are two special
   *   conditions for those objects having the created [time] field:
   *   - createdsince: the smaller created time to return.
   *   - createduntil: the largest created time to return.
   * @param string $order_field
   *   A field to order by.
   * @param string $order_type
   *   DESC: Descending, ASC Ascending.
   *
   * @return array
   *   Objects with attribute $key = $value ordered by $order field.
   */
  public function loadCollection(array $conditions, $order_field = NULL, $order_type = 'ASC') {
    $order = ($order_field != NULL) ? $order_field : NULL;
    if (!db_table_exists($this->table)) {
      // This prevents a fatal error in case CES table is gone.
      drupal_set_message(t('WARNING: CES table @table is missing. You probably need to reinstall CES.'), array('@table' => $this->table));
      return FALSE;
    }
    $query = db_select($this->table, 't')->fields('t');
    // Handle special conditions.
    if (isset($conditions['createdsince'])) {
      $query->condition('t.created', $conditions['createdsince'], '>=');
      unset($conditions['createdsince']);
    }
    if (isset($conditions['createduntil'])) {
      $query->condition('t.created', $conditions['createduntil'], '<=');
      unset($conditions['createduntil']);
    }
    if (isset($conditions['join'])) {
      $field = $conditions['join']['field'];
      $s = new IcesSerializer($conditions['join']['class']);
      $table = $s->table;
      $condition_field = $conditions['join']['condition']['field'];
      $condition_value = $conditions['join']['condition']['value'];
      $query->join($table, 'j', 't.' . $field . ' = ' . 'j.id AND j.' .
        $condition_field . ' = :val', array(':val' => $condition_value));
      unset($conditions['join']);
    }
    if (isset($conditions['limit'])) {
      $query->range(0, $conditions['limit']);
      unset($conditions['limit']);
    }
    if (isset($conditions['or'])) {
      $orcondition = db_or();
      foreach ($conditions['or'] as $orkey => $orvalue) {
        $orcondition = $orcondition->condition('t.' . $orkey, $orvalue);
      }
      $query->condition($orcondition);
      unset($conditions['or']);
    }
    foreach ($conditions as $key => $value) {
      $query->condition('t.' . $key, $value);
    }
    if ($order != NULL) {
      $query->orderBy($order, $order_type);
    }
    $result = $query->execute();
    $loads = array();
    while (($record = $result->fetchAssoc()) !== FALSE) {
      $loads[] = $this->loadFromRecord($record);
    }
    return $loads;
  }
  /**
   * Retrieves and loads an object from a unique key.
   *
   * @return IcesDBObject
   *   The initialized instance of IcesDBObject with $key = $value, or FALSE.
   */
  public function loadFromUniqueKey($key, $value) {
    if (!isset(IcesSerializer::$cache[$this->classname])) {
      $cache[$this->classname] = array();
    }
    if (!isset(IcesSerializer::$cache[$this->classname][$key])) {
      $cache[$this->classname][$key] = array();
    }
    if (!isset(IcesSerializer::$cache[$this->classname][$key][$value])) {
      $query = db_select($this->table, 't')->fields('t')->condition($key, $value)->range(0, 1);
      $result = $query->execute();
      $record = $result->fetchAssoc();
      if ($record === FALSE) {
        return FALSE;
      }
      IcesSerializer::$cache[$this->classname][$key][$value] = $this->loadFromRecord($record);
    }
    return IcesSerializer::$cache[$this->classname][$key][$value];
  }
  /**
   * Deletes a database record.
   *
   * @param DBobject $object
   *   The object to be deleted.
   */
  public function delete(IcesDBObject $object) {
    $object->deleteExtraData();
    $this->deleteCache();
    $record = $object->getRecord();
    $query = db_delete($this->table)->condition('id', $record['id']);
    return $query->execute();
  }
  /**
   * Start database transaction.
   *
   * @return object
   *   The transaction reference.
   */
  public static function DBTransaction() {
    return db_transaction();
  }
  /**
   * Unserializes serialized database record fields.
   */
  protected function handleSerialized(array &$record) {
    // It would be nice if it was done automatically by Drupal, but...
    $serialized = array('data', 'decoration');
    foreach ($serialized as $field) {
      if (isset($record[$field]) && is_string($record[$field])) {
        $record[$field] = unserialize($record[$field]);
      }
    }
  }
  /**
   * Deletes record from static cache.
   */
  protected function deleteRecordFromCache(array $record) {
    if (isset(IcesSerializer::$cache[$this->classname])) {
      foreach (IcesSerializer::$cache[$this->classname] as $key => $value) {
        if (isset($record[$key]) && isset($value[$record[$key]])) {
          unset(IcesSerializer::$cache[$this->classname][$key][$record[$key]]);
        }
      }
    }
  }
  /**
   * Deletes whole static cache.
   */
  protected function deleteCache() {
    IcesSerializer::$cache[$this->classname] = array();
  }
}
/**
 * Special class of IcesSerializer for those objects needing transactional load
 *  and save process. It uses drupal lock framework.
 */
class IcesLockSerializer extends IcesSerializer {
  /**
   * Load many objects.
   *
   * @param array $conditions
   *   Associative array with field names as keys. There are two special 
   *   conditions for those objects having the created [time] field:
   *   - createdsince: the smaller created time to return.
   *   - createduntil: the largest created time to return.
   * @param string $order_field
   *   A field to order by.
   * @param string $order_type
   *   DESC: Descending, ASC Ascending.
   *
   * @return array
   *   Objects with attribute $key = $value ordered by $order field.
   */
  public function loadCollection(array $conditions, $order_field = NULL, $order_type = 'ASC') {
    throw new Exception('Not implemented');
  }
  /**
   * Not implemented.
   */
  public function loadFromUniqueKey($key, $value) {
    throw new Exception('Not implemented');
  }
  /**
   * Load object and acquires lock.
   */
  public function load($id) {
    $lockname = $this->table . ' ' . $id;
    $times = 0;
    while (!lock_acquire($lockname)) {
      $times++;
      if ($times > 5) {
        throw new Exception(t('Impossible to get the database lock %lock', array('%lock' => $lockname)));
      }
      lock_wait($lockname);
    }
    return parent::load($id);
  }
  /**
   * Saves object and releases lock.
   */
  public function save(IcesDBObject $object) {
    $res = parent::save($object);
    lock_release($this->table . ' ' . $object->id);
    return $res;
  }
  /**
   * Updates field and releases lock.
   */
  public function updateField(IcesDBObject $object, $key) {
    $res = parent::updateField($object, $key);
    lock_release($this->table . ' ' . $object->id);
    return $res;
  }
}
/**
 * Base class for all objects that are serializable with this framework.
 */
abstract class IcesDBObject {
  public static $table;
  /**
   * Return object record.
   *
   * @return array
   *   The database record.
   */
  public function getRecord() {
    // Handle decorator pattern.
    $object = $this;
    $parent = $object->getParent();
    $decoration = array();
    if ($parent != NULL && ($parent instanceof IcesDBObject)) {
      do {
        $decoration[] = array(
          'classname' => get_class($object),
          'data' => $object->getSimpleRecord(),
        );
        $object = $parent;
        $parent = $object->getParent();
      } while ($parent != NULL && ($parent instanceof IcesDBObject));
      array_reverse($decoration);
    }
    // Get record.
    $record = $object->getSimpleRecord();
    $record['decoration'] = $decoration;
    return $record;
  }
  /**
   * Object simple record.
   *
   * @return array
   *   The fields directly hanging from this object.
   */
  protected function getSimpleRecord() {
    $record = get_object_vars($this);
    foreach ($record as $key => $value) {
      if ($value === NULL) {
        unset($record[$key]);
      }
    }
    return $record;
  }
  /**
   * Loads a database record to this object.
   */
  protected function loadRecord($record) {
    $reflect = new ReflectionObject($this);
    foreach ($record as $key => $value) {
      if ($reflect->hasProperty($key) && $reflect->getProperty($key)->isPublic()) {
        $this->{$key} = $value;
      }
    }
  }
  /**
   * Save object data.
   *
   * If the overrider has other IcesDBObject objects as class members override
   * this function to save them.
   */
  public function saveExtraData() {
  }
  /**
   * Delete object data.
   *
   * Called when this object is to be deleted. If the overrider has other
   * IcesDBObject objects as class members override this function to delete
   * them.
   */
  public function deleteExtraData() {
  }
  /**
   * Create a new object given its database record.
   */
  public function __construct($record) {
    $this->loadRecord($record);
  }
  /**
   * The parent object for those objects using the decorator pattern.
   *
   * @return DBobject
   *   The parent object or NULL if it doesn't exist.
   */
  public function getParent() {
    return NULL;
  }
}
/**
 * A wrapper interface for Drupal transaction objects.
 */
interface IcesDBTransactionInterface {
  /**
   * Discards transaction.
   */
  public function rollback();
}

/**
 * @} End of "group ces.common.db".
 */
