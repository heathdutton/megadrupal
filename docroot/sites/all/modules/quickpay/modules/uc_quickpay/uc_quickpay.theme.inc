<?php
/**
 * @file
 * The theme functions connected to the uc_quickpay module.
 */

/**
 * The theme function for the ubercart payment tab.
 *
 * This is stolen from the original ubercart theme_ubercart_by_order_form().
 * All we do is add the extra form data, which we've added in the
 * uc_quickpay_form_uc_payment_by_order_form_alter() hook.
 *
 * @see uc_quickpay_form_uc_payment_by_order_form_alter()
 * @see theme_uc_payment_by_order_form()
 */
function theme_uc_quickpay_by_order_form($variables) {
  $form = $variables['form'];

  $output = '<p><strong>' . t('Order total:') . '</strong> ' .
    drupal_render($form['order_total']) . '<br /><strong>' .
    t('Current balance:') . '</strong> ' .
    drupal_render($form['balance']) . '</p>';
  $output .= '<p>' . drupal_render($form['payments']) . '</p><p>' .
    drupal_render($form['form_id']) .
    drupal_render($form['form_token']) . '</p>';
  $output .= drupal_render($form['uc_quickpay']);

  return $output;
}

/**
 * The theme function for the authorized messaged written in the ubercart order
 * payment tab.
 */
function theme_uc_quickpay_authorized_comment($variables) {
  $txn = $variables['txn'];
  return $txn->amount() . ' ' . $txn->currency() . ' Authorized.<br/>Transaction ID: ' . $txn->transaction;
}

/**
 * The theme function for the capture messaged written in the ubercart order
 * payment tab.
 */
function theme_uc_quickpay_captured_comment($variables) {
  $txn = $variables['txn'];
  return $txn->amount() . ' ' . $txn->currency() . ' Captured.<br/>Transaction ID: ' . $txn->transaction;
}

/**
 * The theme function for the cancel messaged written in the ubercart order
 * payment tab.
 */
function theme_uc_quickpay_canceled_comment($variables) {
  $txn = $variables['txn'];
  return 'Canceled.<br/>Transaction ID: ' . $txn->transaction;
}

/**
 * The theme function for the refund messaged written in the ubercart order
 * payment tab.
 */
function theme_uc_quickpay_refunded_comment($variables) {
  $txn = $variables['txn'];
  return $txn->amount() . ' ' . $txn->currency() . ' Refunded.<br/>Transaction ID: ' . $txn->transaction;
}
