<?php
/*
 * @file hotel_booking.rules_defaults.inc
 * Provides default rules for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */


/**
* Implements hook_ca_predicate().
*/
function hotel_booking_default_rules_configuration() {
  // Alter the availability when a payment is received
  // and the order is paid in full
  $rule = rules_reaction_rule();
  $rule->label = t('Reduce room availability status on full payment');
  $rule->active = TRUE;
  $rule->event('uc_payment_entered')
    ->condition('uc_payment_condition_order_balance', array(
      'order:select' => 'order',
      'balance_comparison' => 'less_equal',
    ))
    ->action('hotel_booking_reduce_availability', array(
      'order:select' => 'order',
    ));


  $configurations['hotel_booking_payment_received'] = $rule;

   // Increase availability in case of cancellation
   $rule = rules_reaction_rule();
  $rule->label = t('Increase room availability on order cancellation.');
  $rule->active = TRUE;
  $rule->event('uc_order_status_update')
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'canceled')))
    ->action('hotel_booking_increase_availability', array(
      'order:select' => 'order'
    ));

  $configurations['hotel_booking_order_cancelled'] = $rule;

  return $configurations;
}