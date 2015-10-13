<?php

/**
 * @file
 * Adds a Context module reaction for the Casengo module.
 */

/**
 * Add Casengo code to the page.
 */
class CasengoContextReactionAdd extends context_reaction {

  /**
   * Creating the option form.
   */
  public function optionsForm($context) {
    return array(
      'add' => array('#type' => 'value', '#value' => TRUE),
      'note' => array('#type' => 'markup', '#value' => t('Casengo chat code will be added to the page.')),
    );
  }

  /**
   * Returns an array upon submitting option form.
   */
  public function optionsFormSubmit($values) {
    return array('add' => 1);
  }

  /**
   * Executes.
   */
  public function execute() {
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        return TRUE;
      }
    }
  }
}
