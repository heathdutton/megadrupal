<?php

namespace Drupal\panelizer_deploy;


abstract class AbstractConverter implements ConverterInterface {
  /**
   * Track the references
   *
   * @var array
   */
  protected $refs = array();

  /**
   * Get the uuid field for the object
   *
   * @return string
   */
  public function getUUIDField() {
    return "uuid";
  }

  /**
   * Before converting the object setup the
   * tracking of ids and references
   *
   * @param $objects array
   * @return array
   */
  protected function preConvert(&$objects) {
    if (!is_array($objects)) {
      $things = array(&$objects);
    } else {
      $things = &$objects;
    }

    // track the ids
    $ids = array();
    $i = 0;
    foreach ($things as &$thing) {
      $this->refs[$i] = &$thing->{$this->getKeyField()};
      $ids[] = $this->refs[$i];
      $i++;
    }

    return $ids;
  }

  /**
   * Run after the conversion
   *
   * After the ids have been converted update the references
   *
   * @param $new_ids
   */
  protected function postConvert($new_ids) {
    foreach ($this->refs as $i => $value) {
      if (isset($new_ids[$value])) {
        $this->refs[$i] = $new_ids[$value];
      } else {
        $this->refs[$i] = null;
      }
    }
  }


  /**
   * Converts object ids to uuids
   *
   * @param array $objects
   */
  public function convertToUUID(&$objects) {
    $ids = $this->preConvert($objects);
    $new_ids = $this->convertUUIDs($ids);
    $this->postConvert($new_ids);
  }


  /**
   * Converts Objects uuids to ids
   *
   * @param array $objects
   */
  public function convertToID(&$objects) {
    $ids = $this->preConvert($objects);
    $new_ids = $this->convertIds($ids);
    $this->postConvert($new_ids);
  }

  /**
   * A helper function to query a table for a conversion
   *
   * @param $table
   * @param $from
   * @param $to
   * @param $ids
   * @return array
   */
  protected function queryHelper($table, $from, $to, $ids) {
    return db_select($table, 't')
      ->fields('t', array($from, $to))
      ->condition($from, $ids, 'IN')
      ->execute()
      ->fetchAllKeyed();
  }

  /**
   * Convert an array of ids to ids => uuids
   *
   * @param $ids
   *   array of ids
   * @return array
   *   array of id => uuid
   */
  abstract protected function convertUUIDs($ids);

  /**
   * Convert an array of ids to uuid => id
   *
   * @param $uuids
   *   array of uuids
   * @return array
   *   array of uuid => id
   */
  abstract protected function convertIds($uuids);
} 