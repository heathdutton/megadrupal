<?php
/**
 * @file
 * Page callbacks for commerce booking.
 */

/**
 * Page callback for editing tickets.
 */
function commerce_booking_edit_ticket($js, $ticket = NULL, $order = NULL) {
  if (!$ticket && !$order) {
    return MENU_NOT_FOUND;
  }

  if ($js) {
    // This is in a modal, so let's do the modal magic.
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'build_info' => array(
        'args' => array($ticket, $order),
      ),
    );
    $commands = ctools_modal_form_wrapper('commerce_booking_edit_ticket_form', $form_state);

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
    return drupal_get_form('commerce_booking_edit_ticket_form', $ticket, $order);
  }
}

/**
 * Form constructor for the edit ticket form.
 *
 * @param CommerceBookingTicket $ticket
 *   A commerce_booking_ticket if we are editing one.
 * @param stdClass $order
 *   A commerce_order if we are adding a ticket.
 *
 * @see commerce_booking_edit_ticket_form_validate()
 * @see commerce_booking_edit_ticket_form_submit()
 */
function commerce_booking_edit_ticket_form($form, &$form_state, $ticket, $order) {
  // If we are adding a ticket, let's create it.
  if ($order && !$ticket) {
    $ticket = commerce_booking_ticket_create($order);
  }
  // If we are editing, let's pull through the order.
  elseif ($ticket && !$order) {
    if ($line_item = commerce_booking_get_line_item($ticket)) {
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $order = $line_item_wrapper->order->value();
    }
  }

  // If we haven't managed to figure out both, we can't continue.
  if (!$ticket || !$order) {
    return drupal_not_found();
  }

  // Get a lock check.
  $confirmed = !empty($ticket->confirmed);
  $locked = commerce_booking_ticket_is_locked($ticket);
  if ($locked) {
    drupal_set_message(t('This ticket is locked from editing. Changes made will not be saved.'), 'warning');
  }

  $form['#ticket'] = $ticket;
  $form['#order']
    = $form_state['order'] = $order;

  // Attach the ticket form.
  field_attach_form('commerce_booking_ticket', $ticket, $form, $form_state);

  // Add our submit handlers.
  $form['actions'] = array(
    '#type' => 'actions',
    '#weight' => 99,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => !empty($ticket->is_new) ? t('Add Ticket') : t('Update ticket'),
    '#access' => !$locked,
  );
  $form['actions']['submit_confirm'] = array(
    '#type' => 'submit',
    '#value' => !empty($ticket->is_new) ? t('Add & Confirm Ticket') : t('Update & Confirm ticket'),
    '#access' => !$locked && !$confirmed && user_access('administer tickets'),
    '#submit' => array(
      'commerce_booking_edit_ticket_form_submit',
      'commerce_booking_edit_ticket_form_submit_confirm',
    ),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'op',
    '#limit_validation_errors' => array(),
    '#validate' => array(),
    '#submit' => array(),
  );
  return $form;
}

/**
 * Form validation handler for commerce_booking_edit_ticket_form().
 *
 * @see commerce_booking_edit_ticket_form_submit()
 */
function commerce_booking_edit_ticket_form_validate(&$form, &$form_state) {
  field_attach_form_validate('commerce_booking_ticket', $form['#ticket'], $form, $form_state);

  // Check no other tickets with this email exist on this booking.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $form['#order']);
  $this_ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $form['#ticket']);

  // If no email is provided, we don't want to do this.
  if ($this_ticket_wrapper->getPropertyInfo('ticket_holder_email')
    && $this_ticket_wrapper->ticket_holder_email->value()) {
    // Iterate over each line item.
    foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
      // Check we are a relevant line item.
      if ($line_item_wrapper->getBundle() == 'commerce_booking_ticket') {
        // Iterate over each ticket.
        foreach ($line_item_wrapper->commerce_booking_ticket as $ticket_wrapper) {
          // Check the ticket is valid.
          if ($ticket_wrapper->getIdentifier() == $this_ticket_wrapper->getIdentifier()) {
            continue;
          }

          // Check the ticket isn't cancelled.
          if ($ticket_wrapper->status->value() == 'cancelled') {
            continue;
          }

          // Check the email is different.
          if ($ticket_wrapper->ticket_holder_email->value() == $this_ticket_wrapper->ticket_holder_email->value()) {
            form_set_error('ticket_holder_email', t('A ticket with this email has already been added to your booking, if you would like to continue with this ticket please change or remove the email'));
            break;
          }
        }
      }
    }
  }
}

