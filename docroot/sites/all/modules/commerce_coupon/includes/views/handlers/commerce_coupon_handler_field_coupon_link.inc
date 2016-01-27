<?php

/**
 * @file
 * Base class for coupon links.
 */

/**
 * Base class for coupon links.
 */
abstract class commerce_coupon_handler_field_coupon_link extends views_handler_field {
  /**
   * Overrides parent::construct().
   */
  function construct() {
    parent::construct();

    $this->additional_fields['coupon_id'] = 'coupon_id';
  }

  /**
   * Overrides parent::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Overrides parent::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  /**
   * Overrides parent::check_access().
   */
  function check_access($coupon_id, $op) {
    $coupon = commerce_coupon_create('discount');
    $coupon->coupon_id = $coupon_id;

    if (commerce_coupon_access($op, $coupon)) {
      return TRUE;
    }
  }

}
