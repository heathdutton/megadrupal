<?php

/**
 * @file
 * Hooks provided by the Commerce Klarna module.
 */

/**
 * Alter a Klarna article before it is added to the Klarna object.
 *
 * This will allow a module to set or alter additinal data that is not supported
 * by Commerce Klarna it self. This example will add a discount percentage to
 * the article.
 *
 * @param $order
 *  The wrapped Commerce Order object.
 * @param $line_item
 *  The wrapped Commerce Line item for this article.
 * @param $article.
 *  The configuration for the article that is being modified.
 *
 * @see http://integration.klarna.com/en/api/standard-integration/functions/addarticle
 */
function hook_commerce_klarna_pre_article($order, $line_item, &$article) {
  // Get the discount amount for this line item.
  $discount = $line_item->field_line_item_discount->value();

  if (!$discount) {
    // No discount has been added, exit.
    return;
  }
  
  // Calculate the discount percentage.
  $price = $line_item->commerce_total->value();
  $percentage = ($discount / $price['amount']) * 100;
  
  // Add the discount percentage to the Klarna article.
  $article['discount'] = $percentage;
}

/**
 * Alter a Klarna address before it is added to the Klarna object.
 *
 * Using this, modules are allowed to alter any address configuration before it
 * is saved to the Klarna object.
 *
 * @param $order
 *  The wrapped Commerce Order object.
 * @param $type
 *  The type of the address being modifed. Either 'billing' or 'shipping'.
 * @param $address.
 *  The configuration for the address that is being modified.
 *
 * @see http://integration.klarna.com/en/api/standard-integration/functions/setaddress
 */
function hook_commerce_klarna_pre_address($order, $type, &$address) {
  // No example at this time.
}

/**
 * Alter a Klarna object and it's settings before the transaction is sent.
 *
 * This happens right before the initiation of the transaction, and allows
 * modules to change the transaction settings, or use the Klarna object in any
 * possible way.
 *
 * @param $order
 *  The wrapped Commerce Order object.
 * @param $klarna
 *  The Klarna object.
 * @param $settings.
 *  The settings which holds necessary data to send the transaction.
 *
 * @see http://integration.klarna.com/en/api/standard-integration/functions/addtransaction
 */
function hook_commerce_klarna_pre_transaction($order, $klarna, &$settings) {
  // No example at this time.
}
