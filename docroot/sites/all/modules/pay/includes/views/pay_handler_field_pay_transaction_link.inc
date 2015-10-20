<?php

/**
 * @file
 * A views field handler that provides a link to a payment transaction.
 */

class pay_handler_field_pay_transaction_link extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array(
      'default' => '',
      'translatable' => TRUE,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function render($values) {
    // Id field is either 'pfid' or 'pay_form_pfid', depending on view's base.
    $key = $this->definition['pay_key'];
    if ($this->table != $this->query->base_table) {
      $key = $this->field_alias ? $this->field_alias : $this->table . '_' . $key;
    }

    if (isset($values->$key)) {
      $path = 'pay/transaction/' . $values->$key;
      $text = !empty($this->options['text']) ? $this->options['text'] : t('Details');
      return l($text, $path);
    }
  }
}
