<?php
/**
 * Open CRM hook implementations for party commerce booking.
 */

/**
 * Implements hook_opencrm_archive_info().
 */
function party_commerce_booking_opencrm_archive_info($party, $archiving) {
  $items = array();

  if ($archiving) {
    $items['actions']['bookings'] = t('Ensure all future bookings are transferred to a new booking manager.');
    $items['actions']['tickets'] = t('Ensure all future tickets are cancelled.');
  }

  return $items;
}
