<?php
/**
 * @file
 * Commerce Checkout integration for the booking process.
 */

/**
 * Checkout form callback for terms_conditions commerce checkout pane.
 */
function commerce_booking_checkout_pane_terms_conditions_checkout_form($form, &$form_state, $checkout_pane, $order) {
  // Get the order event entity.
  $event = commerce_booking_get_event_entity($order);
  $event_type = commerce_booking_get_event_entity_type($order);
  list(,,$event_bundle) = entity_extract_ids($event_type, $event);
  $booking_field = commerce_booking_get_field_name($event_type, $event);

  $instance = field_info_instance($event_type, $booking_field, $event_bundle);
  $tc_field = $instance['settings']['tc_field'];

  $pane_form['terms_conditions'] = field_view_field($event_type, $event, $tc_field);

  $pane_form['ts_cs_agree'] = array(
    '#type' => 'checkbox',
    '#title' => t('I have read and agree to the terms and conditions'),
    '#default_value' => !empty($order->commerce_booking_tcs[LANGUAGE_NONE][0]['value']),
    '#disabled' => !empty($order->commerce_booking_tcs[LANGUAGE_NONE][0]['value']),
  );

  return $pane_form;
}

/**
 * Checkout form validate callback for terms_conditions commerce checkout pane.
 */
function commerce_booking_checkout_pane_terms_conditions_checkout_form_validate(&$form, &$form_state, $checkout_pane, $order) {
  if (empty($form_state['values']['terms_conditions']['ts_cs_agree'])) {
    form_error($form['terms_conditions']['ts_cs_agree'], t('Please confirm that you agree to the terms and conditions.'));
    return FALSE;
  }

  return TRUE;
}

/**
 * Checkout form submit callback for terms_conditions commerce checkout pane.
 *
 * Set the booking status to booking_booker_info.
 */
function commerce_booking_checkout_pane_terms_conditions_checkout_form_submit(&$form, &$form_state, $checkout_pane, $order) {
  $order->commerce_booking_tcs[LANGUAGE_NONE][0]['value'] = !empty($form_state['values']['terms_conditions']['ts_cs_agree']);
  commerce_order_save($order);
}

/**
 * Checkout form callback for commerce tickets checkout pane.
 */
