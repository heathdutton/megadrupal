<?php
/**
 * @file
 *   SDBObjects interfaces.
 */

/**
 * Provides methods to handle persisted objects in a unified way.
 */
interface SDBObjectInterface {

  /**
   * Returns name of db table where instances are persisted.
   *
   * @abstract
   * @return string
   *   Table name.
   */
  function getTable();

  /**
   * Gets unique key under which the current object can be found.
   * This unique key will be used to retrieve data from db to
   * init object attributes.
   *
   * @abstract
   * @return string|array
   *   Unique key name or array defining unique key.
   */
  function getUniqueKey();

  /**
   * Implements logic that from provided data initializes unique
   * key instance attributes. In case of separate implementations of
   * getUniqueKey() and getPrimaryKey() the logic must be able to init
   * instance attributes for both methods.
   *
   * @param array $init_data
   */
  function initUniqueKeyValues($init_data);

  /**
  * Called before insert/update query is triggered.
  */
  function beforeSave();

  /**
   * Called after insert/update query is triggered.
   */
  function afterSave();

  /**
   * Called after object is loaded from db or form provided resource.
   */
  function afterLoad();

  /**
   * Called after delete query is triggered.
   *
   * @param int $res
   *    Number of affected rows resulted by delete operation.
   */
  function afterDelete($res);

  /**
   * Function that defines primary key based on which the update queries
   * will be built. It internally calls getUniqueKey() as these two are in
   * most cases the same. However, sometimes for more flexibility you
   * need to define separate getUniqueKey() and getPrimaryKey().
   *
   * @return string
   *   Primary key.
   */
  function getPrimaryKey();

  /**
   * Gets primary key value.
   *
   * @return string
   *   Primary key value.
   */
  function getPrimaryKeyValue();

  /**
  * Loads object from db.
   *
  * @return boolean
  * 	True if load form db was successful.
  */
  function load();

  /**
   * Init instance from provided resource.
   *
   * @param object $res
   * @param boolean $persisted
   * 		Set this flag to FALSE if suthech object does not exists in database.
   *    To set this flag right is a vital on db updates and inserts as this
   *    flag determines if a new entry is going to be inserted or an old
   *    one updated.
   */
  function init($res, $persisted = FALSE);

  /**
   * Provides info whether the object is persisted in database or not.
   *
   * @return boolean
   */
  function isPersisted();

  /**
   * Saves the object into db. It implements the logic to determine
   * if insert or update operation has to be performed.
   *
   * @return SDBObjectInterface
   *   Saved object.
   */
  function save();

  /**
  * Deletes current object from db.
   *
  * @return int
  * 	Number of affected rows, or boolean false on error.
  */
  function delete();

  /**
   * Interface function to handle REL objects.
   *
   * @abstract
   * @param string $class
   * @return SDBRelInterface
   */
  function rel($class);
}

/**
 * Provides methods to handle related persisted objects in a unified way.
 */
interface SDBRelInterface {


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
   * @abstract
   * @return array
   */
  function getDefinition();

  /**
   * Gets foreign key that links current object to REL object.
   *
   * @abstract
   * @return string
   *   Foreign key.
   */
  function getFK();

  /**
   * Gets key of current object.
   *
   * @abstract
   * @return string
   *   Current object key.
   */
  function getMyKey();

  /**
   * Gets delete policy for rel objects. It determines
   * how the rel objects will be handled if current
   * instance is deleted.
   *
   * @abstract
   * @return string
   *   Policy.
   */
  function getDelPolicy();

  /**
   * For N to N relation it provides mapping table of
   * current object and rel object/s.
   *
   * @abstract
   * @return string
   *   Mapping table name.
   */
  function getMappingTable();

  /**
   * Gets rel type: 1_to_n, n_to_n.
   *
   * @abstract
   * @param $class
   *   Rel object class name.
   * @param array $definition
   * @return string
   *   Rel type.
   */
  static function getType($class, array $definition);

  /**
   * Gets a list of all rel objects linked with current instance.
   *
   * @abstract
   * @param array $order_by
   *   Defines sorting of results:
   *   array('field' => 'created', 'direction' => 'ASC')
   * @param string $key_by
   *   Field which value will be used as key for the result array.
   *   If none provided the REL object primary key name will be used.
   * @param boolean $reload
   *   Flag to reload internal object cache.
   * @return array
   *   Array of SDBObjectInterface objects.
   */
  function getAll($order_by = NULL, $key_by = NULL, $reload = FALSE);

  /**
   * Gets single rel object by its key.
   *
   * @abstract
   * @param string|int $obj_key_val
   *   Object key - it has to be a string or int offset contained in
   *   the rel objects array.
   * @param bool $reload
   *   Switch to reload cache.
   * @return SDBObjectInterface
   *   Matched rel object.
   */
  function getByObjectKey($obj_key_val, $reload = FALSE);

  /**
   * Creates rel object and will link it to the current object.
   *
   * @abstract
   * @param array $data
   *   Data to create the rel object.
   * @return SDBObjectInterface
   *   Newly created and linked rel object.
   */
  function add(array $data);

  /**
   * Will delete all rel objects no matter of del policy.
   *
   * @abstract
   * @return int
   *   Number of affected db rows.
   */
  function delAll();

  /**
   * Will link existing rel object to current object.
   *
   * @abstract
   * @param SDBObjectInterface $rel_object
   *   Rel object to be linked to current object.
   * @return SDBObjectInterface
   *   Linked rel object.
   */
  function link(SDBObjectInterface $rel_object);

