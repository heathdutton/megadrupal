<?php
/**
 * @file
 * VoiceboardController extends DrupalDefaultEntityController.
 */

/**
 * The Controller for Model entities
 */
class PeerJSPeerController extends EntityAPIController {

  /**
   * Construct the entity.
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a new entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return object
   *   A new instance of the entity type.
   */
  public function create(array $values = array()) {
    global $user;

    $values += array(
      'pid' => '',
      'uid' => $user->uid,
      'cid' => 0,
      'created' => time(),
      'accessed' => time(),
    );

    $entity = parent::create($values);

    return $entity;
  }
}