function commerce_booking_checkout_pane_tickets_checkout_form(&$form, &$form_state, $checkout_pane, $order) {
  form_load_include($form_state, 'inc', 'commerce_booking', 'commerce_booking.checkout');
  $form['#order'] = $order;
  if (!isset($form_state['current_path'])) {
    $form_state['current_path'] = current_path();
  }

  // Include everything we need for our CTools modals.
  ctools_include('modal');
  ctools_include('ajax');
  ctools_modal_add_js();
  ctools_add_js('ajax-responder');
  drupal_add_js(array(
    'CToolsModal' => array(
      'modalSize' => array(
        'type' => 'fixed',
        'width' => 500,
        'height' => 450,
      ),
    ),
  ), 'setting');

  $pane_form['tickets'] = array(
    '#title' => FALSE,
    '#theme' => 'commerce_booking_ticket_table_form',
    '#header' => array(
      'remove' => t('Remove'),
      'number' => t('Ticket No.'),
      'price' => t('Cost'),
      'status' => t('Status'),
      'operations' => NULL,
    ),
    '#empty' => t('You have no tickets for this booking.'),
    'tickets' => array(),
    '#prefix' => '<div id="checkout-pane-tickets-form-tickets-list">',
    '#suffix' => '</div>',
  );

  $pane_form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 99,
  );

  $pane_form['actions']['tickets_refresh'] = array(
    '#type' => 'submit',
    '#submit' => array(
      'commerce_booking_checkout_pane_tickets_checkout_form_rebuild',
    ),
    '#value' => t('Refresh tickets'),
    '#ajax' => array(
      'callback' => 'commerce_booking_checkout_pane_tickets_checkout_form_additional_tickets_ajax',
      'wrapper' => 'checkout-pane-tickets-form-tickets-list',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
      'progress' => FALSE,
    ),
    '#limit_validation_errors' => array(),
    '#id' => 'refresh-ticket-form',
    '#attributes' => array(
      'style' => 'display:none;',
    ),
  );

  $pane_form['actions']['tickets_remove'] = array(
    '#type' => 'submit',
    '#submit' => array(
      'commerce_booking_checkout_pane_tickets_checkout_form_submit',
      'commerce_booking_checkout_pane_tickets_checkout_form_rebuild',
    ),
    '#value' => t('Remove selected tickets'),
    '#name' => 'tickets_remove',
    '#ajax' => array(
      'callback' => 'commerce_booking_checkout_pane_tickets_checkout_form_additional_tickets_ajax',
      'wrapper' => 'checkout-pane-tickets-form-tickets-list',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
    '#limit_validation_errors' => array(),
  );

  // Get hold of the wrapper so we can jump around.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Show the add a ticket button if booking is open.
  // @TODO: This needs to only show when booking is open.
  $pane_form['actions']['tickets_add'] = array(
    '#markup' => l(t('Add a ticket'), 'commerce_booking/ticket/add/' . $order->order_id . '/nojs', array(
      'attributes' => array(
        'class' => array('ctools-use-modal'),
      ),
      'query' => array(
        'destination' => $form_state['current_path'],
      ),
    )),
  );

  $statuses = commerce_booking_ticket_status_info();

  $count = 0;
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    foreach ($line_item_wrapper->commerce_booking_ticket as $ticket_delta => $ticket_wrapper) {
      if ($ticket_wrapper->value() === NULL) {
        continue;
      }

      $count++;

      // Build our row.
      $pane_form['tickets']['tickets'][$ticket_wrapper->getIdentifier()] = array();
      $row = &$pane_form['tickets']['tickets'][$ticket_wrapper->getIdentifier()];
      $row['#ticket_wrapper'] = $ticket_wrapper;

      $row['remove'] = array(
        '#type' => 'checkbox',
        '#default_value' => FALSE,
        '#parents' => array(
          'tickets',
          'tickets',
          $ticket_wrapper->getIdentifier(),
          'remove',
        ),
        '#disabled' => $ticket_wrapper->status->value() != 'pending',
      );

      $row['number'] = array(
        '#markup' => $count,
      );

      $row['price'] = field_view_field('commerce_booking_ticket', $ticket_wrapper->value(), 'commerce_booking_ticket_price', array(
        'label' => 'hidden',
        'type' => 'commerce_price_formatted_amount',
      ));

      $row['status'] = array(
        '#markup' => $statuses[$ticket_wrapper->status->value()]['label'],
      );

      if ($ticket_wrapper->status->value() == 'pending') {
        $row['operations'] = array(
          '#type' => 'submit',
          '#name' => 'ticket_modal[' . $ticket_wrapper->getIdentifier() . ']',
          '#attributes' => array(
            'class' => array('ctools-use-modal'),
          ),
          '#value' => t('Edit'),
          '#submit' => array('commerce_booking_checkout_pane_ticket_checkout_form_modal_edit'),
        );
        $row['operations'] = array(
          '#markup' => l(t('Edit'), 'commerce_booking/ticket/' . $ticket_wrapper->getIdentifier() . '/edit/nojs', array(
            'attributes' => array(
              'class' => array('ctools-use-modal'),
            ),
            'query' => array(
              'destination' => $form_state['current_path'],
            ),
          )),
        );
      }
      else {
        $row['operations'] = array();
      }
    }
  }

  return $pane_form;
}

/**
 * FAPI submit callback to force a form rebuild on ajax requests.
 */
