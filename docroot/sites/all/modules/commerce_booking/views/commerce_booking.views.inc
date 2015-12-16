<?php
/**
 * @file
 * Views integration for commerce booking tickets.
 */

/**
 * Implements hook_views_data().
 */
function commerce_booking_views_data_alter(&$data) {
  $ticket_fields = &$data['commerce_booking_ticket'];

  // Set up the confirmed date.
  $ticket_fields['confirmed']['field']['handler'] = 'views_handler_field_date';
  $ticket_fields['confirmed']['sort']['handler'] = 'views_handler_sort_date';
  $ticket_fields['confirmed']['filter']['handler'] = 'views_handler_filter_date';

  // Field for ticket balance.
  $ticket_fields['balance'] = array(
    'title' => t('Balance'),
    'help' => t('Show the ticket balance.'),
    'field' => array(
      'real field' => 'ticket_id',
      'handler' => 'commerce_booking_ticket_ticket_balance',
    ),
  );

  // Field for ticket amount paid.
  $ticket_fields['amount_paid'] = array(
    'title' => t('Amount Paid'),
    'help' => t('Show the amount paid towards tickets.'),
    'field' => array(
      'real field' => 'ticket_id',
      'handler' => 'commerce_booking_ticket_ticket_amount_paid',
    ),
  );

  // Ticket edit modal link.
  $ticket_fields['edit_modal'] = array(
    'title' => t('Edit Link (Modal)'),
    'help' => t('Show a link to the ticket edit modal.'),
    'field' => array(
      'real field' => 'ticket_id',
      'handler' => 'commerce_booking_ticket_ticket_edit_link_modal',
    ),
  );

  // Ticket cancel_refund modal link.
  $ticket_fields['cancel_refund_modal'] = array(
    'title' => t('Cancel & Refund Link (Modal)'),
    'help' => t('Show a link to the ticket cancel & refund modal.'),
    'field' => array(
      'real field' => 'ticket_id',
      'handler' => 'commerce_booking_ticket_ticket_cancel_refund_link_modal',
    ),
  );

  // Ticket cancel_refund modal link.
  $ticket_fields['move_modal'] = array(
    'title' => t('Move Link (Modal)'),
    'help' => t('Show a link to the ticket move modal.'),
    'field' => array(
      'real field' => 'ticket_id',
      'handler' => 'commerce_booking_ticket_ticket_move_link_modal',
    ),
  );
}

/**
 * Implements hook_views_plugins().
 */
function commerce_booking_views_plugins() {
  $plugins['display']['event_subpage'] = array(
    'title' => t('Event sub-page'),
    'help' => t('Display the view as a sub-page of an event.'),
    'handler' => 'commerce_booking_views_plugin_display_event_subpage',
    'parent' => 'views_plugin_display_page',
    'theme' => 'views_view',
    'uses hook menu' => TRUE,
    'contextual links locations' => array('page'),
    'use ajax' => TRUE,
    'use pager' => TRUE,
    'use more' => TRUE,
    'accept attachments' => TRUE,
    'admin' => t('Event sub-page'),
  );

  return $plugins;
}
