<?php

/**
 * @file
 * Export a variable. Always returns true.
 */
class CqMappingMathExport extends CqAbstractMapping {

  /**
   * Implements CqAbstractMapping::evaluate()
   */
  function evaluate() {
    $varname = $this->getParam('varname');
    $exportname = $this->getParam('exportname');
    $exportnode = (int) $this->getParam('exportnode');
    if (is_null($varname)) {
      drupal_set_message(t('MathExport without varname attribute found.'), 'warning');
      return TRUE;
    }
    if (is_null($exportname)) {
      drupal_set_message(t('MathExport without exportname attribute found.'), 'warning');
      return TRUE;
    }
    $uid = $this->context->getUserAnswer()->getUserId();
    $thisNodeId = $this->context->getNode()->nid;
    if ($exportnode === 0 || $exportnode === $thisNodeId) {
      // In this case the userAnswer was cached in the form.
      $userAnswer =& $this->context->getUserAnswer();
    } else {
      $userAnswer =& closedquestion_get_useranswer($exportnode, $uid);
    }

    $value = $this->context->evaluateMath($varname);

    $exports = $userAnswer->getData("export");
    if ($exports === NULL) {
      $exports = array();
    }
    $exports[$exportname] = $value;
    $userAnswer->setData("export", $exports);
    $userAnswer->store();
    
    return TRUE;
  }

  /**
   * Overrides CqAbstractMapping::getAllText()
   */
  public function getAllText() {
    $retval = array();
    $retval['logic']['#markup'] = 'Export';
    $retval += parent::getAllText();
    return $retval;
  }

}
