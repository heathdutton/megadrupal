<?php
/**
 * @file
 * Contains the node "is sticky" access handler.
 */

/**
 * Controls access to a node based on its "sticky" property.
 */
class ACKNodeSticky extends AccessControlKit {
  /**
   * Implements AccessControlKitInterface::getRealms().
   */
  public function getRealms($node) {
    return array($node->sticky);
  }
}
