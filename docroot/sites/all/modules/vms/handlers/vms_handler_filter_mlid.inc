<?php
/**
 * @file
 * Filter handler for allowing restricting integer fields based on currently
 * active menu items (plus trails).
 */

/**
 * Filter to compare a field against currently active menu link item.
 */
class vms_handler_filter_mlid extends views_handler_filter_equality {
  /**
   * Provide simple equality operator.
   */
  function operator_options() {
    return array(
      '=' => t('Is in'),
      '!=' => t('Is not in'),
    );
  }

  /**
   * Provide a select list to select (replacement) value.
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Menu item'),
      '#options' => vms_mlid_query_types(),
      '#default_value' => $this->value,
    );
  }

  /**
   * Display the filter on the administrative summary.
   */
  function admin_summary() {
    $options = vms_mlid_query_types();
    $operators = $this->operator_options();
    return check_plain($operators[$this->operator] . ' ' . $options[$this->value]);
  }

  /**
   * This filter cannot be exposed.
   */
  function can_expose() {
    return FALSE;
  }

  /**
   * Add the filtering to the query.
   *
   * Potentially redundant if the parent class could be replaced with an IN
   * handler.
   */
  public function query() {
    $this->ensure_my_table();

    $vms_target = vms_get_current_mlid($this->options['value']);
    $table = $this->table_alias . '.' . $this->real_field;

    if ($this->operator == '=') {
      $this->query->add_where(
        $this->options['group'],
        $table,
        $vms_target,
        'IN'
      );
    }
    else {
      $this->query->add_where(
        $this->options['group'],
        db_or()
          ->condition($table, $vms_target, 'NOT IN')
          ->condition($table, $vms_target, 'IS NULL')
      );
    }
  }
}