  /**
   * Unlinks the rel object from current objects. That is:
   *  - for 1_to_n it removes the foreign_key value
   *  - for n_to_n it removes entry in the linking table
   *
   * @abstract
   * @param SDBObjectInterface $rel_object
   *   Rel object to unlink from current object.
   * @return SDBObjectInterface
   *   Unlinked rel object.
   */
  function unlink(SDBObjectInterface $rel_object);

  /**
   * Will init Rel objects from provided data instead of
   * pulling them from db. This is helpful when creating list
   * of sdb objects which do operations on their rel objects.
   *
   * @abstract
   * @param array $init_data
   *   Associative array of object key and object itself or object data.
   */
  function initObjects(array $init_data);
}

/**
 * Abstract class that contains common operations for both
 * 1_to_n and n_to_n rel types.
 */
abstract class SDBRel implements SDBRelInterface {

  private $definition;
  protected $rel_class;
  protected $objects = array();

  /**
   * @var SDBObjectInterface
   */
  private $base_object;

  function __construct(SDBObjectInterface $base_object, $rel_class) {
    $this->base_object = $base_object;
    $this->rel_class = $rel_class;
  }

  /**
   * Gets base object - the current instance.
   *
   * @return SDBObjectInterface
   */
  function getBaseObject() {
    return $this->base_object;
  }

  /**
   * Gets rel type: 1_to_n, n_to_n.
   *
   * @param $class
   *   Rel object class name.
   * @param array $definition
   *   Rel object definition array.
   * @return string
   *   Rel type.
   */
  static function getType($class, array $definition) {
    if (!isset($definition[$class]['rel_type'])) {
      throw new SDBException('REL Definition does not contain its type.');
    }

    return $definition[$class]['rel_type'];
  }

  /**
   * Gets foreign key that links current object to REL object.
   *
   * @return string
   *   Foreign key.
   */
  function getFK() {
    $rel_definition = $this->getDefinition();

    if (empty($rel_definition)) {
      throw new SDBException('REL Definition for provided class does not exist.');
    }

    if (!isset($rel_definition[$this->rel_class]['foreign_key'])) {
      throw new SDBException('REL Definition does not contain foreign key.');
    }

    return $rel_definition[$this->rel_class]['foreign_key'];
  }

  /**
   * Gets key of current object.
   *
   * @return string
   *   Current object key.
   */
  function getMyKey() {
    $rel_definition = $this->getDefinition();

    if (empty($rel_definition)) {
      throw new SDBException('REL Definition for provided class does not exist.');
    }

    if (!isset($rel_definition[$this->rel_class]['my_key'])) {
      throw new SDBException('REL Definition does not contain my key.');
    }

    return $rel_definition[$this->rel_class]['my_key'];
  }

  /**
   * Gets delete policy for rel objects. It determines
   * how the rel objects will be handled if current
   * instance is deleted.
   *
   * @return string
   *   Policy.
   */
  function getDelPolicy() {
    $rel_definition = $this->getDefinition();

    if (empty($rel_definition)) {
      throw new SDBException('REL Definition for provided class does not exist.');
    }

    if (!isset($rel_definition[$this->rel_class]['del_policy'])) {
      throw new SDBException('REL Definition does not contain del policy.');
    }

    return $rel_definition[$this->rel_class]['del_policy'];
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
   * @return array
   */
  function getDefinition() {
    return $this->definition;
  }

  /**
   * @see getRelDefinition()
   * @param array $definition
   */
  function setDefinition(array $definition) {
    $this->definition = $definition;
  }

  /**
   * Deletes all REL object of current instance.
   *
   * @throws SDBException
   * @return int - Number of affected rows.
   */
  function delAll() {
    $rel_objects = $this->getAll();

    $res = 0;
    foreach ($rel_objects as $rel_object) {
      if (!is_a($rel_object, 'SDBObjectInterface')) {
        throw new SDBException('Provided object is not type of SDBObjectInterface');
      }
      $res += $rel_object->delete();
    }

    // Empty object cache.
    if ($res > 0) {
      unset($this->objects[$this->rel_class]);
    }

    return $res;
  }

  /**
   * Gets REL object by its object key.
   *
   * @param string $obj_unique_key
   *   Object key - it has to be a string or int offset contained in
   *   the rel objects array.
   * @param bool $reload
   * @return SDBObjectInterface
   */
  function getByObjectKey($obj_unique_key, $reload = FALSE) {
    if (!is_string($obj_unique_key)) {
      throw new SDBException('Object unique key must be a string.');
    }

    if (isset($this->objects[$this->rel_class][$obj_unique_key])) {
      return $this->objects[$this->rel_class][$obj_unique_key];
    }

    return NULL;
  }

  /**
   * Will init Rel objects instead of pulling them from db.
   *
   * @param array $init_data
   *   Associative array of object key and object itself or object data.
   */
  function initObjects(array $init_data) {
    if (!isset($this->objects[$this->rel_class]) || !is_array($this->objects[$this->rel_class])) {
      $this->objects[$this->rel_class] = array();
    }

    // This is to get first array item without tampering the array itself.
    foreach ($init_data as $first_item) {
      break;
    }

    // First item is a SDBObject - just add them all.
    if (is_a($first_item, 'SDBObjectInterface')) {
      $this->objects[$this->rel_class] += $init_data;
    }
    // We have only data array, so we will initialize SDBObjects.
    elseif (is_array($first_item) || is_object($first_item)) {
      $this->objects[$this->rel_class] += SDBFactory::initInstancesOf($this->rel_class, $init_data);
    }
  }


  /**
   * Gets REL instance for util purposes.
   *
   * @static
   * @param string $class
   * @return SDBObjectInterface
   */
  protected static function getUtilRelInstance($class) {
    static $instances = array();

    if (!isset($instances[$class])) {
      $instances[$class] = new $class();
    }

    return $instances[$class];
  }
}