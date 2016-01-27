<?php

/**
 * @file
 * Defines a node type ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

/**
 * A node type ad tier plugin.
 */
class TestNodeType extends NodeType {

  /**
   * {@inheritdoc}
   */
  protected function getActiveNode() {
    static $calls;
    if (!isset($calls)) {
      $calls = TRUE;
      return (object) array(
        'type' => 'foo',
      );
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  protected static function filter($value) {
    return htmlspecialchars($value);
  }

}
