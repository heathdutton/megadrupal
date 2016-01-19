<?php

/**
 * @file
 * Contains \Drupal\queue_unique\Queue\SystemSetQueue
 */

namespace Drupal\queue_unique\Queue;

class SystemSetQueue implements \DrupalQueueInterface {

  /**
   * Table name.
   */
  const TABLE_NAME = 'queue_unique';

  /**
   * The name of the queue this instance is working with.
   *
   * @var string
   */
  protected $name;

  public function __construct($name) {
    $this->name = $name;
  }

  /**
   * {@inheritdoc}
   */
  public function createItem($data) {
    // During a Drupal 6.x to 7.x update, drupal_get_schema() does not contain
    // the queue table yet, so we cannot rely on drupal_write_record().
    $sql = "INSERT INTO {" . static::TABLE_NAME . "} (name, created, data, md5) VALUES (:name, :created, :data, MD5(:md5))";
    try {
      return (bool) db_query($sql, array(
        ':name' => $this->name,
        ':data' => serialize($data),
        // We cannot rely on REQUEST_TIME because many items might be created
        // by a single request which takes longer than 1 second.
        ':created' => time(),
        ':md5' => $this->name . serialize($data),
      ));
    }
    catch (\PDOException $e) {
      // Do not alter the DrupalQueueInterface by throwing exceptions. If the
      // item could not be inserted due to uniqueness issues, just fail silently
      // and return FALSE as \DrupalQueueInterface expects.
      return FALSE;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function numberOfItems() {
    return db_query('SELECT COUNT(item_id) FROM {' . static::TABLE_NAME . '} WHERE name = :name', array(':name' => $this->name))->fetchField();
  }

  /**
   * {@inheritdoc}
   */
  public function claimItem($lease_time = 30) {
    // Claim an item by updating its expire fields. If claim is not successful
    // another thread may have claimed the item in the meantime. Therefore loop
    // until an item is successfully claimed or we are reasonably sure there
    // are no unclaimed items left.
    while (TRUE) {
      $item = db_query_range('SELECT data, item_id FROM {' . static::TABLE_NAME . '} q WHERE expire = 0 AND name = :name ORDER BY created, item_id ASC', 0, 1, array(':name' => $this->name))->fetchObject();
      if ($item) {
        // Try to update the item. Only one thread can succeed in UPDATEing the
        // same row. We cannot rely on REQUEST_TIME because items might be
        // claimed by a single consumer which runs longer than 1 second. If we
        // continue to use REQUEST_TIME instead of the current time(), we steal
        // time from the lease, and will tend to reset items before the lease
        // should really expire.
        $update = db_update(static::TABLE_NAME)
          ->fields(array(
            'expire' => time() + $lease_time,
          ))
          ->condition('item_id', $item->item_id)
          ->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          $item->data = unserialize($item->data);
          return $item;
        }
      }
      else {
        // No items currently available to claim.
        return FALSE;
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function releaseItem($item) {
    $update = db_update(static::TABLE_NAME)
      ->fields(array(
        'expire' => 0,
      ))
      ->condition('item_id', $item->item_id);
    return $update->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function deleteItem($item) {
    db_delete(static::TABLE_NAME)
      ->condition('item_id', $item->item_id)
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function createQueue() {
    // All tasks are stored in a single database table (which is created when
    // Drupal is first installed) so there is nothing we need to do to create
    // a new queue.
  }

  /**
   * {@inheritdoc}
   */
  public function deleteQueue() {
    db_delete(static::TABLE_NAME)
      ->condition('name', $this->name)
      ->execute();
  }

  /**
   * Gets the schema as expected by Schema API.
   *
   * @return array
   *   A DB schema
   */
  public static function getSchema() {
    module_load_include('install', 'system', 'system');
    $system_schema = system_schema();
    $schema = array(
      static::TABLE_NAME => $system_schema['queue'],
    );

    $schema[static::TABLE_NAME]['fields']['md5'] = array(
      'type' => 'char',
      'length' => 32,
      'not null' => TRUE,
    );
    $schema[static::TABLE_NAME]['unique keys'] = array(
      'md5' => array('md5'),
    );

    return $schema;
  }

}
