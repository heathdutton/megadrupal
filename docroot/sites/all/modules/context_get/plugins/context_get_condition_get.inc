<?php

/**
 * Expose $_GET parameters as a context condition.
 */
class context_get_condition_get extends context_condition {

  /**
   * Omit condition values. We will provide a custom input form for our conditions.
   */
  function condition_values() {
    return array();
  }
  
  /**
   * Condition form.
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    unset($form['#options']);

    $form['#type'] = 'textarea';
    $form['#default_value'] = implode("\n", $this->fetch_from_context($context, 'values'));
    return $form;
  }
  
  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    $parsed = array();
    $items = explode("\n", $values);
    if (!empty($items)) {
      foreach ($items as $v) {
        $v = trim($v);
        if (!empty($v)) {
          $parsed[$v] = $v;
        }
      }
    }
    return $parsed;
  }
  
  /**
   * Execute.
   */
  function execute() {
    if ($this->condition_used()) {
      foreach ($this->get_contexts() as $context) {
        $gets = $this->fetch_from_context($context, 'values');
        if ($this->match($_GET, $gets)) {
          $this->condition_met($context);
        }
      }
    }
  }
  
  /**
   * Match the provided combonations with what is in the provided
   * $_GET variable
   *
   * @param mixed $subject
   *   The $_GET variable.
   * @param array $combos
   *   A keyed array of subject => value combinations.
   */
  protected function match($get, $combos) {
    $matches = 0;
    foreach($combos as $combo) {
      $combo = explode("=", $combo);
      if(isset($get[$combo[0]]) && $get[$combo[0]] == $combo[1]) {
        $matches++;
      }
      // Make sure all of the conditions were matched
      if($matches == count($combos)) {
        return TRUE;
      }
    }
  }
  
}