function commerce_booking_checkout_pane_tickets_checkout_form_rebuild(&$form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Return the form that should have the new number of tickets.
 */
function commerce_booking_checkout_pane_tickets_checkout_form_additional_tickets_ajax($form, $form_state) {
  return $form['tickets']['tickets'];
}

/**
 * Checkout form validate callback for terms_conditions commerce checkout pane.
 */
function commerce_booking_checkout_pane_tickets_checkout_form_validate(&$form, &$form_state, $checkout_pane, $order) {
  if ($errors = form_get_errors()) {
    // Re-write the errors.
    foreach ($errors as $key => $error) {
      if (strpos($key, 'nw_booking_ticket_holder_dob')) {
        foreach ($_SESSION['messages']['error'] as &$message) {
          if ($message == $error) {
            $message = t('We require a date of birth for each person to calculate the appropriate fee.');
          }
        }
        break;
      }
    }

    return FALSE;
  }

  return TRUE;
}

/**
 * Checkout form submit callback for terms_conditions commerce checkout pane.
 */
function commerce_booking_checkout_pane_tickets_checkout_form_submit(&$form, &$form_state, $checkout_pane = NULL, $order = NULL) {
  $order_wrapper = entity_metadata_wrapper("commerce_order", $form['#order']);

  if (!empty($form_state['triggering_element'])) {
    if ($form_state['triggering_element']['#name'] == 'tickets_remove') {
      foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
        // Skip over irrelevant line item wrappers.
        if ($line_item_wrapper->getBundle() != 'commerce_booking_ticket') {
          continue;
        }

        // Get the rows from the table. We're going to assume they're in order.
        foreach ($line_item_wrapper->commerce_booking_ticket as $ticket_wrapper) {
          $ticket = $ticket_wrapper->value();
          $values = &$form_state['input']['tickets']['tickets'][$ticket->ticket_id];

          if (!isset($values['remove']) || $values['remove'] != 1) {
            continue;
          }
          elseif ($ticket_wrapper->status->value() != 'pending') {
            drupal_set_message(t('Tickets cannot be removed once a deposit has been paid.'), 'error');
            continue;
          }

          // Remove the reference.
          foreach ($line_item_wrapper->commerce_booking_ticket as $delta => $item) {
            if ($item->getIdentifier() == $ticket->ticket_id) {
              unset($line_item_wrapper->commerce_booking_ticket[$delta]);
            }
          }
          $line_item_wrapper->save();

          // Delete the ticket.
          entity_delete('commerce_booking_ticket', $ticket_wrapper->getIdentifier());
        }
      }
    }

    $order_wrapper->save();
  }
}

/**
 * Themes the line item manager widget form element.
 */
function theme_commerce_booking_ticket_table_form($variables) {
  drupal_add_css(drupal_get_path('module', 'commerce_line_item') . '/theme/commerce_line_item.admin.css');

  $form = $variables['form'];
  $rows = array();

  // Add each line item to the table.
  foreach (element_children($form['tickets']) as $ticket_id) {
    $row = array();
    foreach (element_children($form['#header']) as $key) {
      $row[] = isset($form['tickets'][$ticket_id][$key]) ? drupal_render($form['tickets'][$ticket_id][$key]) : NULL;
    }
    $rows[] = $row;
  }

  // Setup the table's variables array and build the output.
  $table_variables = array(
    'caption' => check_plain($form['#title']),
    'header' => $form['#header'],
    'rows' => $rows,
    'empty' => $form['#empty'],
  );

  $output = theme('table', $table_variables) . drupal_render($form['actions']);

  return '<div class="commerce-booking-tickets clearfix">' . $output . '</div>';
}

/**
 * Checkout review for tickets.
 */
