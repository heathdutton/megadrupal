<?php

/**
 * @file
 * Implement handler for filtering by allocation.
 */

class ec_store_views_handler_filter_allocation extends views_handler_filter_in_operator {

  function construct() {
    parent::construct();
    $this->value_title = t('Allocation');
  }

  /**
   * Child classes should override this function to set the 'value options'.
   * This can use a guard to be used to reduce database hits as much as
   * possible.
   */
  function get_value_options() {
    $this->value_options = ec_store_transaction_allocation();
  }

  function exposed_form(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    if (!empty($this->options['expose']['use_operator']) && !empty($this->options['expose']['operator'])) {
      $operator = $this->options['expose']['operator'];
      $this->operator_form($form, $form_state);
      $form[$operator] = $form['operator'];

      if (isset($form[$operator]['#title'])) {
        unset($form[$operator]['#title']);
      }

      $this->exposed_translate($form[$operator], 'operator');

      unset($form['operator']);
    }

    if (!empty($this->options['expose']['identifier'])) {
      $value = $this->options['expose']['identifier'];
      $this->value_form($form, $form_state);
      $form[$value] = $form['value'];

      if (isset($form[$value]['#title']) && !empty($form[$value]['#type']) && $form[$value]['#type'] != 'checkbox') {
        unset($form[$value]['#title']);
      }

      if (!empty($this->options['expose']['single'])) {
        $form[$value]['#type'] = 'radios';
      }

      if (!empty($form['#type']) && ($form['#type'] == 'checkboxes' || ($form['#type'] == 'select' && !empty($form['#multiple'])))) {
        unset($form[$value]['#default_value']);
      }

      if (!empty($form['#type']) && $form['#type'] == 'select' && empty($form['#multiple'])) {
        $form[$value]['#default_value'] = 'All';
      }

      if ($value != 'value') {
        unset($form['value']);
      }
    }
  }

  function query() {
    if (empty($this->value)) {
      return;
    }

    // As checkboxes and radios return 0 for all positions we want to filter
    // out the zeros
    $this->value = array_filter($this->value);


    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where_expression($this->options['group'], "{$this->table_alias}.{$this->real_field} {$this->operator} (:allocation_types)", array(':allocation_types' => $this->value));
  }
}
