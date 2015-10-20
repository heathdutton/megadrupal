<?php

/**
 * As the Payment API can be used with a wide variety of forms, this module
 * isn't able to add the full range of necessary data fields to the page by
 * itself. It depends on the modules implementing the individual Pay Forms to
 * fill in fields such as the customer's address and products purchased. Two
 * hooks are provided for this purpose:
 *   hook_pay_googleanalytics_transaction_alter()
 *   hook_pay_googleanalytics_products_alter()
 */

/**
 * Allows modules to alter transaction info before it's added to the Google
 * Analytics e-commerce tracking code.
 *
 * The Pay Google Analytics module works in much the same way as the Ubercart
 * Google Analytics module by constructing function calls that work through
 * the Google Analytics JS API to report order information for e-commerce
 * tracking purposes.  The module builds the argument list for the transaction
 * and uses this hook to give other modules a chance to alter what gets reported
 * to Google Analytics.
 *
 * @param $trans
 *   An array of arguments being passed to Google Analytics representing the
 *     transaction, including pxid (pay transaction id), store (defaults to site
 *     name), total, tax, shipping, city, state and country.  Only pxid, store
 *     and total are set by the pay_googleanalytics module and you will need to
 *     implement this hook in order to set the other values based on your form
 *     implementation.
 * @param $transaction
 *   The pay transaction object being reported to Google Analytics.
 * @param $activity
 *   The pay activity object being reported to Google Analytics.
 * @return
 *   Nothing should be returned. Hook implementations should receive the $trans
 *     array by reference and alter it directly.
 */
function hook_pay_googleanalytics_transaction_alter(&$trans, $transaction, $activity) {

  // Fetch the location fields of your custom form fields, for example:
  $record = db_fetch_object(db_query("SELECT city, state, country FROM {my_form_data} WHERE txid = %d", $transaction->pxid));

  // Set city, state and country.
  $trans['city'] = $record->city;
  $trans['state'] = $record->state;
  $trans['country'] = $record->country;
}

/**
 * Allow modules to add products to the Google Analytics e-commerce tracking
 * code. Should add product objects to the $products array.
 *
 * The Pay Google Analytics module works in much the same way as the Ubercart
 * Google Analytics module by constructing function calls that work through
 * the Google Analytics JS API to report purchased items for e-commerce tracking
 * purposes.  
 *
 * @param $products
 *   An array of product objects being passed to Google Analytics each
 *   representing an item on the order. Each product object has the  elements:
 *   product_id, title, category, price, quantity.
 * @param $transaction
 *   The pay transaction object being reported to Google Analytics.
 * @param $activity
 *   The pay activity object being reported to Google Analytics.
 * @return
 *   Nothing should be returned. Hook implementations should receive the $item
 *     array by reference and alter it directly.
 */
function hook_pay_googleanalytics_products_alter(&$products, $transaction, $activity) {
  // Fetch your product data and then set up an object similar to this one.
  $product = new stdClass;
  $product->product_id = 'PRDID';
  $product->title = 'My product title';
  $product->price = $transaction->total;
  $product->quantity = 1;
  $product->price = 100.00;
  $product->category = t('No category');

  // Add to the $products array.
  $products[] = $product;
}

