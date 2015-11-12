<?php
/**
 * @file
 * The database connection and tools.
 */
namespace Ming\Core;

use Mongo;
use MongoDB;
use MongoCollection;
use MongoCursor;
use MongoId;

/**
 * The Database connection and tools
 */
class Database {

  /**
   * The MongoDB object
   *
   * @var MongoDB
   */
  public $mongoDB;

  /**
   * The Mongo connection object
   */
  public $connection;

  /**
   * The currently active collection
   *
   * @var MongoCollection
   */
  public $collection;

  /**
   * The last inserted QID
   */
  protected $last_insert;

  /**
   * The last retrieved find result
   */
  protected $last_cursor;

  /**
   * Constructor function
   *
   * @param \MongoDB $mongo
   */
  public function __construct(MongoDB $mongo) {
    $this->setDatabase($mongo);
  }

  /**
   * Select the database to use
   *
   * @param \MongoDB $db
   */
  public function setDatabase(MongoDB $db) {
    $this->mongoDB = $db;
  }

  /**
   * Set the connection
   *
   * @param $connection
   */
  public function setConnection($connection) {
    $this->connection = $connection;
  }

  /**
   * Returns a collection object from a db
   *
   * @param $c_name
   *
   * @return bool|$MongoCollection
   */
  public function collection($c_name) {
    if (!isset($this->mongoDB)) {
      return FALSE;
    }
    $this->mongoDB->selectCollection($c_name);
    $this->collection = $this->mongoDB->selectCollection($c_name);
    return $this->collection;
  }

  /**
   * Insert into a collection
   *
   * @todo Support optional parameters fsync and timeout
   * @todo Error handling for return values
   *
   * @param array $data
   * @param bool $safe
   *
   * @return array
   *  An array of keys indicating the status of the insert result:
   *    'success' - TRUE or FALSE, depending on the insert result. Note that non-
   *                safe queries may always return TRUE regardless of their actual
   *                success.
   *    'id' - The ID of the inserted object.
   *  If $safe is TRUE, the following Mongo keys will also be available:
   *    'n' - The number of objects affected.
   *    'connectionId'  - The connection in use
   *    'err' - A description of any error
   *    'ok'
   *
   * @see
   *  http://php.net/manual/en/mongocollection.insert.php
   */
  public function insert(array $data, $safe = FALSE) {
    if (empty($this->collection)) {
      return FALSE;
    }
    $options = array('safe' => $safe);
    $result = $this->collection->insert($data, $options);

    $this->processWriteResult($result, $data);

    return $this->last_insert;
  }

  /**
   * Update the first matched item
   *
   * For updating properties where only one item should exist
   *
   * @param array $filter
   *  An array of keys to match on
   * @param array $data
   *  The data to pass to Mongo
   * @param bool $partial
   *  If TRUE, this is a partial update and Ming will only update the fields
   *  provided in $data. If FALSE, $data completely overwrites the object.
   * @param array $options
   *  Any other options to pass to Mongo
   *
   * @return array
   *  See \Mongo\Core\Database::insert() for possible values
   */
  public function update(array $filter, array $data, $partial = FALSE, $options = array()) {
    if (empty($this->collection)) {
      return FALSE;
    }
    // Update single - this is the default behaviour, however the Mongo docs
    // suggest that this should be set explicitly for future-proofing
    $options = array('multiple' => FALSE);

    // Fix partial keys
    if (isset($partial) && $partial == TRUE) {
      $data = array('$set' => $data);
    }

    $result = $this->collection->update($filter, $data);

    $this->processWriteResult($result, $data);

    return $this->last_insert;
  }

  /**
   * Update all matched items.
   *
   * Useful for changing properties across a range of items simultaneously.
   *
   * @todo Add support for atomic updates
   *
   * @param array $filter
   *  An array of keys to match on
   * @param array $data
   *  The data to pass to Mongo
   * @param bool $partial
   *  If TRUE, this is a partial update and Ming will only update the fields
   *  provided in $data. If FALSE, $data completely overwrites the object.
   * @param array $options
   *  Any other options to pass to Mongo
   *
   * @return array
   *  See \Mongo\Core\Database::insert() for possible values
   */
  public function updateAll(array $filter, array $data, $partial = FALSE, $options = array()) {
    if (empty($this->collection)) {
      return FALSE;
    }
    // Update multiple
    $options['multiple'] = TRUE;

    $result = $this->collection->update($filter, $data, $options);

    $this->processWriteResult($result, $data);

    return $this->last_insert;
  }

  /**
   * Update an individual item by ID
   *
   * For updating properties on an item when you already know the item's $id
   *
   * @param $id
   * @param array $data
   *
   * @return array
   *  See \Mongo\Core\Database::insert() for possible values
   */
  public function updateByID($id, array $data) {
    if (empty($this->collection)) {
      return FALSE;
    }
    $filter = $this->filterID($id);

    $result = $this->collection->update($filter, $data);

    $this->processWriteResult($result, $data);

    return $this->last_insert;
  }

