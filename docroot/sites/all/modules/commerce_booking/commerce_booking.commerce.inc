<?php
/**
 * @file
 * Contains Drupal Commerce hooks.
 */

/**
 * Implements hook_commerce_checkout_page_info().
 */
function commerce_booking_commerce_checkout_page_info() {
  $pages = array();

  $pages['terms_conditions'] = array(
    'page_id' => 'terms_conditions',
    'title' => t('Terms & Conditions'),
    'weight' => -99,
    'status_cart' => FALSE,
  );

  $pages['tickets'] = array(
    'page_id' => 'tickets',
    'title' => t('Tickets'),
    'weight' => 1,
    'status_cart' => FALSE,
  );

  return $pages;
}

/**
 * Implements hook_commerce_checkout_pane_info().
 */
function commerce_booking_commerce_checkout_pane_info() {
  $panes = array();

  $panes['terms_conditions'] = array(
    'pane_id' => 'terms_conditions',
    'title' => FALSE,
    'name' => t('Terms & Conditions'),
    'page' => 'terms_conditions',
    'review' => FALSE,
    'file' => 'commerce_booking.checkout.inc',
    'base' => 'commerce_booking_checkout_pane_terms_conditions',
  );

  $panes['tickets'] = array(
    'pane_id' => 'tickets',
    'title' => t('Tickets'),
    'name' => t('Tickets'),
    'page' => 'tickets',
    'file' => 'commerce_booking.checkout.inc',
    'base' => 'commerce_booking_checkout_pane_tickets',
  );

  $panes['ticket_payments'] = array(
    'pane_id' => 'ticket_payments',
    'title' => t('Tickets'),
    'name' => t('Ticket Payments'),
    'page' => 'review',
    'file' => 'commerce_booking.checkout.inc',
    'base' => 'commerce_booking_checkout_pane_ticket_payments',
  );

  return $panes;
}

/**
 * Implements hook_commerce_line_item_type_info().
 */
function commerce_booking_commerce_line_item_type_info() {
  $line_item_types = array();

  $line_item_types['commerce_booking_ticket'] = array(
    'type' => 'commerce_booking_ticket',
    'name' => t('Ticket'),
    'description' => t('References a ticket and event.'),
    'product' => TRUE,
    'add_form_submit_value' => t('Add ticket'),
    'callbacks' => array(
      'title' => 'commerce_booking_line_item_ticket_title',
    ),
  );

  return $line_item_types;
}

/**
 * Title callback for commerce_booking_ticket line item.
 */
function commerce_booking_line_item_ticket_title($line_item) {
  $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $num_tickets = 0;
  foreach ($wrapper->commerce_booking_ticket as $ticket_wrapper) {
    $num_tickets++;
  }

  $event_label = $wrapper->order->commerce_booking_event->label();
  return $num_tickets ? t('@num tickets for @event', array('@num' => $num_tickets, '@event' => $event_label)) : t('Invalid Ticket');
}

/**
 * Calculate the value of a line item.
 *
 * @param stdClass $line_item
 *   The line item to calculate the value of.
 */
function commerce_booking_line_item_ticket_process(&$line_item) {
  if ($line_item->type != 'commerce_booking_ticket') {
    drupal_set_message('Invalid line item type.', 'error');
    return;
  }

  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

  // Sum the ticket prices.
  $total_amount = 0;
  foreach ($line_item_wrapper->commerce_booking_ticket as $ticket_wrapper) {
    if ($ticket_wrapper->value()) {
      $price = $ticket_wrapper->commerce_booking_ticket_price->value();
      $total_amount += $price['amount'];

      // If one of the tickets has no value set stop us from losing currency
      // information.
      if (!empty($price['currency_code'])) {
        $currency = $price['currency_code'];
      }
    }
    else {
      watchdog('commerce_booking', 'Ticket %ticket_id not found for order %order_id.', array(
        '%ticket_id' => $ticket_wrapper->getIdentifier(),
        '%order_id' => $line_item_wrapper->order_id->value(),
      ), WATCHDOG_WARNING);
    }
  }

  $line_item->commerce_unit_price[LANGUAGE_NONE][0]
    = $line_item->commerce_total[LANGUAGE_NONE][0] = array(
      'amount' => $total_amount,
      'currency_code' => isset($currency) ? $currency : commerce_default_currency(),
    );

  if (!is_null($line_item_wrapper->commerce_unit_price->value())) {
    // Add the base price to the components array.
    if (!commerce_price_component_load($line_item_wrapper->commerce_unit_price->value(), 'base_price')) {
      $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
        $line_item_wrapper->commerce_unit_price->value(),
        'base_price',
        $line_item_wrapper->commerce_unit_price->value(),
        TRUE
      );
    }
  }

  if (!is_null($line_item_wrapper->commerce_total->value())) {
    // Add the base price to the components array.
    if (!commerce_price_component_load($line_item_wrapper->commerce_total->value(), 'base_price')) {
      $line_item_wrapper->commerce_total->data = commerce_price_component_add(
        $line_item_wrapper->commerce_total->value(),
        'base_price',
        $line_item_wrapper->commerce_total->value(),
        TRUE
      );
    }
  }
}

