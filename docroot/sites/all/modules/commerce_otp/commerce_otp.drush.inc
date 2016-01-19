<?php
/**
 * @file
 * Drush command for Commerce OTP module to run cron.
 */

/**
 * Implements hook_drush_command().
 */
function commerce_otp_drush_command() {
  return array(
    'commerce-otp-process-transactions' => array(
      'description' => dt('Process unprocessed transactions.'),
      'aliases' => array('otp-cron'),
    ),
  );
}

/**
 * Check unprocessed OTP transactions status.
 */
function drush_commerce_otp_process_transactions() {
  module_load_include('inc', 'commerce_otp', 'commerce_otp.admin');
  commerce_otp_process_transactions();
}
