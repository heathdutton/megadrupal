<?php
/**
 * @file
 * Contains the node taxonomy index access handler.
 */

/**
 * Controls access to a node based on its relationship to a term in the index.
 */
class ACKNodeTaxonomyIndex extends AccessControlKit {
  /**
   * Implements AccessControlKitInterface::getRealms().
   */
  public function getRealms($node) {
    // Return the node's term associations, if any.
    return db_query('SELECT tid FROM {taxonomy_index} WHERE nid = :nid', array(':nid' => $node->nid))->fetchCol();
  }
}
