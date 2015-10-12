<?php

/**
 * @file
 * Views handler.
 */

/**
 * Filters by order status.
 */
class uc_order_access_handler_filter_order_status extends uc_order_handler_filter_order_status {

  /**
   * Overrides views_handler_filter_in_operator::get_value_options().
   */
  function get_value_options() {
    if (!isset($this->value_options)) {
      $permissions = uc_order_access_get_perms_statuses();
      $options['_active'] = t('Active');
      foreach (uc_order_status_list() as $status => $name) {
        if (in_array($name['id'], $permissions)) {
          $options[$name['id']] = $name['title'];
        }
      }

      $this->value_title = t('Order status');
      $this->value_options = $options;
    }
  }
}
