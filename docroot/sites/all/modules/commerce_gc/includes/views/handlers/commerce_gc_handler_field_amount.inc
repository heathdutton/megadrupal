<?php

/**
 * @file
 * Provides the transaction amount field handler.
 */

class commerce_gc_handler_field_amount extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['display_format'] = array('default' => 'formatted');

    return $options;
  }

  /**
   * Provide the currency format option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['display_format'] = array(
      '#type' => 'select',
      '#title' => t('Display format'),
      '#options' => array(
        'formatted' => t('Currency formatted amount'),
        'raw' => t('Raw amount'),
      ),
      '#default_value' => $this->options['display_format'],
    );
  }

  function render($values) {
    $value = $this->get_value($values);
    $currency_code = commerce_default_currency();

    switch ($this->options['display_format']) {
      case 'formatted':
        return commerce_currency_format($value, $currency_code);

      case 'raw':
        // First load the full currency array.
        $currency = commerce_currency_load($currency_code);

        // Format the price as a number.
        return number_format(commerce_currency_round(commerce_currency_amount_to_decimal($value, $currency_code), $currency), $currency['decimals']);
    }
  }
}
