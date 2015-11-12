<?php

/**
 * @file
 * "timeinterval" implementation for mappings. Returns TRUE if it's one child
 * returns
 * FALSE.
 */
class CqMappingTimeInterval extends CqAbstractMapping {
  /**
   * The identifier of the hotspot.
   *
   * @var integer
   */
  private $start;
  /**
   * The identifier of the hotspot.
   *
   * @var integer
   */
  private $end;

  /**
   * Implements CqAbstractMapping::evaluate()
   */
  function evaluate() {
    if (isset($this->children[0])) {
      return (!$this->children[0]->evaluate());
    }
    return FALSE;
  }

  /**
   * Overrides CqAbstractMapping::getAllText()
   */
  public function getAllText() {
    $retval = array();
    $retval['logic']['#markup'] = 'NOT';
    $retval += parent::getAllText();
    return $retval;
  }

}
