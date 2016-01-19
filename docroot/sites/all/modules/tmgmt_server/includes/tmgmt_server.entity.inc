<?php

/**
 * Entity object for the remote source entity.
 */
class TMGMTRemoteSource extends Entity {

  public $rsid;
  public $cid;
  public $data;
  public $reference;
  public $state;
  public $user_agent;

  /**
   * Overrides Entity::__construct().
   */
  function __construct(array $values = array()) {
    parent::__construct($values, 'tmgmt_server_remote_source');

    if (empty($this->rsid)) {
      $this->created = REQUEST_TIME;
    }
  }

}

/**
 * Entity object for the client entity.
 */
class TMGMTClient extends Entity {

  public $cid;

  /**
   * The label of the client entity.
   *
   * @var string
   */
  public $label;

  /**
   * A string that describes the client entity. Can be used by users to help
   * maintaining a list of multiple client entities.
   *
   * @var string
   */
  public $description;

  public $uuid;
  public $uid;

  /**
   * The DS unique identifier.
   *
   * @var integer
   */
  public $ds_uid;

  public $created;
  public $changed;


  /**
   * Overrides Entity::__construct().
   */
  function __construct(array $values = array()) {
    parent::__construct($values, 'tmgmt_server_client');

    if (empty($this->cid)) {
      $this->created = REQUEST_TIME;
    }
  }

}

class TMGMTDirectoryServer extends Entity {
  public $sid;
  public $url;
  public $remote_identifier;
}
