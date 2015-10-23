<?php

/**
 * @file
 * Contains the basic 'order' field handler.
 */

/**
 * Field handler: simple renderer that links to the order administration page.
 *
 * @TODO This is useless because if user cannot see the order he cannot see
 *       it's order_id with or without link also. We overwrite this here just
 *       in case we change this behaviour in future.
 */
class uc_order_access_handler_field_order_id extends uc_order_handler_field_order_id {
  /**
   * Override uc_order_handler_field_order_id::render_link().
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_order'])) {
      $this->options['alter']['make_link'] = FALSE;

      $order = uc_order_load($this->get_value($values, 'order_id'));
      if (user_access('view all orders') || uc_order_access_check_access($order, 'view order')) {
        $path = 'admin/store/orders/' . $this->get_value($values, 'order_id');
      }
      elseif (user_access('view own orders') && $this->get_value($values, 'uid') == $GLOBALS['user']->uid) {
        $path = 'user/' . $GLOBALS['user']->uid . '/orders/' . $this->get_value($values, 'order_id');
      }
      else {
        $path = FALSE;
      }

      if ($path && $data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = $path;
      }
    }
    return $data;
  }
}
