<?php

/**
 * @file
 * Defines the NPC Instance entity.
 */

class DrpgNPCInstance extends Entity {

  // @codingStandardsIgnoreStart
  public $npc_instance_id = NULL;
  public $type = '';
  public $entity_id = 0;
  public $npc_id = 0;
  public $created = '';
  public $updated = '';

  // Not stored. Used to provide access to referenced attributes.
  public $attributes;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_npc_instance');

    // Check for new NPC Instance.
    if (!$this->npc_instance_id) {
      $this->created = time();
    }
  }
}
