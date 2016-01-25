<?php

/**
 * @file
 * Views handler: Return actions for order id.
 */

/**
 * Overrides uc_order_handler_field_order_actions.
 *
 * @todo This handler will be removed when hook_uc_order_actions_alter will
 *   be implemented.
 *   @see uc_order_access_actions().
 */
class uc_order_access_field_order_actions extends views_handler_field {
  function construct() {
    parent::construct();
    // Add the node fields that comment_link will need..
    $this->additional_fields['order_status'] = array(
      'field' => 'order_status',
    );
    $this->additional_fields['uid'] = array(
      'field' => 'uid',
    );
  }

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $order = (object) array(
      'order_id' => $values->order_id,
      'order_status' => $this->get_value($values, 'order_status'),
      'uid' => $this->get_value($values, 'uid'),
    );

    return uc_order_access_order_actions($order, TRUE);
  }

  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

}
