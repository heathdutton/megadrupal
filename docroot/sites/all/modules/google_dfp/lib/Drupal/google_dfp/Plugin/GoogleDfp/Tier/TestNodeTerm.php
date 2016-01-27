<?php

/**
 * @file
 * Defines a node term ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Tier;

/**
 * A node term ad tier plugin.
 */
class TestNodeTerm extends NodeTerm {

  /**
   * {@inheritdoc}
   */
  protected function getActiveNode() {
    return (object) array(
      'field_foo' => array(
        'und' => array(
          array('tid' => 1),
        ),
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  protected function getFieldItems($entity_type, $entity, $field_name) {
    if ($field_name != 'field_baz') {
      return array(
        array('tid' => 1),
      );
    }
    return array();
  }

  /**
   * {@inheritdoc}
   */
  protected function loadTerms($ids) {
    if (is_array($ids)) {
      return array(
        (object) array('name' => 'foo'),
        (object) array('name' => 'bar'),
      );
    }
    return (object) array('name' => 'foo');
  }

  /**
   * {@inheritdoc}
   */
  protected static function filter($value) {
    return htmlspecialchars($value);
  }

}
