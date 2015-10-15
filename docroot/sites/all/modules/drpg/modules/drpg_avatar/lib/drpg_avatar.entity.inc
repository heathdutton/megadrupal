<?php

/**
 * @file
 * Defines the avatar entity.
 */

class DrpgAvatar extends Entity {

  // @codingStandardsIgnoreStart
  public $avatar_id = NULL;
  public $owner_uid = NULL;
  public $label = '';
  public $currency = 0;
  public $created = '';
  public $updated = '';

  // Not stored. Used to provide access to referenced attributes.
  public $attributes;
  // @codingStandardsIgnoreEnd

  /**
   * Override parent constructor.
   */
  public function __construct(array $values = array()) {
    global $user;

    parent::__construct($values, 'drpg_avatar');

    // Check for new avatar.
    if (!$this->avatar_id) {
      $this->owner_uid = $user->uid;
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
    return array('path' => 'admin/drpg/avatars/manage/' . $this->internalIdentifier());
  }

}
