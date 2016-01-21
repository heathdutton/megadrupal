<?php
/**
 * @file
 */

/**
 * Expose cart contents as a context condition.
 */
class commerce_cart_context_cart_contains_items extends context_condition {
  function condition_values() {
    return array(
      0 => t('Cart is empty.'),
      1 => t('Cart contains items.'),
    );
  }

  function condition_form($context = NULL) {
    $form = parent::condition_form($context);

    $values = $this->fetch_from_context($context, 'values');

    $form['#markup'] = t('Cart contents');
    $form['#weight'] = -10;
    $form['#type'] = 'radios';
    $form['#default_value'] = (isset($values[0])) ? $values[0] : 1;

    return $form;
  }

  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    return array($values);
  }

  /**
   * Execute condition test.
   */
  function execute() {
    global $user;

    foreach ($this->get_contexts() as $context) {
      $values = $this->fetch_from_context($context, 'values');
      $value = $values[0];


      if ($order = commerce_cart_order_load($user->uid)) {
        // Count the number of product line items on the order.
        $wrapper = entity_metadata_wrapper('commerce_order', $order);
        $quantity = commerce_line_items_quantity($wrapper->commerce_line_items, commerce_product_line_item_types());
        if ($quantity > 1) $quantity = 1;

        if ($quantity == $value) {
          $this->condition_met($context);
        }
      }
      else if (empty($value)) {
        // Cart should be empty, there is no order...
        $this->condition_met($context);
      }
    }
  }
}
