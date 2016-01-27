<?php

/**
 * @file
 * Defines a test node term ad tier.
 */

namespace Drupal\google_dfp\Plugin\GoogleDfp\Keyword;


/**
 * A test node term ad tier plugin.
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
      'field_bar' => array(
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
        array('tid' => $field_name),
      );
    }
    return array();
  }

  /**
   * {@inheritdoc}
   */
  protected function loadTerms($ids) {
    $return = array();
    foreach ($ids as $id) {
      $return[] = (object) array('name' => $id);
    }
    return $return;
  }

  /**
   * {@inheritdoc}
   */
  protected static function filter($value) {
    return htmlspecialchars($value);
  }

}
