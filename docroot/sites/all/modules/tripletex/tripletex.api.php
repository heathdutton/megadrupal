<?php
/**
 * API description to the Tripletex Drupal module.
 *
 * This file describes the various ways the module may be interfaced to from
 * other modules.
 *
 * In case of any difficulties, oddities and errors, please add your request to the
 * module issue queue at https://drupal.org/project/issues/tripletex
 *
 * Author: Sten Johnsen (steoj)
 *
 */


/**
 * Hooks - available in the module
 * To call in your module code, creat a function and name it mymodule_hookfunction()
 * where <mymodule> is the name of your module, and <hookfunction> is the name of the
 * hook you are calling. Basically replace the word "hook" in the names of the functions
 * with your module name to have the function called by the Tripletex module
 *
 * See: http://api.drupal.org/api/drupal/includes!module.inc/group/hooks/7 and
 * https://drupal.org/node/292 for more information
 */

/**
 * Add tripletex invoice
 *
 * Hook is called just before a new invoice is created and the call is made to the Tripletex
 * server to create it.
 *
 * @param structure $invoice
 *   Invoice array
 */
function hook_tripletex_add_invoice(&$invoice_structure) {

  // called by:
  drupal_alter('tripletex_add_invoice', $invoice);
}

function hook_tripletex_add_invoice_failed($invoice, $error) {

  // called by:
  module_invoke_all('tripletex_add_invoice_failed', $invoice, $error);

}

function hook_tripletex_add_invoice_success($invoice, $result) {

}

function hook_tripletex_cancel_invoice(&$invoice) {

}


/**
* Implements hook_tripletex_invoice_update()
* Called when an invoice is updated
*
* Example below from tripletex_commerce
*
* @param int $invoice_id
*/
function hook_tripletex_invoice_update($invoice_id) {

  $invoice = _tripletex_load_invoice($invoice_id);
  $order_id = (int) _tripletex_commerce_get_order_mapping($invoice_id);

  if ($order_id && is_int($order_id)) {
    // If there is an Commerce order with this invoice
    $order = commerce_order_load($order_id);
    $method = array_shift(tripletex_commerce_commerce_payment_method_info());

    // Check if already paid if neccessary?
    $balance = commerce_payment_order_balance($order);
    if ($invoice->remaining_amount == 0) {
      // If the order still has a balance - this is now settled
      if ($balance && $balance['amount'] > 0) {
        $charge = array('amount' => $balance['amount'], 'currency_code' => $invoice->currency);
        tripletex_commerce_transaction($method, $order, $charge, t('Invoice #@invoice paid in full via bank.'), array('@invoice' => $invoice_id));
      }
    }
    else {  // Still remaining on this invoice. Check if balance is same
      if ($balance && ($invoice->remaining_amount * 100 < $balance['amount'])) {
        $charge = array('amount' => ($balance['amount'] - ($invoice->remaining_amount * 100)), 'currency_code' => $invoice->currency);
        tripletex_commerce_transaction($method, $order, $charge, t('Invoice #@invoice paid in part via bank.'), array('@invoice' => $invoice_id));
      }
    }
  }
}

/**
 * API Fuctions
 */

/**
 * CREATE INVOICE
 *
 * Create and send an invoice to a customer
 *
 * @param array $customer
 * 		Structured array containing:
 *      'mail'				Customer email	*
 *      'first_name'	First name			*
 *      'last_name'		Last name
 *      'billto' 			Billing address
 *      'street1'			First street name	*
 *      'street2'			Second street name
 *      'city'				City	*
 *      'state'				State or region
 *      'zip'					Zip code
 *      'country'			Country *
 *      'phone'				Phone number
 *        (* = required)
 *
 * @param array $products
 *    Sructured array containing all products to be invoiced
 *    array(
 *    	array(
 *    		name,
 *    		count,
 *    		unit_price
 *    	)
 *    )
 *
 *  @param array $extra
 *     node and user id's of connected to the submission causing this invoice.
 *     array(nid, uid)
 *
 *  @return int invoice_id
 *     Newly created invoice ID if success. 0 if no invoice is created.
 */
function tripletex_add_invoice($customer, $products, $extra) {dd(array(), 'at the wrong place');}
function tripletex_cancel_invoice($invoice_id) {}
function tripletex_get_invoice_status($invoice_id) {}
function tripletex_get_invoice_document($invoice_id) {}

