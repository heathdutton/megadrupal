<?php
/*
 * @file hotel_booking.token.inc
 * Token hooks for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 * 
 */

/**
* Implements hook_token_list().
* Provides tokens for hotel booking pane and check in/out dates
*/
function hotel_booking_token_list($type = 'site') {
  $tokens = array();
  if ($type == 'order' || $type == 'all') {

    $tokens['order']['bookings-pane'] = t('Hotel bookings pane');
    $tokens['order']['check-out-date'] = t('Check out date (latest if more than one');
    $tokens['order']['check-in-date'] = t('Check in date (earliest if more than one');
    
  }

  return $tokens;
  
}


/**
* Implements hook_token_values().
* Evaluate the token values
*/
function hotel_booking_token_values($type, $object = NULL, $options = array()) {
  $tokens = array();
 
  if ($type == 'order') {   
    $order = $object;
    $order->as_template = TRUE; //notify booking pane that this item is being shown in a template
        
    $tokens['bookings-pane'] = hotel_booking_booking_pane('customer', $order);
    $tokens['check-in-date'] = NULL;
    $tokens['check-out-date'] = NULL;
    
    $dates_exist = FALSE;
    if ($order->products && is_array($order->products)) {
      $first = TRUE;
      $first_check_in = $last_check_out = FALSE;
      foreach ($order->products as $product) {
        if ($product->data['module'] == 'hotel_booking') {
          list($check_in, $check_out) = _hotel_booking_check_in_out($product->data['nights']);
          //have we had 1 pass yet?
          if ($first) {
            $first_check_in = $check_in;
            $last_check_out = $check_out;
            $first = FALSE;
            $dates_exist = TRUE;
            continue; //next loop
          }
          if ((int)$last_check_out->format('U') < (int)$check_out->format('U')) {
            //this check out is later
            $last_check_out = $check_out;
          }
          if ((int)$first_check_in->format('U') > (int)$check_in->format('U')) {
            //this check in is eariler
            $first_check_in = $check_in;
          }
        }
      }
    }
    if ($dates_exist) {
      $format = variable_get('uc_date_format_default', 'd M Y');
      $tokens['check-in-date'] = $first_check_in->format($format);
      $tokens['check-out-date'] = $last_check_out->format($format);
    }
  }
  
  return $tokens;
}