<?php

/**
 * @file
 * UI for Any and All loops.
 */

/**
 * UI for Rules Any and All loops.
 */
class RulesListConditionsUI extends RulesLoopUI {

  /**
   * Add negate checkbox.
   */
  public function form(&$form, &$form_state, $options = array()) {
    parent::form($form, $form_state, $options);

    $form['negate'] = array(
      '#title' => t('Negate'),
      '#type' => 'checkbox',
      '#description' => t('If checked, the loop result is negated such that it returns TRUE if the loop evaluates to FALSE.'),
      '#default_value' => $this->element->isNegated(),
      '#weight' => 9,
    );

  }

  /**
   * Set negate.
   */
  public function form_extract_values($form, &$form_state) {
    parent::form_extract_values($form, $form_state);
    $form_values = RulesPluginUI::getFormStateValues($form, $form_state);

    if (isset($form_values['negate'])) {
      $this->element->negate($form_values['negate']);
    }
  }
}
