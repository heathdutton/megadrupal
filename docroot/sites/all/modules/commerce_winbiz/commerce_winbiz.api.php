<?php

/**
 * Implements hook_econda_customer_data_alter().
 *
 * Use this hook to provide additional information or extend
 * the already gathered information about orders, customerdata etc.
 *
 * For a full list of fieldmappings and their indexes please refer
 * to the WinBIZ manual PDF.
 *
 * @param array $row_data
 * @param object $order
 */
function hook_commerce_winbiz_export_row_alter(&$row_data, $order) {
  // Override some customer information data
  $row_data[22] = 'Company';
  $row_data[23] = 'Lastname';
  $row_data[24] = 'Firstname';
  $row_data[25] = 'Address';
  $row_data[26] = 'Premise';
  $row_data[27] = 'Zipcode'; // Should be numeric of course.
  $row_data[28] = 'City';
  $row_data[29] = 'Country'; // Country code.
}

/**
 * Implements hook_commerce_winbiz_product_data_alter().
 *
 * Use this hook to alter data on a product base. This hook gets
 * called for each single line item in the order.
 *
 * Make sure to add conditions for the line item type to prevent
 * general overrides.
 *
 * @param array $row_data
 * @param object $line_item
 * @param object $order
 */
function ja_job_lib_commerce_winbiz_product_data_alter(&$row_data, $line_item, $order) {
  if ($line_item->type == 'commerce_coupon') {
    // Load the product.
    $item = field_get_items('commerce_line_item', $line_item, 'commerce_coupon_reference');
    $coupon = entity_load_single('commerce_coupon', $item[0]['target_id']);

    $data[50] = $line_item->line_item_label;
    // 51 Product description
    $data[51] = $data[76] = $data[77] = $data[78] = $data[79] = $data[80] = $line_item->line_item_label;
    $data[52] = strftime('%Y%m%d', $line_item->created);
    // 53 Quantity
    $data[53] = 1;

    $item = field_get_items('commerce_line_item', $line_item, 'commerce_unit_price');
    $data[54] = commerce_currency_amount_to_decimal($item[0]['amount'], 'USD');
    // 55 product unit
    $line[55] = $line[85] = t('Piece');

    $coupon_percentage = field_get_items('commerce_coupon', $coupon, 'commerce_coupon_percent_amount');
    // 56 Discount (0-100%)
    $line[56] = $coupon_percentage[0]['value'];
    // 57 Price
    $data[57] = $data[83] = commerce_currency_amount_to_decimal($item[0]['amount'], 'USD');

    // 60 Profit account
    $data[60] = variable_get('commerce_winbiz_profit_account_number', '3000');
    // 61 MwSt %.
    $data[61] = commerce_winbiz_get_tax_rate($line_item);
    // Incl./Excl. Tax
    $data[62] = variable_get('commerce_winbiz_tax_inclusion', 1);
    // 63 Tax account
    $data[63] = variable_get('commerce_winbiz_vattax_account_number', '');
    // 64 Tax handling
    $data[64] = 1;
    // 68 Mehrwertsteuer art, N(1)
    // 0, 1 = Keine MwSt 2 = Geschuldete MwSt 3 = Vorsteuer 4 = Ausserhalb des Felds
    $data[68] = 2;

    // Update of the Article in WB N(1)
    $data[74] = 0;
    // Short description C(40)
    $data[75] = $line_item->line_item_label;
  }
}
