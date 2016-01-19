<?php
// $Id:

/**
 * @file
 *   Adds a Context module reaction for the TopBar message module.
 */

/**
 * Add TopBar message code to the page.
 */
class topbar_msg_context_reaction_add extends context_reaction {

  function options_form($context) {
    return array(
      'add' => array('#type' => 'value', '#value' => TRUE),
      'note' => array('#type' => 'markup', '#value' => t('TopBar message code will be added to the page.')),
    );
  }

  function options_form_submit($values) {
    return array('add' => 1);
  }

  function execute() {
    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      if (!empty($context->reactions[$this->plugin])) {
        return TRUE;
      }
    }
  }
}
