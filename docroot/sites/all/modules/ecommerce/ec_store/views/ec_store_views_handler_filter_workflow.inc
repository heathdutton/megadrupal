<?php

/**
 * @file
 * Implement handler for filtering by transaction workflow.
 */

class ec_store_views_handler_filter_workflow extends views_handler_filter_in_operator {

  function construct() {
    parent::construct();
    $this->value_title = t('Workflow');
  }

  /**
   * Child classes should override this function to set the 'value options'.
   * This can use a guard to be used to reduce database hits as much as
   * possible.
   */
  function get_value_options() {
    $this->value_options = ec_store_transaction_workflow();
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['#type'] = 'checkboxes';
    $form['value']['#type'] = 'checkboxes';
    $form['#multiple'] = TRUE;
    $form['#no_convert'] = TRUE;
  }

  function query() {

    // As checkboxes and radios return 0 for all positions we want to filter
    // out the zeros
    $this->value = array_filter($this->value);

    if (empty($this->value)) {
      return;
    }

    $this->ensure_my_table();

    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where_expression($this->options['group'], "{$this->table_alias}.{$this->real_field} {$this->operator} (:transaction_workflow)", array(':transaction_workflow' => $this->value));
  }
}
