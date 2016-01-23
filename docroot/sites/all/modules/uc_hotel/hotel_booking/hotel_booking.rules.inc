<?php
/*
 * @file hotel_booking.rules.inc
 * Rules hooks for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */



/**
 * Implements hook_rules_action_info().
 */
function hotel_booking_rules_action_info() {
  $order_arg = array(
    'type' => 'uc_order',
    'label' => t('Order'),
  );

  $actions['hotel_booking_reduce_availability'] = array(
    'label' => t('Reduce the availability of rooms'),
    'group' => t('Order'),
    'base' => 'hotel_booking_alter_availability',
    'parameter' => array(
      'order' => $order_arg,
    ),
  );
  $actions['hotel_booking_increase_availability'] = array(
    'label' => t('Increase the availability of rooms'),
    'group' => t('Order'),
    'base' => 'hotel_booking_alter_availability',
    'parameter' => array(
      'order' => $order_arg,
    ),
  );

  return $actions;
}

/**
 * Callback for ca action to reduce availability of
 * a room type associated with an order
 * @param $order object uc_order order object
 * @see hotel_booking_ca_action
*/
function hotel_booking_alter_availability($order) {
  switch ($order->order_status) {
    case 'cancelled':
      $message = 'Processing cancelled booking(s)';
      $sign = "+";
      break;
    default:
      $message = 'Processing booking(s)';
      $sign = "-";
  }
  watchdog('hotel booking', $message);
  if (is_array($order->products)) {
    foreach ($order->products as $key => $product) {
      if ($product->data['module'] == 'hotel_booking') {
        $nid = $product->nid;
        $nights = $product->data['nights'];
        drupal_alter('hotel_booking_order_dates', $nights, $sign);
        if (is_array($nights)) {
          foreach ($nights as $night) {
            if (hotel_booking_non_zeroed_date(new DateObject($night, date_default_timezone(), DATE_FORMAT_DATETIME))) {
              $available = db_query("SELECT available FROM
                                    {hotel_booking_availability_calendars}
                                    WHERE nid = :nid
                                    AND calendar_dt = :date",
                                    array(':nid' => $nid,
                                          ':date' => $night))->fetchField();
              if ($sign == '+') {
                $available++;
              }
              else {
                $available--;
              }
              watchdog('hotel booking', t('Room type: @model availability updated for !night, set available to !available',
                                          array('@model' => $product->model, '!night' => $night, '!available' => $available)));
              db_update("hotel_booking_availability_calendars")
              ->fields(array('available' => $available))
              ->condition('nid', $nid)
              ->condition('calendar_dt', $night)
              ->execute();
            }
          }
        }
      }
    }
  }
} // end of hotel_booking_alter_availability()