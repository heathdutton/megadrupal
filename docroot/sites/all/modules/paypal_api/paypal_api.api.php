<?php
// $Id$

/**
 * Documentation regarding hooks implemented by the PayPal API module.
 */

/**
 * 
 * React to a PayPal transaction event.
 * 
 * This is called after processing a PayPal IPN transaction.
 * 
 * The hook is invoked during the IPN transaction, which means if you want to see values from PayPal, simply
 * examine the $_POST variable.
 * 
 * @param $status
 * The state of the transaction.  Corresponds to the possible values documented in the PayPal IPN documentation.
 * 
 * @param $uid
 * The id of the user doing the transaction
 * 
 * @param $op
 * The operation being purchased:
 *   create, view or instance
 *   
 * @param $bundle
 * The content type being purchased
 * 
 * @param $quantity
 * The current quantity (after this purchase) owned by this user.
 * Note that the value zero means unlimited.
 * NULL means no change.  This will be the case if $status != 'Completed'
 * 
 */
function hook_paypal_payment($status, $uid, $op, $bundle, $nid, $quantity = NULL) {
  
}

/**
 * 
 * Called when creating the Paypal form for payment.
 * 
 * This hook is only called for buttons created by the Paypal API module.
 * 
 * @param $variables
 * Contains the variables that will be inserted into the form.  Add to this array any custom variables you want to add in order
 * to customize your button.
 * 
 * @param $op
 * The operation being paid for: create, view or instance
 * 
 * @param $bundle
 * The type of content being purchased
 * 
 * @param $nid
 * The nid (if this is an instance purchase)
 * 
 * @return
 * The modified $variables array
 */
function hook_paypal_api_presale($variables, $op, $bundle, $nid) {
  // change amount of sale according to my own logic
  $variables['amount'] = mymodule_get_amount($op, $bundle, $nid);

  return $variables;
}