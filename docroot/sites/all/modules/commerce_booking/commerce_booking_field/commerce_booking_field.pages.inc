<?php
/**
 * @file
 * Pages for the commerce booking field module.
 */

/**
 * Process a book now link being clicked.
 *
 * Prepare a booking entity if one is needed and redirect to the booking
 * checkout. Includes some integration with the opencrm_user module
 * for a smoother booking process.
 */
function commerce_booking_field_book_now_page($entity_type, $entity_id) {
  global $user;

  if ($user->uid) {
    $account = $user;
  }
  elseif (isset($_SESSION['opencrm_user_uid'])) {
    $account = user_load($_SESSION['opencrm_user_uid']);
  }
  else {
    $dest = drupal_get_destination();
    drupal_goto('user/login', array('query' => drupal_get_destination()));
  }

  if (!user_access('can book for events', $account)) {
    drupal_access_denied();
  }

  $entity = entity_load_single($entity_type, $entity_id);

  if (!$entity) {
    return MENU_NOT_FOUND;
  }

  if ($booking = commerce_booking_prepare_booking($entity_type, $entity, $account)) {
    // Figure out where we can direct to. We want as far along as we can get up
    // to the ticket page, so we'll start from the current status or the order
    // and work back to see if we need to go to the ticket page or the current
    // page.
    $redirect_page = '';
    $order_status = commerce_order_status_load($booking->status);
    if ($order_status['state'] == 'checkout') {
      // If we are on or ahead of tickets, redirect there.
      if ($order_status['checkout_page'] == 'tickets') {
        $redirect_page == '/tickets';
      }
      else {
        $status_checkout_page = commerce_checkout_page_load($order_status['checkout_page']);
        $ticket_checkout_page = commerce_checkout_page_load('tickets');

        if ($ticket_checkout_page['weight'] < $status_checkout_page['weight']) {
          $redirect_page = '/tickets';
        }
        // Otherwise redirect to the current page.
        else {
          $redirect_page = '/' . $status_checkout_page['page_id'];
        }
      }
    }

    drupal_goto('checkout/' . $booking->order_id . $redirect_page);
  }
  else {
    $uri = entity_uri($entity_type, $entity);
    drupal_goto($uri['path'], $uri['options']);
  }
}
