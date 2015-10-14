<?php
/**
 * @file
 *   SDB factory operations.
 */

define('SDB_REGISTRY', 'sdb_registry');

/**
 * Factory utility to instantiate implementations of SDBObject.
 */
class SDBFactory {

  /**
   * Inits instance from provided values. This may be used to init
   * instance from array of values (i.e. from submitted form) or to
   * init persisted objects from a query. For the later it is required
   * to set $persisted flag to TRUE so that data operations are done
   * as if persisted.
   *
   * @static
   * @throws SDBException
   * 	 If provided class does not exists.
   * 	 If the class is not an implementation of SDBObject.
   * @param $class
   * 	 Class that will be instantiated and loaded with $object_vals.
   * @param object|array $object_vals
   * 	 Data that will be used to load the instance attributes.
   * @param array $obj_unique_key
   * 	 key => value pairs. In this case this data is used only for
   * 	 storing the object in registry, so it does not need to correspond
   *   with db unique key.
   * @param boolean $persisted
   *   Determines if the object is persisted. This flag is used on save action
   *   when the logic decides to either insert or update a db row.
   * @return SDBObject
   */
  static function initInstanceOf($class, $object_vals, $obj_unique_key = NULL, $persisted = FALSE) {

    if (!class_exists($class)) {
      throw new SDBException('Class ' . $class . ' does not exist.');
    }

    /**
     * @var SDBObject
     */
    $object = new $class();
    if (!is_a($object, 'SDBObject')) {
      throw new SDBException('Instantiated object ' . $class . ' is not of type SDBObject.');
    }

    $object->init($object_vals, $persisted);

    // Unique key is empty, therefore we just return instantiated object.
    if (empty($object_unique_key)) {
      return $object;
    }

    // We have unique key, so we can store the object in the registry.
    self::setInstanceIntoRegistry($object, $obj_unique_key);

    return $object;
  }

  /**
   * Loads multiple instances from provided resource. Internaly it uses
   * initInstanceOf() without use of unique key. That means the initied
   * objects will not be cached in the internal object registry.
   *
   * @static
   * @param $class
   * @param array $resource
   * @param boolean $persisted
   *
   * @return array - associative array keyed as resource is provided.
   * @throws SDBException
   */
  static function initInstancesOf($class, $resource, $persisted = TRUE) {

    if (!class_exists($class)) {
      throw new SDBException('Class ' . $class . ' does not exist.');
    }

    if (!is_array($resource)) {
      throw new SDBException('Provided invalid format of resource, array expected.');
    }

    $data = array();

    foreach ($resource as $key => $object_vals) {
      $data[$key] = self::initInstanceOf($class, $object_vals, NULL, $persisted);
    }

    return $data;
  }

  /**
   * Loads instance from registry based on provided unique key. This method
   * is mostly for internal purpose to pull cached objects from registry, however
   * you can use it to do a look-up into the registry. In case you want a regular
   * load operation from db or from registry use loadInstanceOf().
   *
   * @param $class
   * @param array $obj_unique_key
   *
   * @return SDBObject
   */
  static function loadInstanceFromRegistry($class, $obj_unique_key) {
    $registry = &drupal_static(SDB_REGISTRY, array());

    $key = self::getKey($class, $obj_unique_key);

    if (isset($registry[$key])) {
      return $registry[$key];
    }

    return NULL;
  }

  /**
   * Sets objects into registry.
   *
   * @static
   * @param SDBObject $object
   * @param array|string $obj_unique_key
   */
  static function setInstanceIntoRegistry($object, $obj_unique_key) {
    $registry = &drupal_static(SDB_REGISTRY, array());
    $key = self::getKey(get_class($object), $obj_unique_key);
    $registry[$key] = $object;
  }

  /**
   * Removes instance from registry.
   *
   * @static
   * @param SDBObject $object
   * @param array|string $obj_unique_key
   */
  static function removeInstanceFromRegistry($object, $obj_unique_key) {
    $registry = &drupal_static(SDB_REGISTRY, array());
    $key = self::getKey(get_class($object), $obj_unique_key);

    if (isset($registry[$key])) {
      unset($registry[$key]);
    }
  }

  /**
   * Loads instance from database based on unique key values. It internaly caches
   * the object using static registry, so if an object with same class and unique key
   * is requested the cached one is returned.
   *
   * @param $class
   *   Class of object that will be instantiated.
   * @param array $obj_unique_key
   * 	 Must be an associative array with key=>value pairs. These data is then used
   * 	 to build select query to load the rest of data from database.
   * @param boolean $reload
   *   Flag that identifies if a fresh load from db should be performed.
   * @return SDBObject
   * @throws SDBException
   * 	 If provided class does not exists.
   * 	 If the class is not an implementation of SDBObject.
   */
  static function loadInstanceOf($class, $obj_unique_key, $reload = FALSE) {

    if (!class_exists($class)) {
      throw new SDBException('Class ' . $class . ' does not exist.');
    }

    $object = self::loadInstanceFromRegistry($class, $obj_unique_key);

    if (!empty($object) && !$reload) {
      return $object;
    }

    $object = new $class();

    if (!is_a($object, 'SDBObject')) {
      throw new SDBException('Instantiated object ' . $class . ' is not of type SDBObject.');
    }
    $object->initUniqueKeyValues($obj_unique_key);

    $object->load();

    self::setInstanceIntoRegistry($object, $obj_unique_key);

    return $object;
  }

  /**
   * Internal key builder.
   *
   * @static
   * @param string $class
   * @param string|array $obj_unique_key
   * @return string
   */
  static private function getKey($class, $obj_unique_key) {

    if (!is_array($obj_unique_key)) {
      $obj_unique_key = array($obj_unique_key);
    }

    foreach ($obj_unique_key as $key => $val) {
      $class .= '__' . $key . '_' . $val;
    }

    return $class;
  }
}