/**
 * Form submission handler for commerce_booking_edit_ticket_form().
 *
 * @see commerce_booking_edit_ticket_form_validate()
 */
function commerce_booking_edit_ticket_form_submit(&$form, &$form_state) {
  $is_new = empty($form['#ticket']->ticket_id);
  $order_wrapper = entity_metadata_wrapper("commerce_order", $form['#order']);

  field_attach_submit('commerce_booking_ticket', $form['#ticket'], $form, $form_state);
  entity_save('commerce_booking_ticket', $form['#ticket']);

  if ($is_new) {
    module_load_include('inc', 'commerce_booking', 'commerce_booking.commerce');

    foreach ($order_wrapper->commerce_line_items as $wrapper) {
      if ($wrapper->getBundle() == 'commerce_booking_ticket') {
        $line_item_wrapper = $wrapper;
        break;
      }
    }

    if (!isset($line_item_wrapper)) {
      $line_item = commerce_line_item_new('commerce_booking_ticket', $order_wrapper->getIdentifier());
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $line_item_wrapper->save();

      // Add the line item to the booking.
      $order_wrapper->commerce_line_items[] = $line_item;
    }

    $line_item_wrapper->commerce_booking_ticket[] = $form['#ticket'];
    commerce_booking_ticket_process($form['#ticket']);
    commerce_booking_line_item_ticket_process($line_item_wrapper->value(), $form['#order']);
    $line_item_wrapper->save();

  }

  // Make sure the order gets updated.
  $order_wrapper->save();
}

/**
 * Form submission handler for commerce_booking_edit_ticket_form().
 *
 * @see commerce_booking_edit_ticket_form_validate()
 */
function commerce_booking_edit_ticket_form_submit_confirm($form, &$form_state) {
  $form['#ticket']->confirm();
}

/**
 * Page callback for cancelling and refunding tickets.
 */
function commerce_booking_cancel_ticket($js, $ticket = NULL) {
  if (!$ticket) {
    return MENU_NOT_FOUND;
  }

  if ($js) {
    // This is in a modal, so let's do the modal magic.
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'build_info' => array(
        'args' => array($ticket),
      ),
    );
    $commands = ctools_modal_form_wrapper('commerce_booking_cancel_ticket_form', $form_state);

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
    return drupal_get_form('commerce_booking_cancel_ticket_form', $ticket);
  }
}

/**
 * Form constructor for the edit ticket form.
 *
 * @param CommerceBookingTicket $ticket
 *   A commerce_booking_ticket to cancel and refund.
 *
 * @see commerce_booking_cancel_refund_ticket_form_confirm()
 */
