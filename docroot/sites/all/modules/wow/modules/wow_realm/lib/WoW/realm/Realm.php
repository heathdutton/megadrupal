<?php

/**
 * @file
 * Definition of WoW\realm\Realm.
 */

/**
 * Defines the wow_realm entity class.
 */
class WoWRealm extends Entity {

  /**
   * The realm ID.
   *
   * @var integer
   */
  public $rid;

  /**
   * The realm type: pvp, pve, rp or rppvp.
   *
   * @var string
   */
  public $type;

  /**
   * The population density: low, medium, or high.
   *
   * @var string
   */
  public $population;

  /**
   * Whether the server has queue to enter.
   *
   * @var integer
   */
  public $queue;

  /**
   * Whether the server is running(1) or not(0).
   *
   * @var integer
   */
  public $status;

  /**
   * The realm region.
   *
   * @var string
   */
  public $region;

  /**
   * The realm name.
   *
   * @var string
   */
  public $name;

  /**
   * The realm machine name (slug).
   *
   * @var string
   */
  public $slug;

  /**
   * @todo: foreign key.
   * The realm battle group.
   *
   * @var string
   */
  public $battlegroup;

}
