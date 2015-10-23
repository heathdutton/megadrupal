<?php

/**
 * @file epay.theme.inc
 * Theme implementations for epay module.
 */

/**
 * Theme function to format the numbers that should be displayed as an amount of money.
 */
function theme_epay_number($number) {
  return number_format($number, 2, ',', '.');
}

/**
 * Preprocess function for the dibs_cancel_page theme
 *
 * @param array $variables
 */
function template_preprocess_epay_decline_page(&$vars) {
  $transaction = $vars['transaction'];
  
  $vars['template_files'][] = 'epay-decline-page-' . $transaction['api_module'];
  $vars['template_files'][] = 'epay-decline-page-' . $transaction['api_module'] . '-' . $transaction['api_delta'];
}

/**
 * Preprocess function for the dibs_accept_page theme
 *
 * @param array $variables
 */
function template_preprocess_epay_accept_page(&$vars) {
  $transaction = $vars['transaction'];
  $vars['template_files'][] = 'epay-decline-page-' . $transaction['api_module'];
  $vars['template_files'][] = 'epay-decline-page-' . $transaction['api_module'] . '-' . $transaction['api_delta'];
}
