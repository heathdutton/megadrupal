<?php
/**
 * @file
 * 
 */

/**
 * Define CASHNet items.
 */
function hook_webform_cashnet_items() {
  $items = array(
    'test_item' => array(
      'name' => t('Test Item'),
      'desc' => t('A test item.'),
      // The store is the "Merchant Code" string sent to CASHNet to identify
      // the Checkout store to which we're submitting.
      'store' => 'misc',
      // Define common fields amount, gl, and qty, where the value can be one of
      // the following:
      // - TRUE: this field exists and is required (it must have a form field
      //   mapped to it, with info in it when the form is submitted).
      // - string|number: this field exists and has a default value of
      //   VALUE, whatever that is.  E.g. ['amount'] => '100.00'.
      // - FALSE: this field exists and is optional.
      // The amount in USD.
      'amount' => '100.00',
      // The general ledger, which this payment will be credited against.
      'gl' => 'test_ledger',
      // The quantity of this item when purchased (for information purposes
      // only; will not affect the submitted amount, etc.).
      'qty' => 1,
      // Define an optional callback function, invoked when the item is compiled
      // and is about to be added to the submission[] array on its way out to
      // CASHNet.
      'submission callback' => 'test_item_callback',
      'submission callback arguments' => array('abc', 123, 'etc.'),
      // Define the reference fields as an associative array [key] = value,
      // where the key is the reftype defined for the item in CASHNet, and
      // the value matches the description for the common fields (see above).
      // For example, ['refs']['city_g'] => 'Appleton'.
      'refs' => array(
        'addr_g' => FALSE,
        'name_g' => TRUE,
        'city_g' => FALSE,
        'state_g' => FALSE,
        'zip_g' => FALSE,
        'country_g' => FALSE,
        'email_g' => FALSE,
      ),
    ),
  );
  return $items;
}

/**
 * Alter CASHNet items.
 */
function hook_webform_cashnet_items_alter(&$items) {
  // Require email_g for this item to be submitted.
  $items['test_item']['refs']['email_g'] = TRUE;
}

/**
 * Alter the actual compiled CASHNet Item(s) before they are submitted to CASHNet Checkout.
 */
function hook_webform_cashnet_presubmit_items_alter(&$items) {
  foreach ($items as &$item) {
    if ($item['store'] == 'some_value') {
      $item['store'] = 'some_other_store';
    }
  }
}

/**
 * Define per-submission reference values.
 *
 * Each item in CASHNet Checkout can define free-form "reference values." These
 * are NOT guaranteed to exist in the CASHNet store, but if they are defined,
 * then they should only be submitted once per submission. See the CASHNet
 * ePayment eMarket Manual's "Checkout Data Elements" table for more info.
 */
function hook_webform_cashnet_per_submission_reference_values() {
  return array('ORDERNUMBER_G');
}

/**
 * Alter per-submission reference values.
 */
function hook_webform_cashnet_per_submission_reference_values_alter(&$refvals) {
  if (isset($refvals['EMAIL_G'])) {
    unset($refvals['EMAIL_G']);
  }
}

/**
 * Alter the per-submission (as opposed to per-item) values before they are submitted to CASHNet Checkout.
 */
function hook_webform_cashnet_presubmit_shared_values(&$per_submission_values) {
  // Change the signout URL for the submission.
  $per_submission_values['signouturl'] = 'http://example.com/foo/bar/cashnet_signout?a=b';
}

/**
 * Alter the submission, after all items/values are determined, just before the redirection to CASHNet Checkout.
 */
function hook_webform_cashnet_presubmit_submission(&$submission) {
  // Change the effective store of the entire submission.
  $submission['virtual'] = 'foo_bar_store';
}
