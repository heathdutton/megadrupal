<?php

/**
 * Class for claim entities.
 */
class ClaimEntityClass extends Entity {

  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'claim');
  }

  /**
   * Specifies the default label, which is picked up by label() by default.
   */
  public function defaultLabel() {
    $user  = user_load($this->uid);
    $node  = node_load($this->nid);
    $label = 'Claim #' . $this->id . ': ' . $user->name . '(uid:' . $user->uid . ') claims ' . $node->title . '(nid:' . $node->nid . ')';

    return $label;
  }
}

