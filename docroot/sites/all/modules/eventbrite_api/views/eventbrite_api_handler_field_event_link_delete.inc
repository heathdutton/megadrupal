<?php

/**
 * Field handler to present a link to delete an event.
 */
class eventbrite_api_handler_field_event_link_delete extends eventbrite_api_handler_field_event_link {
  function construct() {
    parent::construct();

    //$this->additional_fields['uid'] = 'uid';
  }

  function render($values) {
    /*// Ensure the user has access to delete this product.
    $product = commerce_product_new();
    $product->product_id = $this->get_value($values, 'product_id');
    $product->type = $this->get_value($values, 'type');
    $product->uid = $this->get_value($values, 'uid');

    if (!commerce_product_access('delete', $product)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    return l($text, 'admin/commerce/products/' . $product->product_id . '/delete', array('query' => drupal_get_destination()));*/

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $eid = $this->get_value($values, 'eid');

    return l($text, 'admin/eventbrite/event/' . $eid . '/delete');
  }
}