  /**
   * Upsert an item
   *
   * For updating or creating an object if it does not already exist
   *
   * @param $filter
   *  A normal MongoDB filter array
   * @param array $data
   *  The data to insert
   * @param bool $safe
   *  If TRUE, conduct a safe insert. Note that this returns an exception on
   *  error, which you will need to catch.
   *
   * @return array
   *  See \Mongo\Core\Database::insert() for possible values
   */
  public function upsert($filter, array $data, $safe = FALSE) {
    if (empty($this->collection)) {
      return FALSE;
    }
    $options = array("upsert" => TRUE, "multiple" => FALSE, "safe" => $safe);

    $result = $this->collection->update($filter, $data, $options);

    $this->processWriteResult($result, $data);

    return $this->last_insert;
  }

  /**
   * Delete items matching a filter
   *
   * @param $filter
   *  A normal MongoDB filter array
   * @param bool $safe
   *  If TRUE, conduct a safe insert. Note that this returns an exception on
   *  error, which you will need to catch.
   *
   * @return mixed
   */
  public function delete($filter, $safe = FALSE) {
    return $this->collection->remove($filter, array("safe" => $safe));
  }

  /**
   * Delete an item
   *
   * @param $id
   *  The ID of the item to delete
   * @param bool $safe
   *  If TRUE, conduct a safe insert. Note that this returns an exception on
   *  error, which you will need to catch.
   *
   * @return mixed
   */
  public function deleteByID($id, $safe = FALSE) {
    $filter = $this->filterID($id);
    return $this->collection->remove($filter, array("justOne" => TRUE, "safe" => $safe));
  }

  /**
   * Query the current collection
   *
   * Simply a wrapper around MongoCollection::find(), this function returns a
   * MongoCursor object which should be foreach()ed to get its results. We
   * also store the returned value in $last_cursor just in case we need it
   * again.
   *
   * @param array $filter
   *  A standard MongoDB filter
   *
   * @return MongoCursor | bool
   */
  public function find(array $filter) {
    if (empty($this->collection)) {
      return FALSE;
    }
    $this->last_cursor = $this->collection->find($filter);
    return $this->last_cursor;
  }

  /**
   * Query by ID
   *
   * As finding by ID should only return one result, we shortcut any cursor
   * foreach handling and simply translate the result into an array on return.
   *
   * @param $id
   *  An ID of a Mongo document. We convert to a MongoID automatically.
   *
   * @return array | bool
   * Either the result of the find as an array, or FALSE
   */
  public function findByID($id) {
    if (empty($this->collection)) {
      return FALSE;
    }
    $filter = $this->filterID($id);
    $this->last_cursor = $this->collection->find($filter);
    return iterator_to_array($this->last_cursor);
  }

  /**
   * Shortcut to return all results for a query in an array
   *
   * @param array $filter
   *  A standard MongoDB filter
   *
   * @return array
   *  An array of results
   */
  public function findAll(array $filter) {
    $results = array();

    $this->find($filter);
    if ($this->last_cursor->hasNext()) {
      foreach ($this->last_cursor as $result) {
        $results[] = $result;
      }
    }

    return $results;
  }
  /**
   * Sanitize user input before using it in a filter
   *
   * To avoid request injection attacks, user data used in queries should be in
   * the form of strings before sending it to the query functions (like find(),
   * for example). This function can be used to quickly sanitize data before use.
   *
   * @param $data
   *  The data to sanitize
   *
   * @return string
   */
  public function sanitize($data) {
    return (string) $data;
  }

  /**
   * Shortcut to set up a filter for filtering by ID
   *
   * @param $id
   *
   * @return array
   */
  public function filterID($id) {
    $mid = new MongoId($id);
    $filter = array('_id'=>$mid);
    return $filter;
  }

  /**
   * Given a MongoID, return the ID number
   *
   * @param $data
   * @return bool
   */
  public function extractID($data) {
    $item = NULL;
    if (is_array($data) && isset($data['_id'])) {
      $item = (array) $data['_id'];
    }
    elseif (is_object($data)) {
      $item = (array) $data;
    }
    else {
      return FALSE;
    }

    if (isset($item['$id'])) {
      return $item['$id'];
    }

    return FALSE;
  }

  /**
   * Helper to add a modifier to data
   *
   * @todo Individual validation for modifier types
   *
   * @param $modifier
   *  The Mongo modifier to set
   * @param $data
   *  The data to use
   *
   * @return array
   *  An array suitable for insertion in to Mongo
   */
  public function mod($modifier, $data) {
    if (is_array($data)) {
      foreach ($data as $key => $value) {
        if ($key = '_id') {
          unset($data[$key]);
          ming_error('Mongo IDs can not be used in modifier operations', 'warning');
        }
      }
    }
    return array('$' . $modifier => $data);
  }

  /**
   * Process the result of a write operation
   *
   * @param $result
   *  The result returned by the write operation
   * @param $data
   *  The data passed to the write operation
   */
  protected function processWriteResult($result, $data) {

    if (is_array($result)) {
      $this->last_insert = $result;
      if (is_null($result['err'])) {
        $this->last_insert['success'] = TRUE;
      }
      else {
        $this->last_insert['success'] = FALSE;
      }
    }
    else {
      $this->last_insert['success'] = $result;
    }

    $this->last_insert['id'] = $this->extractID($data);
  }
}