/**
 * Implements hook_commerce_checkout_complete().
 */
function commerce_booking_commerce_checkout_complete($order) {
  // Only act on commerce booking orders.
  if ($order->type != 'commerce_booking') {
    return;
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // On each ticket, confirm and save.
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == 'commerce_booking_ticket') {
      foreach ($line_item_wrapper->commerce_booking_ticket as $ticket_wrapper) {
        $ticket = $ticket_wrapper->value();
        $ticket->confirm();
        $ticket->save();
      }
    }
  }
}

/**
 * Implements hook_commerce_checkout_router().
 *
 * Prevent access to closed bookings.
 */
function commerce_booking_commerce_checkout_router($order, $checkout_page) {
  // We only want to deal with bookings.
  if ($order->type == 'commerce_booking') {
    $event_type = commerce_booking_get_event_entity_type($order);
    $event = commerce_booking_get_event_entity($order);
    $field_name = commerce_booking_get_field_name($event_type, $event);

    // Skip the Terms and Conditions page if there are none.
    if ($checkout_page['page_id'] == 'terms_conditions') {
      list(,,$bundle) = entity_extract_ids($event_type, $event);
      $instance = field_info_instance($event_type, $field_name, $bundle);
      if (empty($instance['settings']['tc_field']) || empty($event->{$field_name})) {
        $order->commerce_booking_tcs[LANGUAGE_NONE][0]['value'] = 1;
        $order->status = 'checkout_' . $checkout_page['next_page'];
        commerce_order_save($order);
        drupal_goto('checkout/' . $order->order_id . '/' . $checkout_page['next_page']);
      }
    }

    // Go to the tickets step for anything after tickets.
    $current = menu_get_item();
    if (!empty($current['map'][2])) {
      return;
    }

    $statuses = array(
      'checkout_tickets',
      'checkout_accommodation',
      'checkout_review',
      'checkout_payment',
      'checkout_complete',
    );

    if (in_array($order->status, $statuses)) {
      $targeturi = 'checkout/' . $order->order_id . '/tickets';
      return drupal_goto($targeturi);
    }
  }
}

/**
 * Implements hook_commerce_payment_transaction_insert().
 */
function commerce_booking_commerce_payment_transaction_insert($transaction, $type = 'commerce_payment_transaction') {
  $transaction_wrapper = entity_metadata_wrapper('commerce_payment_transaction', $transaction);

  $order = $transaction_wrapper->order->value();

  if (empty($order->data['ticket_amounts'])) {
    return;
  }

  foreach ($order->data['ticket_amounts'] as $ticket_id => $amount) {
    if (!is_numeric($ticket_id)) {
      continue;
    }

    $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket_id);

    $item = entity_create('field_collection_item', array(
      'field_name' => 'commerce_booking_ticket_payments',
    ));
    $item->setHostEntity('commerce_booking_ticket', $ticket_wrapper->value());

    $item_wrapper = entity_metadata_wrapper('field_collection_item', $item);
    $item_wrapper->cbt_payment_payment = $transaction;
    $item_wrapper->cbt_payment_value = $amount;
    $item_wrapper->save();
    $ticket_wrapper->save();

    // Clear the ticket amount so we don't end up doing this again.
    unset($order->data['ticket_amounts'][$ticket_id]);
  }
}

/**
 * Implements hook_commerce_order_state_info().
 *
 * Define order states for the booking system including
 *  - booking
 *  - booking_event_past
 */
function commerce_booking_commerce_order_state_info() {
  $order_states = array();

  $order_states['booking_event_past'] = array(
    'name' => 'booking_event_past',
    'title' => t('Booking Event Past'),
    'description' => t('Orders in this state are bookings for events that have passed their start date.'),
    'weight' => 11,
    'default_status' => 'booking_event_past',
  );

  return $order_states;
}

/**
 * Implements hook_commerce_order_status_info().
 */
function commerce_booking_commerce_order_status_info() {
  $statuses = array();

  $statuses['booking_event_past'] = array(
    'name' => 'booking_event_past',
    'title' => t('Booking Event is Over'),
    'state' => 'booking_event_past',
  );

  return $statuses;
}
