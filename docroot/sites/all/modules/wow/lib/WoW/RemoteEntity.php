<?php

/**
 * @file
 * Definition of WoW\RemoteEntity.
 */

/**
 * Defines a remote entity class.
 */
abstract class WoWRemoteEntity extends Entity {

  /**
   * Timestamp for entity's last fetch.
   *
   * @var integer
   */
  public $lastFetched = 0;

  /**
   * The entity region.
   *
   * @var string
   */
  public $region;

  public function merge(array $values) {
    foreach ($values as $key => $value) {
      $this->{$key} = $value;
    }
  }

  /**
   * Return the entity remote path relative to wow/api/ in the service.
   */
  abstract public function remotePath();
}
