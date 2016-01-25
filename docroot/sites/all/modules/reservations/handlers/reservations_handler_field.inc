<?php

/**
 * @file
 * reservations views handlers
 */


/**
 * Type setting Bucket|Resource.
 */
class reservations_handler_field_reservations_node_type_type_setting extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['reservations_type_setting'] = 'reservations_type_setting';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $type_setting = $values->{$this->aliases['reservations_type_setting']};
    switch ($type_setting) {
      case 'bucket':
        return t('Bucket');

      case 'resource':
        return t('Resource');

      default:
        return t('Disabled');
    }
  }
}

/**
 * Item status.
 */
class reservations_handler_field_reservations_node_type_status extends views_handler_field {
  function construct() {
    parent::construct();
    // TODO: does this need an alias?
    $this->additional_fields['reservations_active_status'] = 'reservations_active_status';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $statuses = reservations_node_type_status();
    $status = $values->{$this->aliases['reservations_active_status']};
    return $statuses[$status];
  }
}

/**
 * Reservation status.
 */
class reservations_handler_field_reservations_reservation_status extends views_handler_field {
  function construct() {
    parent::construct();
    // TODO: does this need an alias?
    $this->additional_fields['reservations_reservation_status'] = 'reservations_reservation_status';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $statuses = reservations_record_status();
    $status = $values->{$this->aliases['reservations_reservation_status']};
    return $statuses[$status];
  }
}

/**
 * Bucket/resource default availability.
 */
class reservations_handler_field_reservations_bucket_resource_node_default_availability extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['reservations_default_availability'] = 'reservations_default_availability';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $availabilities = reservations_item_status();
    $availability = $values->{$this->aliases['reservations_default_availability']};
    return $availabilities[$availability];
  }
}

/**
 * Bucket|Resource sub type.
 */
class reservations_handler_field_reservations_bucket_resource_node_sub_type extends views_handler_field {


  function construct() {
    parent::construct();
    $this->additional_fields['reservations_sub_type'] = 'reservations_sub_type';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $sub_type = $values->{$this->aliases['reservations_sub_type']};
    switch ($sub_type) {
      case RESERVATIONS_SUB_TYPE_ITEM:
        return t('Item');

      case RESERVATIONS_SUB_TYPE_RESERVATION:
        return t('Reservation placeholder');

      default:
        return t('Unknown');
    }
  }
}

/**
 * Bucket/resource default availability.
 */
class reservations_handler_field_reservations_reservation_item_node_item_status extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['reservations_item_status'] = 'reservations_item_status';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    parent::render($values);
    $availabilities =  reservations_item_reservation_status();
    $availability = $values->{$this->aliases['reservations_item_status']};
    return $availabilities[$availability];
  }
}
