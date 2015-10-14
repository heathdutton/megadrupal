<?php

/**
 * @file
 * Documentation for the hooks provided by the epay module.
 */

/**
 * Makes it possible for other modules to implement the ePay payment gateway API.
 *
 * @param $op
 *   What kind of action is being performed. Possible values:
 *   - "info": Used to get information about the modules that implements the epayapi.
 *   - "transaction_cancel": Executed when a transaction is cancelled by the user.
 *      For example used to make a module specific redirect after the transaction is
 *      cancelled.
 *   - "transaction_accept": Executed when a transaction is accepted.
 *      For example used to make a module specific redirect after the transaction is
 *      accepted
 *   - "transaction_callback": Executed after the hidden callback from ePay when payment
 *      is completed, but before the user is redirected.
 * @param $delta
 *   Which ePay implementation to return.
 *   Although it is most commonly an integer starting at 0, this is not mandatory.
 * @param &$transaction
 *   An array with all the information about the transaction.
 * @param $a3
 *  - Not used at the moment.
 * @param $a4
 *  - Not used at the moment.
 * @return
 *   This varies depending on the operation.
 *   - The "transaction_cancel", "transaction_accept" and "transaction_callback"
 *     operations have no return value.
 *   - The "info" operation should return an array with info about the DIBS implementations.
 */
function hook_epayapi($op = 'info', $delta = NULL, &$transaction = NULL, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    case 'info':
      $info[0] = array('info' => t('ePay implementation #1.......'));
      $info[1] = array('info' => t('ePay implementation #2.......'));
      if (!empty($delta)) {
        return isset($info[$delta]) ? $info[$delta] : NULL;
      }
      else {
        return $info;
      }
      break;
    case 'transaction_cancel':
      switch ($delta) {
        case 0:
          // You have a few options available in this op.
          // You can set the theme function to be used.
          // You can set a redirect.
          // The redirect will take presedence.
          $transaction['theme'] = 'mymodule_theme_function';
          break;
        case 1:
          $transaction['redirect'] = 'some-path/payment/aborted/2';
          break;
        }
      break;
    case 'transaction_accept':
      switch ($delta) {
        case 0:
          // You have a few options available in this op.
          // You can set the theme function to be used.
          // You can set a redirect.
          // The redirect will take presedence.
          $transaction['theme'] = 'mymodule_theme_function';
          break;
        case 1:
          $transaction['redirect']  = 'some-path/payment/receipt/2';
          break;
        }
      break;
    case 'transaction_callback':
      // Doing some stuff when the payment is completed.
      // For example sending receipt mail storing data or what else is needed.
      break;
  }
}

/**
 * Makes it possible to alter the form array used when redirecting an user to ePay.
 *
 * @param &$form
 *   The form array that can we altered.
 * @param $delta
 *   Which ePay implementation to return.
 *   Although it is most commonly an integer starting at 0, this is not mandatory.
 * @return
 *   No data are returned.
 */
function hook_epayapi_form_alter(&$form, $delta = NULL) {
  switch ($delta) {
    case 0:
        $form['epay_example_field'] = array(
          '#type' => 'hidden',
          '#value' => 'some value',
        );
      break;
    case 1:
      break;
  }
}
