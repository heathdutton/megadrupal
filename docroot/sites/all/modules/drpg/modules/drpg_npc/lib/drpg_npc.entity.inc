<?php

/**
 * @file
 * Defines the NPC entity.
 */

class DrpgNPC extends Entity {

  // @codingStandardsIgnoreStart
  public $npc_id = NULL;
  public $type = '';
  public $label = '';
  public $created = '';
  public $updated = '';
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    parent::__construct($values, 'drpg_npc');

    // Check for new NPC.
    if (!$this->npc_id) {
      $this->created = time();
    }
  }

  /**
   * Specifies the default entity label.
   */
  protected function defaultLabel() {
    return $this->label;
  }

  /**
   * Specifies the entity URI.
   */
  protected function defaultUri() {
    return array('path' => 'admin/drpg/npcs/manage/' . $this->internalIdentifier());
  }

}
