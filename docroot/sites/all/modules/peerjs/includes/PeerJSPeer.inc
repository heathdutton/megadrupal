<?php
/**
 * @file
 * PeerJSChat extends Entity.
 */

/**
 * The class used for peerjs chat entities.
 */
class PeerJSPeer extends Entity {

  /**
   * Construct the entity.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'peerjs_peer');
  }

  /**
   * Returns the default label.
   */
  public function defaultLabel() {
    return $this->id;
  }

  /**
   * Returns the default uri.
   */
  public function defaultUri() {
    return array('path' => 'peerjs_peer/' . $this->id);
  }
}