function commerce_booking_checkout_pane_ticket_payments_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $event_wrapper = $order_wrapper->commerce_booking_event;

  // Get deposit information for the ticket from the commerce_booking field on
  // the event entity.
  $field_name = commerce_booking_get_field_name($event_wrapper->type(), $event_wrapper->value());
  $instance = field_info_instance($event_wrapper->type(), $field_name, $event_wrapper->getBundle());

  if (!empty($instance['settings']['deposit_field'])) {
    $deposit_field = $instance['settings']['deposit_field'];
    $deposit = $event_wrapper->{$deposit_field}->value();
  }

  if (empty($deposit)) {
    $deposit = array(
      'amount' => 0,
      'currency_code' => commerce_default_currency(),
    );
  }

  // Clear this stuff against the order.
  unset($order->data['payment_amount']);
  unset($order->data['ticket_amounts']);

  // Get our various order variables.
  $order_total = $order_wrapper->commerce_order_total->value();
  $order_balance = commerce_payment_order_balance($order);
  $order_paid = $order_total;
  $order_paid['amount'] = $order_total['amount'] - commerce_currency_convert($order_balance['amount'], $order_balance['currency_code'], $order_total['currency_code']);

  // Set up our variables to track.
  $ticket_total = array(
    'amount' => 0,
    'currency_code' => $order_total['currency_code'],
  );
  $custom_total = $ticket_total;
  $ticket_paid = $ticket_total;
  $custom_paid = $ticket_total;
  $custom_balance = $ticket_total;

  // Track our minimum/maximum ticket payments.
  $ticket_minimum = $ticket_total;
  $ticket_maximum = $ticket_total;


  // Get hold of our re-usable variables.
  $ticket_statuses = commerce_booking_ticket_status_info();

  // Start building our form.
  $pane_form['tickets'] = array(
    '#theme' => 'commerce_booking_ticket_table_form',
    '#header' => array(
      'label' => t('Ticket'),
      'status' => t('Status'),
      'price' => t('Price'),
      'outstanding' => t('Outstanding'),
      'payment' => t('Payment'),
    ),
    '#title' => NULL,
    '#empty' => t('There are no ticket in this booking.'),
  );

  // Iterate over all the line items.
  $count = 0;
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    // If it's not a ticket, we can simply add this to our custom amount and
    // continue with the rest of the line items. We deal with the amount paid
    // at the end.
    if (!in_array($line_item_wrapper->getBundle(), array('commerce_booking_ticket', 'commerce_booking_accommodation'))) {
      $custom_total['amount'] += commerce_currency_convert($line_item_wrapper->commerce_total->amount->value(), $line_item_wrapper->commerce_total->currency_code->value(), $custom_total['currency_code']);
      continue;
    }

    if ($line_item_wrapper->getBundle() == 'commerce_booking_ticket') {
      // Iterate over all the tickets on this line item.
      foreach ($line_item_wrapper->commerce_booking_ticket as $ticket_wrapper) {
        // Increase our count for the tickets.
        $count++;

        // Get hold of the ticket price, paid and calculate the minimum and
        // maximum payment values.
        $price = $ticket_wrapper->commerce_booking_ticket_price->value();
        $balance = $ticket_wrapper->balance->value();
        $paid = $ticket_wrapper->paid->value();

        // Add these to our running totals.
        $ticket_total['amount'] += commerce_currency_convert($price['amount'], $price['currency_code'], $ticket_total['currency_code']);
        $ticket_paid['amount'] += commerce_currency_convert($paid['amount'], $paid['currency_code'], $ticket_paid['currency_code']);

        // A deposit needs paying if the event has a deposit amount and the
        // ticket is in a pending status.
        if ($deposit['amount'] > 0 && strpos($ticket_wrapper->status->value(), 'pending') !== FALSE) {
          // Figure out how much deposit needs paying.
          $ticket_deposit = commerce_currency_convert($price['amount'], $price['currency_code'], $deposit['currency_code']) < $deposit['amount'] ? $price: $deposit;
          // And adjust for any amount already paid.
          $ticket_deposit['amount'] -= commerce_currency_convert($paid['amount'], $paid['currency_code'], $ticket_deposit['currency_code']);
          // And make sure it's not less than 0.
          $ticket_deposit['amount'] = max(0, $ticket_deposit['amount']);

          // Add this to our minimum amount.
          $ticket_minimum['amount'] += commerce_currency_convert($ticket_deposit['amount'], $ticket_deposit['currency_code'], $ticket_minimum['currency_code']);
        }
        else {
          $ticket_deposit = $deposit;
          $ticket_deposit['amount'] = 0;
        }

        // Calculate our full amount and add to our running total.
        $ticket_full = $balance;
        $ticket_full['amount'] = max(0, $ticket_full['amount']);
        $ticket_maximum['amount'] += commerce_currency_convert($ticket_full['amount'], $ticket_full['currency_code'], $ticket_maximum['currency_code']);

        // Now render our 'pay per ticket' form.
        $id = $ticket_wrapper->getIdentifier();
        $pay_form = array();

        $pay_form['full_amount'] = array(
          '#type' => 'value',
          '#value' => $ticket_full,
        );
        $pay_form['full'] = array(
          '#type' => 'item',
          '#markup' => commerce_currency_format($ticket_full['amount'], $ticket_full['currency_code']),
        );

        $pay_form['deposit_amount'] = array(
          '#type' => 'value',
          '#value' => $ticket_deposit,
        );

        // If we don't have a single value, add the various elements.
        if ($ticket_full['amount'] != $ticket_deposit['amount']) {
          $pay_form['full']['#states'] = array(
            'visible' => array(
              ':input[name="ticket_payments[payment_options]"]' => array('value' => 'full'),
            ),
          );

          $pay_form['deposit'] = array(
            '#type' => 'item',
            '#markup' => commerce_currency_format($ticket_deposit['amount'], $ticket_deposit['currency_code']),
            '#states' => array(
              'visible' => array(
                ':input[name="ticket_payments[payment_options]"]' => array('value' => 'deposit'),
              ),
            ),
          );

          $pay_form['payment_amount'] = array(
            '#type' => 'textfield',
            '#default_value' => commerce_currency_amount_to_decimal($ticket_full['amount'], $ticket_full['currency_code']),
            '#size' => 10,
            '#field_suffix' => 'GBP',
            '#states' => array(
              'visible' => array(
                ':input[name="ticket_payments[payment_options]"]' => array('value' => (string) 'custom'),
              ),
            ),
          );
        }

        $pane_form['tickets']['tickets'][$id] = array(
          'label' => array('#markup' => t('Ticket #@ticket', array('@ticket' => $count))),
          'status' => array('#markup' => $ticket_statuses[$ticket_wrapper->status->value()]['label']),
          'price' => array('#markup' => empty($price) ? '' : commerce_currency_format($price['amount'], $price['currency_code'])),
          'outstanding' => array('#markup' => empty($balance) ? '' : commerce_currency_format($balance['amount'], $balance['currency_code'])),
          'payment' => $pay_form,
        );
      }
    }
    else {
      // Here we're dealing with accommodation line items.
      // @todo: Move the tickets stuff to be line items based so that we don't have two versions of almost the same code here.
      // Increase our count for the rows.
      $count++;

      // Get hold of the line item price.
      $price = $line_item_wrapper->commerce_total->value();
      $balance = $line_item_wrapper->balance->value();
      $paid = $line_item_wrapper->paid->value();

      // Add these to our running totals.
      $ticket_total['amount'] += commerce_currency_convert($price['amount'], $price['currency_code'], $ticket_total['currency_code']);
      $ticket_paid['amount'] += commerce_currency_convert($paid['amount'], $paid['currency_code'], $ticket_paid['currency_code']);

      // Calculate our full amount and add to our running total.
      $ticket_full = $balance;
      $ticket_full['amount'] = max(0, $ticket_full['amount']);
      $ticket_maximum['amount'] += commerce_currency_convert($ticket_full['amount'], $ticket_full['currency_code'], $ticket_maximum['currency_code']);

      // Now render our 'pay per ticket' form.
      $id = $line_item_wrapper->getIdentifier();
      $pay_form = array();

      $pay_form['full_amount'] = array(
        '#type' => 'value',
        '#value' => $ticket_full,
      );
      $pay_form['full'] = array(
        '#type' => 'item',
        '#markup' => commerce_currency_format($ticket_full['amount'], $ticket_full['currency_code']),
      );

      $pay_form['deposit_amount'] = array(
        '#type' => 'value',
        '#value' => array('amount' => 0, 'currency_code' => $ticket_full['currency_code']),
      );

      $ticket_deposit = array('amount' => 0, 'currency_code' => $ticket_full['currency_code']);

      // If we don't have a single value, add the various elements.
      if ($ticket_full['amount'] != $ticket_deposit['amount']) {
        $pay_form['full']['#states'] = array(
          'visible' => array(
            ':input[name="ticket_payments[payment_options]"]' => array('value' => 'full'),
          ),
        );

        $pay_form['deposit'] = array(
          '#type' => 'item',
          '#markup' => commerce_currency_format($ticket_deposit['amount'], $ticket_deposit['currency_code']),
          '#states' => array(
            'visible' => array(
              ':input[name="ticket_payments[payment_options]"]' => array('value' => 'deposit'),
            ),
          ),
        );

        $pay_form['payment_amount'] = array(
          '#type' => 'textfield',
          '#default_value' => commerce_currency_amount_to_decimal($ticket_full['amount'], $ticket_full['currency_code']),
          '#size' => 10,
          '#field_suffix' => 'GBP',
          '#states' => array(
            'visible' => array(
              ':input[name="ticket_payments[payment_options]"]' => array('value' => (string) 'custom'),
            ),
          ),
        );
      }

      $pane_form['tickets']['tickets']['accommodation_' . $id] = array(
        'label' => array('#markup' => $line_item_wrapper->line_item_label->value()),
        'status' => array('#markup' => $line_item_wrapper->commerce_line_item_status->label()),
        'price' => array('#markup' => empty($price) ? '' : commerce_currency_format($price['amount'], $price['currency_code'])),
        'outstanding' => array('#markup' => empty($balance) ? '' : commerce_currency_format($balance['amount'], $balance['currency_code'])),
        'payment' => $pay_form,
      );
    }
  }

  // Now we have all the ticket amounts, we can figure out how much has been
  // paid towards custom charges and therefore the balance.
  $custom_paid['amount'] = $order_paid['amount'] - $ticket_paid['amount'];
  $custom_balance['amount'] = max(0, $custom_total['amount'] - $custom_paid['amount']);

  // Output additional charge information.
  if ($custom_total['amount'] > 0) {
    $pay_form = array();

    $pay_form['full_amount'] = array(
      '#type' => 'value',
      '#value' => $custom_balance,
    );
    $pay_form['full'] = array(
      '#type' => 'item',
      '#markup' => commerce_currency_format($custom_balance['amount'], $custom_balance['currency_code']),
    );

    $pay_form['deposit_amount'] = array(
      '#type' => 'value',
      '#value' => array('amount' => 0, 'currency_code' => 'GBP'),
    );

    // If we don't have a single value, add the various elements.
    if ($custom_balance['amount'] != $pay_form['deposit_amount']['amount']) {
      $pay_form['full']['#states'] = array(
        'visible' => array(
          ':input[name="ticket_payments[payment_options]"]' => array('value' => 'full'),
        ),
      );

      $pay_form['deposit'] = array(
        '#type' => 'item',
        '#markup' => commerce_currency_format($ticket_deposit['amount'], $ticket_deposit['currency_code']),
        '#states' => array(
          'visible' => array(
            ':input[name="ticket_payments[payment_options]"]' => array('value' => 'deposit'),
          ),
        ),
      );

      $pay_form['payment_amount'] = array(
        '#type' => 'textfield',
        '#default_value' => commerce_currency_amount_to_decimal($custom_balance['amount'], $custom_balance['currency_code']),
        '#size' => 10,
        '#field_suffix' => 'GBP',
        '#states' => array(
          'visible' => array(
            ':input[name="ticket_payments[payment_options]"]' => array('value' => (string) 'custom'),
          ),
        ),
      );
    }

    $pane_form['tickets']['tickets']['custom'] = array(
      'label' => array('#markup' => t('Additional charges')),
      'status' => array('#markup' => $custom_balance['amount'] > 0 ? t('Oustanding'): t('Paid in full')),
      'price' => array('#markup' => commerce_currency_format($custom_total['amount'], $custom_total['currency_code'])),
      'outstanding' => array('#markup' => commerce_currency_format($custom_balance['amount'], $custom_balance['currency_code'])),
      'payment' => $pay_form,
    );
  }

  // Now we can calculate the real minimum and maximum payment amounts.
  $order_minimum = $ticket_minimum;
  $order_maximum = $ticket_maximum;
  $order_maximum['amount'] += $custom_balance['amount'];

  // Set up our values to pass through.
  $pane_form['full_amount'] = array(
    '#type' => 'value',
    '#value' => $order_maximum['amount'],
  );
  $pane_form['minimum_amount'] = array(
    '#type' => 'value',
    '#value' => $order_minimum['amount'],
  );
  $pane_form['custom_amount'] = array(
    '#type' => 'value',
    '#value' => $custom_balance['amount'],
  );

  // Set up our form.
  if ($order_maximum['amount'] == $order_minimum['amount']) {
    // We only have one option so let's not show anything.
    $pane_form['payment_options'] = array(
      '#type' => 'value',
      '#value' => 'full',
    );
  }
  else {
    // Show the choice of payment options.
    $pane_form['payment_options'] = array(
      '#type' => 'radios',
      '#title' => t('Payment options'),
      '#default_value' => 'full',
      '#weight' => -1,
    );

    $placeholders = array(
      '%full' => commerce_currency_format($order_maximum['amount'], $order_maximum['currency_code']),
      '%deposit' => commerce_currency_format($order_minimum['amount'], $order_minimum['currency_code']),
    );

    // If no payment has been made use 'one time' language.
    if ($order_total['amount'] == $order_maximum['amount']) {
      $pane_form['payment_options']['#options']['full'] = t('Pay in full %full', $placeholders);
      if ($order_minimum['amount']) {
        $pane_form['payment_options']['#options']['deposit'] = t('Pay all deposits %deposit', $placeholders);
      }
    }
    // Otherwise deal with 'remaining'.
    else {
      $pane_form['payment_options']['#options']['full'] = t('Pay balance %full', $placeholders);
      if ($order_minimum['amount']) {
        $pane_form['payment_options']['#options']['deposit'] = t('Pay remaining deposits %deposit', $placeholders);
      }
    }

    $pane_form['payment_options']['#options']['custom'] = t('Choose per ticket');
  }

  // Need our validation to run as early as possible.
  form_load_include($form_state, 'inc', 'commerce_booking', 'commerce_booking.checkout');
  $pane_form['#element_validate'][] = 'commerce_booking_checkout_pane_ticket_payments_element_validate';

  return $pane_form;
}

