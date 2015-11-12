<?php
/**
 * @file
 * PeerJSChat extends Entity.
 */

/**
 * The class used for peerjs chat entities.
 */
class PeerJSChat extends Entity {

  /**
   * Construct the entity.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'peerjs_chat');
  }

  /**
   * Returns the default label.
   */
  public function defaultLabel() {
    return $this->label;
  }

  /**
   * Returns the default uri.
   */
  public function defaultUri() {
    return array('path' => 'peerjs_chat/' . $this->cid);
  }
}
