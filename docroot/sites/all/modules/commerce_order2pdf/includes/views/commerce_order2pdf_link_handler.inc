<?php

/**
 * Field handler to present an order edit link.
 */
class commerce_order2pdf_handler_field_order_pdf_link extends commerce_order_handler_field_order_link {
  function construct() {
    parent::construct();
  }

  function render($values) {
    $order = commerce_order_new();
    $order->order_id = $this->get_value($values, 'order_id');
    // Add hash key for a download link.
    $hash = hash('md5', $order->order_id . $values->commerce_order_created);

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Download');
    return l($text, "order2pdf/{$order->order_id}/{$hash}");
  }
}
