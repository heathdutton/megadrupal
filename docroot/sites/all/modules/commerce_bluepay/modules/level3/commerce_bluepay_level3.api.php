<?php

/**
 * @file
 * API for BluePay Level 3 transaction support.
 */

/**
 * Allow other modules to modify Level 3 transaction data.
 *
 * This hook is called per-line item so that a module can override data as it is
 * added to the order. The $data array is fully populated at this point but has
 * not been added to the request. A module can modify any or all of the $data
 * array.
 *
 * @param array $data
 *   An array that contains the following keys:
 *   - product_code: The UPC, SKU, or any other data that uniquely identifies
 *     this product. 12 characters allowed.
 *   - unit cost: The unit price amount of this line item.
 *   - quantity: The quantity of products on this line item.
 *   - description: A description of this item. Up to 26 characters are allowed.
 *   - unit: Level 3 unit of measure field.
 *   - commodity_code: The commodity code for this product.
 *   - tax_amount: The amount of tax on this line item.
 *   - tax_rate: Unknown at this time. Currently set to 0.
 *   - discount: The total discount amount on the order.
 *   - total: Total amount for this line item.
 * @param object $line_item
 *   The commerce_line_item object that is being processed.
 * @param object $order
 *   The commerce_order object that is being considered.
 */
function hook_commerce_bluepay_level3_data_alter(&$data, $line_item, $order) {
  $data['unit'] = 'XCR';
  $data['commodity_code'] = '07301';
}
