<?php

/**
 * @file
 * Definition of rut_field_handler_filter_rut.
 */

/**
 * This filter combine the rut column and dv column from one input.
 */
class rut_field_handler_filter_rut extends views_handler_filter_string {

  /**
   * Set the operators for rut
   */
  function operators() {
    $operators = array(
      'rut_equal' => array(
        'title' => t('This Rut is equal'),
        'short' => t('rut equal'),
        'method' => 'op_rut_equal',
        'values' => 1,
      ),
    );

    return $operators;
  }


  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    if ($form['value']['#type'] == 'textfield') {
      // Add the theme of the rut field and activate the validate with
      // javascript.
      $form['value']['#theme'][] = 'rut_field';
      $form['value']['#theme_wrappers'] = array('form_element');
      $form['value']['#validate_js'] = TRUE;
      $form['value']['#message_js'] = t('The Rut/Run is invalid in %label', array('%label' => $this->definition['title']));
      rut_add_js_form_element();
    }
  }

  function operator() {
    $op = '=';
    switch ($this->operator) {
      case 'rut_equal':
        return $op;
    }

    return $op;
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    // The real field was the rut and dv.
    $field_rut = $field . '_rut';
    $field_dv = $field . '_dv';

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field_rut, $field_dv);
    }
  }


  function op_rut_equal($field_rut, $field_dv) {
    // From the value detect the rut and the dv.
    list($rut, $dv) = rut_separate_rut($this->value);

    $this->query->add_where($this->options['group'], $field_rut, $rut, $this->operator());
    $this->query->add_where($this->options['group'], $field_dv, $dv, $this->operator());
  }

}
