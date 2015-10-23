<?php
/**
 * @file
 * Pages for commerce booking accommodation.
 */

/**
 * Page callback for cancelling line items.
 */
function commerce_booking_accommodation_cancel_line_item($js, $line_item = NULL) {
  if (!$line_item) {
    return MENU_NOT_FOUND;
  }

  if ($js) {
    // This is in a modal, so let's do the modal magic.
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'build_info' => array(
        'args' => array($line_item),
      ),
    );
    $commands = ctools_modal_form_wrapper('commerce_booking_accommodation_cancel_line_item_form', $form_state);

    if (!empty($form_state['executed'])) {
      // The form has been executed, so let's redirect to the destination page.
      $commands = array();
      $commands[] = ctools_ajax_command_reload();
      $commands[] = ctools_modal_command_dismiss();
    }

    print ajax_render($commands);
    ajax_footer();
    return;
  }
  else {
    // This is just a page, so we don't need to worry.
    return drupal_get_form('commerce_booking_accommodation_cancel_line_item_form', $line_item);
  }
}

/**
 * Form constructor for the cance line item form.
 *
 * @param stdClass $line_item
 *   A commerce_line_item to cancel.
 *
 * @see commerce_booking_cancel_refund_ticket_form_confirm()
 */
function commerce_booking_accommodation_cancel_line_item_form($form, &$form_state, $line_item) {
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $order = $line_item_wrapper->order->value();

  $form['#line_item'] = $line_item;
  $form['#order']
    = $form_state['order'] = $order;

  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array('#markup' => t('Are you sure you want to cancel this accommodation?'));
  if (user_access('administer tickets')) {
    $form['description']['#markup'] .= ' ' . t('You will have to refund any payments seperately.');
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel Accommodation'),
    '#submit' => array(
      'commerce_booking_accommodation_cancel_line_item_form_confirm',
    ),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#submit' => array(),
  );
  // By default, render the form using theme_confirm_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'confirm_form';
  }

  return $form;
}

/**
 * Form submission handler for commerce_booking_cancel_refund_ticket_form().
 */
function commerce_booking_accommodation_cancel_line_item_form_confirm(&$form, &$form_state) {
  $line_item = $form['#line_item'];
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $line_item_wrapper->commerce_line_item_status = 'cancelled';

  // Get hold of the event.
  $event_wrapper = $line_item_wrapper->cba_accommodation_type->commerce_booking_event;
  $event = $event_wrapper->value();
  $field_name = commerce_booking_get_field_name($event_wrapper->type(), $event_wrapper->value());

  // Find out the value based off of event deposit and payments on the ticket.
  $instance = field_info_instance($event_wrapper->type(), $field_name, $event_wrapper->getBundle());
  $deposit_field = $instance['settings']['deposit_field'];
  $deposit = !empty($event->{$deposit_field}[LANGUAGE_NONE][0]) ? $event->{$deposit_field}[LANGUAGE_NONE][0]: array('amount' => 0);
  $paid = $line_item_wrapper->paid->value();

  // If either is zero, we don't need to worry about conversion.
  if ($deposit['amount'] == 0 || $paid['amount'] == 0) {
    $line_item->commerce_unit_price[LANGUAGE_NONE][0]['amount'] = 0;
    $line_item->commerce_total[LANGUAGE_NONE][0]['amount'] = 0;
  }
  // Otherwise convert and find the minumum.
  else {
    $paid_amount = commerce_currency_convert($paid['amount'], $paid['currency_code'], $deposit['currency_code']);
    $line_item->commerce_unit_price[LANGUAGE_NONE][0] = array(
      'amount' => min($deposit['amount'], $paid_amount),
      'currency_code' => $deposit['currency_code'],
    );
    $line_item->commerce_total[LANGUAGE_NONE][0] = array(
      'amount' => min($deposit['amount'], $paid_amount),
      'currency_code' => $deposit['currency_code'],
    );
  }

  $line_item_wrapper->save();

  // Save the order to update the total.
  commerce_order_save($form['#order']);
}