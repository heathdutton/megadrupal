<?php

/**
 * @file
 * Documentation for all the receipting sub-systems
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Register a payment gateway.
 *
 * @return
 *  Return the configuration of the payment gateway.
 */
function hook_receipt_info() {

}

/**
 * Initialize the selected receipting payment type.
 *
 * Set up anything that is required by the payment gateway. This can be called
 * multiple times so check that you have not already done the setup.
 *
 * @param $type
 *  The type of the object which is used when calling the allocation
 *  interface.
 * @param $object
 *  The object is used in conjunction with the allocation interface to get
 *  additional information from the allocation system.
 */
function hook_receipt_init($type, $object) {

}

/**
 * Generation a form for the collection of the payment
 *
 * The form that is generated to collect all the information required to
 * collect the payment.
 *
 * @param $type
 *  The type of the object which is used when calling the allocation
 *  interface.
 * @param $object
 *  The object is used in conjunction with the allocation interface to get
 *  additional information from the allocation system.
 */
function hook_receipt_payment_form($type, $object) {

}

/**
 * Interface with the payment gateway to collect the payment.
 *
 * This is used when you need use when you interface with the payment gateway
 * via an API in which the payment information can be sent to the payment
 * gateway when it will be processed and then the result will be passed back.
 * The receipt needs to be updated with the result of the payment.
 *
 * @param $receipt
 *  A receipt that has been generated that needs to be updated with the result
 *  of the payment.
 * @param $type
 *  The type of the object which is used when calling the allocation
 *  interface.
 * @param $object
 *  The object is used in conjunction with the allocation interface to get
 *  additional information from the allocation system.
 */
function hook_receipt_process_payment($receipt, $type, $object) {

}

/**
 * Build the url to redirect to a hosted payment page.
 *
 * This is to be used with hosted payment pages where the customer is
 * redirected to the payment gateways site to make the payment. The actual
 * payment results need to be updated outside of this interface.
 *
 * @param $receipt
 *  A receipt that has been generated that needs to be updates with the result
 *  of the payment.
 * @param $type
 *  The type of the object which is used when calling the allocation
 *  interface.
 * @param $object
 *  The object is used in conjunction with the allocation interface to get
 *  additional information from the allocation system.
 *
 * @return
 *  A URL which is compatible with drupal_goto()
 */
function hook_receipt_payment_url($receipt, $type, $object) {

}

/**
 * Allow external processes to influence where the system send the user when they have completed their order.
 *
 * @param $url
 *  The url of the return page which can be altered by other systems.
 *
 * @param $receipt
 *  If the receipt object is know the process can use this to further the redirect.
 */
function hook_ec_receipt_return_page_alter(&$url, $receipt = NULL) {
  $url = array('store/receipt', 'param=complete');
}

/**
 * Allow external processes to influence where the system send the user when they have cancelled their order from the payment gateway.
 *
 * @param $url
 *  The url of the return page which can be altered by other systems.
 *
 * @param $receipt
 *  If the receipt object is know the process can use this to further the redirect.
 */
function hook_ec_receipt_cancel_page_alter(&$url, $receipt = NULL) {
  $url = array('store/receipt', 'param=cancel');
}

/**
 * Allow filtering which payment gateways are available.
 *
 * @param $rtype
 *  Payment gateway type
 * @param $type
 *  Which transaction type.
 * @param $object
 *  Transaction object which can be past via the allocation interface.
 *
 * @return
 *  Return a true or false to indicate if this payment gateway should be included.
 *  NULL will be considered TRUE
 */
function hook_ec_receipt_type_filter($rtype, $type, $object) {
  // Don't allow paypal basic to be a valid payment gateway.
  if ($rtype == 'ec_paypal_basic') {
    return FALSE;
  }
}

/**
 * @} End of "addtogroup hooks".
 */