function commerce_booking_cancel_ticket_form($form, &$form_state, $ticket) {
  $form['#ticket'] = $ticket;

  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array('#markup' => t('Are you sure you want to cancel this ticket?'));
  if (user_access('administer tickets')) {
    $form['description']['#markup'] .= ' ' . t('You will have to refund any payments seperately.');
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel Ticket'),
    '#submit' => array(
      'commerce_booking_cancel_ticket_form_confirm',
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
function commerce_booking_cancel_ticket_form_confirm(&$form, &$form_state) {
  $ticket = $form['#ticket'];
  $ticket_wrapper = entity_metadata_wrapper('commerce_booking_ticket', $ticket);
  $ticket_wrapper->status = 'cancelled';
  $ticket_wrapper->save();

  // Save the order to update the total.
  if ($line_item = commerce_booking_get_line_item($ticket)) {
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $order = $line_item_wrapper->order->value();
  }
  if ($order) {
    commerce_order_save($order);
  }
}

/**
 * Page callback for confirming tickets.
 */
function commerce_booking_confirm_ticket($js, $ticket = NULL) {
  if (!$ticket) {
    return MENU_NOT_FOUND;
  }

  if ($js) {
    // This is in a modal, so let's do the modal magic.
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'build_info' => array(
        'args' => array($ticket),
      ),
    );
    $commands = ctools_modal_form_wrapper('commerce_booking_confirm_ticket_form', $form_state);

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
    return drupal_get_form('commerce_booking_confirm_ticket_form', $ticket);
  }
}

/**
 * Form constructor for the confirm ticket form.
 *
 * @param CommerceBookingTicket $ticket
 *   A commerce_booking_ticket to confirm.
 *
 * @see commerce_booking_confirm_ticket_form_confirm()
 */
function commerce_booking_confirm_ticket_form($form, &$form_state, $ticket) {
  if ($line_item = commerce_booking_get_line_item($ticket)) {
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $order = $line_item_wrapper->order->value();
  }

  $form['#ticket'] = $ticket;
  $form['#order']
    = $form_state['order'] = $order;

  $form['#attributes']['class'][] = 'confirmation';
  $form['description'] = array('#markup' => t('Are you sure you want to confirm this ticket?'));

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm Ticket'),
    '#submit' => array(
      'commerce_booking_confirm_ticket_form_confirm',
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
 * Form submission handler for commerce_booking_confirm_ticket_form().
 */
function commerce_booking_confirm_ticket_form_confirm(&$form, &$form_state) {
  $ticket = $form['#ticket'];
  $ticket->confirm();
}

/**
 * Page callback for cancelling and refunding tickets.
 */
function commerce_booking_move_ticket($js, $ticket = NULL) {
  if (!$ticket) {
    return MENU_NOT_FOUND;
  }

  if ($js) {
    // This is in a modal, so let's do the modal magic.
    ctools_include('ajax');
    ctools_include('modal');

    $form_state = array(
      'ajax' => TRUE,
      'build_info' => array(
        'args' => array($ticket),
      ),
    );
    $commands = ctools_modal_form_wrapper('commerce_booking_move_ticket_form', $form_state);

    if (!empty($form_state['executed'])) {
      // The form has been executed, so let's redirect to the destination page.
      $commands = array();
      $commands[] = ajax_command_invoke('#refresh-ticket-form', 'click');
      $commands[] = ctools_modal_command_dismiss();
    }

    print ajax_render($commands);
    ajax_footer();
    return;
  }
  else {
    // This is just a page, so we don't need to worry.
    return drupal_get_form('commerce_booking_move_ticket_form', $ticket);
  }
}

/**
 * Form constructor for the edit ticket form.
 *
 * @param CommerceBookingTicket $ticket
 *   A commerce_booking_ticket to cancel and refund.
 *
 * @see commerce_booking_cancel_refund_ticket_form_confirm()
 */
function commerce_booking_move_ticket_form($form, &$form_state, $ticket) {
  if ($line_item = commerce_booking_get_line_item($ticket)) {
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $order = $line_item_wrapper->order->value();
  }

  $form['#ticket'] = $ticket;
  $form['#order']
    = $form_state['order'] = $order;

  $form['#attributes']['class'][] = 'confirmation';
  $form['booking'] = array(
    '#type' => 'textfield',
    '#title' => t('New Booking'),
    '#description' => t('Please enter the booking reference of the booking you want to move this ticket to.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Move'),
    '#submit' => array(
      'commerce_booking_move_ticket_form_move',
    ),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
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
function commerce_booking_move_ticket_form_move(&$form, &$form_state) {
  drupal_set_message('This would normally move the ticket to the booking you specified and done something to work out the finances.', 'status');

  // Do some stuff to sort out the payments.
}
