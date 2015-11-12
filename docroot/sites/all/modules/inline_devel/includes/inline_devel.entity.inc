<?php

/**
 * @file
 * The base class of the inline devel entity.
 */

class InlineDevelEntity extends Entity {

  /**
   * @var
   * The identifier of the entity.
   */
  public $id;

  /**
   * @var
   * The unix timestamp the entity has created.
   */
  public $time;

  /**
   * @var
   * The user which created the entity.
   */
  public $uid;

  /**
   * @var
   * Holds information like: function, arguments and content of functions.
   */
  public $data;

}
