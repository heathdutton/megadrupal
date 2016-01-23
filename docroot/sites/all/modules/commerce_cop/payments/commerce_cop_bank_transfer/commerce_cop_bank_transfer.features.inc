<?php
/**
 * @file
 * commerce_cop_bank_transfer.features.inc
 */

/**
 * Implements hook_commerce_custom_offline_payments().
 */
function commerce_cop_bank_transfer_commerce_custom_offline_payments() {
  $items = array(
    'bank_transfer' => array(
      'id' => 'bank_transfer',
      'title' => 'Bank Transfer',
      'description' => 'Receive direct money transfers initiated by the customer to your bank account.',
      'information' => '<p>
        <strong>Please make payment to:</strong><br />
        Account owner: <br />
        Account number: <br />
        IBAN: <br />
        Bank code: <br />
        SWIFT: <br />
        Banking institution: <br />
        Branch office: <br />
        <br />
        Payment instructions: <br />
        </p>',
      'format' => 'filtered_html',
      'status' => 1,
      'checkout' => 1,
      'terminal' => 1,
      'fieldable' => 1,
    ),
  );
  return $items;
}
