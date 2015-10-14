<?php

/**
 * Field handler to present an order edit link.
 */
class commerce_preorder_handler_field_checkout_link extends commerce_order_handler_field_order_link {
  function construct() {
    parent::construct();

    $this->additional_fields['uid'] = 'uid';
  }

  function render($values) {
    global $user;

    $order_id = $this->get_value($values, 'order_id');
    $order_uid = $this->get_value($values, 'uid');

    if (empty($user->uid) || $user->uid != $order_uid) {
      return;
    }

    if (!user_access('access checkout')) {
      return;
    }

    $order = commerce_order_load($order_id);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    if (!isset($order_wrapper->preOrderCheckoutURL)) {
      return;
    }

    $url = $order_wrapper->preOrderCheckoutURL->value();
    if (empty($url)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('checkout');
    return l($text, $url);
  }
}
