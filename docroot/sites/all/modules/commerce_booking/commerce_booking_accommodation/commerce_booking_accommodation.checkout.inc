<?php
/**
 * @file
 * Commerce Checkout integration for the accommodation booking process.
 */

/**
 * Checkout form callback for accommodation commerce checkout pane.
 */
function commerce_booking_accommodation_checkout_pane_accommodation_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Check we have enabled accommodation on this event.
  $booking_field = commerce_booking_get_field_name($wrapper->commerce_booking_event->type(), $wrapper->commerce_booking_event->value());
  $booking = $wrapper->commerce_booking_event->{$booking_field}->value();
  if (empty($booking['settings']['accommodation'])) {
    return array();
  }

  // Get a list of all accommodation types.
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'commerce_booking_accom_type');
  $query->fieldCondition('commerce_booking_event', 'entity_type', $wrapper->commerce_booking_event->type());
  $query->fieldCondition('commerce_booking_event', 'entity_id', $wrapper->commerce_booking_event->getIdentifier());
  $query->propertyOrderBy('label');
  $result = $query->execute();

  if (empty($result['commerce_booking_accom_type'])) {
    return array();
  }

  // Build the form.
  $pane_form = array(
    '#attributes' => array(
      'class' => array('commerce-booking-accommodation-selection'),
    ),
  );
  $pane_form['#types'] = array();
  $info = entity_get_info('commerce_booking_accom_type');

  // Find out what we already have.
  foreach ($wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == 'commerce_booking_accommodation') {
      // Figure out whether to count this line item.
      switch ($line_item_wrapper->commerce_line_item_status->value()) {
        case 'pending':
          $key = 'pending';
          break;

        case 'cancelled':
          continue;

        default:
          $key = 'locked';
          break;
      }
      $pane_form['#types'][$line_item_wrapper->cba_accommodation_type->getIdentifier()][$key][$line_item_wrapper->getIdentifier()] = $delta;
    }
  }

  // Loop over the accommodation types adding them to the form.
  foreach (entity_load('commerce_booking_accom_type', array_keys($result['commerce_booking_accom_type'])) as $accom_type) {
    // Process any 'missing' existing variables and calculate totals.
    if (!isset($pane_form['#types'][$accom_type->accommodation_type_id])) {
      $pane_form['#types'][$accom_type->accommodation_type_id] = array();
    }
    $pane_form['#types'][$accom_type->accommodation_type_id] += array(
      'pending' => array(),
      'locked' => array(),
    );
    $pane_form['#types'][$accom_type->accommodation_type_id]['total'] = count($pane_form['#types'][$accom_type->accommodation_type_id], COUNT_RECURSIVE) - 2;

    // If we don't already have the category, create it now.
    if (!isset($pane_form[$accom_type->category])) {
      $pane_form[$accom_type->category] = array(
        '#type' => 'fieldset',
        '#title' => $info['bundles'][$accom_type->category]['label'],
        '#parents' => array($checkout_pane['pane_id']),
      );
    }

    // Get the description.
    $desc = '';
    if (!empty($accom_type->commerce_booking_accom_desc[LANGUAGE_NONE][0])) {
      $item = $accom_type->commerce_booking_accom_desc[LANGUAGE_NONE][0];
      $desc = text_summary($item['value'], $item['format']);
    }

    $pane_form[$accom_type->category][$accom_type->accommodation_type_id] = array(
      '#type' => 'select',
      '#title' => $accom_type->label,
      '#title_display' => 'after',
      '#description' => $desc,
      '#options' => drupal_map_assoc(range(count($pane_form['#types'][$accom_type->accommodation_type_id]['locked']), 50)),
      '#default_value' => $pane_form['#types'][$accom_type->accommodation_type_id]['total'],
      '#accommodation_type' => $accom_type,
    );
  }

  return $pane_form;
}

/**
 * Checkout form submit callback for accommodation commerce checkout pane.
 *
 * Set the booking status to booking_booker_info.
 */
function commerce_booking_accommodation_checkout_pane_accommodation_checkout_form_validate(&$form, &$form_state, $checkout_pane, $order) {
  // Check we have the accommodation form.
  if (!isset($form[$checkout_pane['pane_id']])) {
    return TRUE;
  }

  // Make sure our desired accommodation doesn't involve removing locked items.
  $return = TRUE;
  foreach ($form[$checkout_pane['pane_id']]['#types'] as $accom_type_id => $existing) {
    if ($form_state['values'][$checkout_pane['pane_id']][$accom_type_id] < count($existing['locked'])) {
      $return = FALSE;
      form_error($form[$checkout_pane['pane_id']], t('You cannot remove paid for accommodation.'));
    }
  }

  return $return;
}

/**
 * Checkout form submit callback for accommodation commerce checkout pane.
 *
 * Set the booking status to booking_booker_info.
 */
function commerce_booking_accommodation_checkout_pane_accommodation_checkout_form_submit(&$form, &$form_state, $checkout_pane, $order) {
  // Check we have the accommodation form.
  if (!isset($form[$checkout_pane['pane_id']])) {
    return;
  }

  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Loop over our accommodation types.
  foreach ($form[$checkout_pane['pane_id']]['#types'] as $accom_type_id => $existing) {
    $count = $form_state['values'][$checkout_pane['pane_id']][$accom_type_id];
    $accom_type_wrapper = entity_metadata_wrapper('commerce_booking_accom_type', $accom_type_id);

    // If we want the same as we had we are all done.
    if ($count == $existing['total']) {
      continue;
    }
    // If we want less, remove some pending line items.
    elseif ($count < $existing['total']) {
      // Delete until will have the right number or no more deletable items.
      while ($count < $existing['total'] && count($existing['pending']) > 0) {
        // Get the delta and line item wrapper.
        $delta = array_pop($existing['pending']);
        $line_item = $wrapper->commerce_line_items->offsetGet($delta);

        // Remove and delete the line item.
        $wrapper->commerce_line_items->offsetUnset($delta);
        $line_item->delete();
        unset($existing['pending'][$line_item->getIdentifier()]);

        // Reduce our count.
        $existing['total']--;
      }
    }
    // If we want more, add some line items.
    elseif ($count > $existing['total']) {
      // Delete until will have the right number or no more deletable items.
      while ($count > $existing['total']) {
        $line_item = commerce_line_item_new('commerce_booking_accommodation', $order->order_id);
        $line_item->quantity = 1;
        $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
        $line_item_wrapper->cba_accommodation_type = $accom_type_id;
        $line_item_wrapper->line_item_label = 'Accommodation: ' . $accom_type_wrapper->label();
        $line_item_wrapper->save();
        $wrapper->commerce_line_items[] = $line_item;

        // Increase our count.
        $existing['total']++;
      }
    }
  }
}
