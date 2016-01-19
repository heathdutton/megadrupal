<?php

namespace Drupal\go1_base;

/**
 * Callback for kv service.
 *
 * Mostly copied from Drupal\Core\KeyValueStore\DatabaseStorage
 */
class KV extends \Drupal\go1_base\KV\StorageBase {
  /**
   * @var string
   */
  protected $table;

  /**
   * @var \Drupal\go1_base\Helper\Wrapper\Database
   */
  protected $db;

  public function __construct($collection, $table = 'go1_kv') {
    $this->collection = $collection;
    $this->table = $table;
    $this->db = go1_container('wrapper.db');
  }

  public function getMultiple($keys) {
    $values = $this->db
      ->query(
        'SELECT name, value'
          . '  FROM {'. $this->table .'}'
          . '  WHERE name IN (:keys) AND collection = :collection'
        ,
        array(
          ':keys' => $keys,
          ':collection' => $this->collection,
        ))
      ->fetchAllKeyed();
    return array_map('unserialize', $values);
  }

  public function getAll() {
    $values = $this->db
      ->query(
        'SELECT name, value'
          . ' FROM {' . $this->table . '}'
          . ' WHERE collection = :collection'
        ,
        array(':collection' => $this->collection)
      )->fetchAllKeyed();
    return array_map('unserialize', $values);
  }

  public function set($key, $value) {
    $this->db->merge($this->table)
      ->key(array('name' => $key, 'collection' => $this->collection))
      ->fields(array('value' => serialize($value)))
      ->execute()
    ;
  }

  public function setIfNotExists($key, $value) {
    return \MergeQuery::STATUS_INSERT == $this->db->merge($this->table)
      ->insertFields(array(
        'collection' => $this->collection,
        'name' => $key,
        'value' => serialize($value),))
      ->condition('collection', $this->collection)
      ->condition('name', $key)
      ->execute()
    ;
  }

  public function deleteMultiple($keys) {
    do {
      $this->db->delete($this->table)
        ->condition('name', array_splice($keys, 0, 1000))
        ->condition('collection', $this->collection)->execute();
    } while (count($keys));
  }

  public function deleteAll() {
    $this->db->delete($this->table)->condition('collection', $this->collection)->execute();
  }
}
