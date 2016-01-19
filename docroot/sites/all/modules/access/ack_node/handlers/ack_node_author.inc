<?php
/**
 * @file
 * Contains the node author access handler.
 */

/**
 * Controls access to a node based on its author.
 */
class ACKNodeAuthor extends AccessControlKit {
  /**
   * Implements AccessControlKitInterface::getRealms().
   */
  public function getRealms($node) {
    return array($node->uid);
  }
}
