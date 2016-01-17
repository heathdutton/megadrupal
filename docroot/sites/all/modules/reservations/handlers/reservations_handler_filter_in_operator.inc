<?php

/**
 * @file
 * reservations views handlers
 */


/**
 * Filter by type_setting
 */
class reservations_handler_filter_reservations_node_type_type_setting extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Type setting');
      $options = array(
        'disabled' => t('Disabled'),
        'bucket' => t('Bucket'),
        'resource' => t('Resource'),
      );
      $this->value_options = $options;
    }
  }
}

/**
 * Filter by item status
 */
class reservations_handler_filter_reservations_node_type_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status');
      $this->value_options = reservations_item_status();
    }
  }
}

/**
 * Filter by reservation status
 */
class reservations_handler_filter_reservations_reservation_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status');
      $this->value_options = reservations_record_status();
    }
  }
}

/**
 * Filter by bucket/resource default availability.
 */
class reservations_handler_filter_reservations_bucket_resource_node_default_availability extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Default availability');
      $this->value_options = reservations_item_status();
    }
  }
}

/**
 * Filter by bucket/resource sub type.
 */
class reservations_handler_filter_reservations_bucket_resource_node_sub_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Sub type');
      $this->value_options = array(
        RESERVATIONS_SUB_TYPE_ITEM => t('Item'),
        RESERVATIONS_SUB_TYPE_RESERVATION => t('Reservation placeholder'),
      );
    }
  }
}

/**
 * Filter by bucket/resource default availability.
 */
class reservations_handler_filter_reservations_reservation_item_node_item_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Availability');
      $this->value_options = reservations_item_reservation_status();
    }
  }
}

