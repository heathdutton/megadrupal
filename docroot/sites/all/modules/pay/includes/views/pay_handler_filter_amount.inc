<?php

/**
 * Simple filter to handle greater than/less than filters
 */
class pay_handler_filter_amount extends views_handler_filter_numeric {

  function option_definition() {
    $options = parent::option_definition();

    $options['value'] = array(
      'contains' => array(
        'min' => array('default' => ''),
        'max' => array('default' => ''),
        'value' => array('default' => ''),
      ),
    );

    return $options;
  }

  function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;

    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    $which = 'all';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(2)) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . drupal_clean_css_identifier($this->options['expose']['operator']);
      }
    }

    if ($which == 'all') {
      $form['value']['value'] = array(
        '#type' => 'textfield',
        '#title' => empty($form_state['exposed']) ? t('Value') : '',
        '#size' => 30,
        '#default_value' => $this->value['value'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array($source => $this->operator_values(1)),
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['value'])) {
        $form_state['input'][$identifier]['value'] = $this->value['value'];
      }
    }
    else if ($which == 'value') {
      // When exposed we drop the value-value and just do value if
      // the operator is locked.
      $form['value'] = array(
        '#type' => 'textfield',
        '#title' => empty($form_state['exposed']) ? t('Value') : '',
        '#size' => 30,
        '#default_value' => $this->value['value'],
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value['value'];
      }
    }

    if ($which == 'all' || $which == 'minmax') {
      $form['value']['min'] = array(
        '#type' => 'textfield',
        '#title' => empty($form_state['exposed']) ? t('Min') : '',
        '#size' => 30,
        '#default_value' => $this->value['min'],
      );
      $form['value']['max'] = array(
        '#type' => 'textfield',
        '#title' => empty($form_state['exposed']) ? t('And max') : t('And'),
        '#size' => 30,
        '#default_value' => $this->value['max'],
      );
      if ($which == 'all') {
        $dependency = array(
          '#process' => array('views_process_dependency'),
          '#dependency' => array($source => $this->operator_values(2)),
        );
        $form['value']['min'] += $dependency;
        $form['value']['max'] += $dependency;
      }
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['min'])) {
        $form_state['input'][$identifier]['min'] = $this->value['min'];
      }
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['max'])) {
        $form_state['input'][$identifier]['max'] = $this->value['max'];
      }

      if (!isset($form['value'])) {
        // Ensure there is something in the 'value'.
        $form['value'] = array(
          '#type' => 'value',
          '#value' => NULL,
        );
      }

      $form['value']['value_or_goal'] = array(
        '#type' => 'select',
        '#title' => 'Value',
        '#options' => array(
          'value' => t('Enter a value'),
          'goal' => t('Payment form goal amount'),
        ),
        '#weight' => -1,
        '#default_value' => $this->value['value_or_goal'],
        '#process' => array('views_process_dependency'),
        '#dependency' => array($source => $this->operator_values(1)),
      );
      $form['value']['value']['#title'] = '';
      $form['value']['value']['#dependency'] += array('edit-options-value-value-or-goal' => array('value'));
      $form['value']['value']['#dependency_count'] = 2;
    }
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    $sql = "SELECT SUM($this->real_field) FROM {pay_transaction} px WHERE px.pfid = $this->table_alias.pfid";
    $field_alias = $this->query->add_field('', "($sql)", $this->real_field . '_total_paid');
    $this->field_alias = $field = $field_alias;

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }

  function op_simple($field) {
    if ($this->value['value_or_goal'] == 'goal') {
      $total_goal = $this->query->add_field($this->table_alias, 'total_goal');
      $this->query->add_having($this->options['group'], "$field $this->operator %s", $total_goal);
    }
    else {
      $this->query->add_having($this->options['group'], "$field $this->operator %d", $this->value['value']);
    }
  }

  function op_between($field) {
    if ($this->operator == 'between') {
      $this->query->add_having($this->options['group'], "$field >= %d", $this->value['min']);
      $this->query->add_having($this->options['group'], "$field <= %d", $this->value['max']);
    }
    else {
      $this->query->add_having($this->options['group'], "$field <= %d OR $field >= %d", $this->value['min'], $this->value['max']);
    }
  }

  function op_empty($field) {
    if ($this->operator == 'empty') {
      $operator = "IS NULL";
    }
    else {
      $operator = "IS NOT NULL";
    }

    $this->query->add_having($this->options['group'], "$field $operator");
  }
}
