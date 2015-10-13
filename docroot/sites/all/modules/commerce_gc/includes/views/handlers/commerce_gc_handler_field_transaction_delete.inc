<?php

/**
 * @file
 * Provides a transaction delete link.
 */

class commerce_gc_handler_field_transaction_delete extends views_handler_field {
  function construct() {
    parent::construct();
    
    $this->additional_fields['transaction_id'] = 'transaction_id';
    $this->additional_fields['coupon_id'] = 'coupon_id';
  }
  
  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => 'delete');

    return $options;
  }

  /**
   * Provide the currency format option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }  

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }
  
  function render($values) {
    if (user_access('delete giftcard transactions')) {
      $transaction_id = $this->get_value($values, 'transaction_id');
      $coupon_id = $this->get_value($values, 'coupon_id');
      $text = $this->options['text'];

      return l($text, 'giftcards/' . $coupon_id . '/transactions/' . $transaction_id . '/delete');
    }
  }
}