/**
 * Validation callback for checkout review for tickets.
 */
function commerce_booking_checkout_pane_ticket_payments_element_validate($element, &$form_state, $form) {
  $values = &$form_state['values']['ticket_payments'];

  $ticket_amounts = array();
  switch ($values['payment_options']) {
    case 'full':
      $amount = array(
        'amount' => $values['full_amount'],
        'currency_code' => 'GBP',
      );
      foreach ($values['tickets']['tickets'] as $ticket_id => $ticket) {
        $ticket_amounts[$ticket_id] = $ticket['payment']['full_amount'];
      }
      break;

    case 'deposit':
      $amount = array(
        'amount' => $values['minimum_amount'],
        'currency_code' => 'GBP',
      );
      foreach ($values['tickets']['tickets'] as $ticket_id => $ticket) {
        $ticket_amounts[$ticket_id] = $ticket['payment']['deposit_amount'];
      }
      break;

    case 'custom':
      $amount = array(
        'amount' => 0,
        'currency_code' => 'GBP',
      );

      foreach ($values['tickets']['tickets'] as $ticket_id => $ticket) {
        $amount_input = isset($ticket['payment']['payment_amount']) ? $ticket['payment']['payment_amount']: 0;
        $amount_input = commerce_currency_decimal_to_amount($amount_input, $ticket['payment']['full_amount']['currency_code']);

        if ($amount_input < 0) {
          form_set_error('ticket_payments][tickets][tickets][' . $ticket_id . '][payment][payment_amount', t('Please enter a positive amount'));
        }
        elseif ($amount_input < $ticket['payment']['deposit_amount']['amount']) {
          form_set_error('ticket_payments][tickets][tickets][' . $ticket_id . '][payment][payment_amount', t('You must pay at least the deposit on a new ticket.'));
        }
        elseif ($amount_input > $ticket['payment']['full_amount']['amount']) {
          form_set_error('ticket_payments][tickets][tickets][' . $ticket_id . '][payment][payment_amount', t('The amount entered exceeds the value of the ticket.'));
        }

        $amount['amount'] += $amount_input;
        if ($ticket_id != 'custom') {
          $ticket_amounts[$ticket_id] = array(
            'amount' => $amount_input,
            'currency_code' => $ticket['payment']['full_amount']['currency_code'],
          );
        }
      }
      break;
  }

  // Store for the submission handler.
  $form_state['ticket_payments']['payment_amount'] = $amount;
  $form_state['ticket_payments']['ticket_amounts'] = $ticket_amounts;
}

/**
 * Submit callback for checkout review for tickets.
 */
function commerce_booking_checkout_pane_ticket_payments_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  // Store this stuff against the order.
  $order->data['payment_amount'] = $form_state['ticket_payments']['payment_amount'];
  $order->data['ticket_amounts'] = $form_state['ticket_payments']['ticket_amounts'];

  if (isset($form_state['values']['commerce_partial_payment_pane']['amount'])) {
    $form_state['values']['commerce_partial_payment_pane']['amount'] = $order->data['payment_amount'];
  }
}
