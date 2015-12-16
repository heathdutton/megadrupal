<?php

/**
 * Filter by customer profile type.
 */
class user_addressbook_handler_filter_user_address_default_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Default user address type');
      $types = user_addressbook_address_types();
      foreach ($types as $type => $info) {
        $options[$type] = t($info['title']);
      }
      $this->value_options = $options;
    }
  }
}
