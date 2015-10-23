<?php

/**
 * @file
 * Hooks provided by the Commerce Price Savings formatter.
 */

/**
 * Lets modules alter the formatted prices created by the formattters
 *
 * @param $formatted_prices
 *  An array of the formatted prices keyed by the price type.
 *  Each price item 'list', 'price', 'savings' contains:
 *    - label: The price label SAFE for display
 *    - amount: The price amount, integer
 *    - currency_code: The currency code, string
 *    - formatted: The formatted price provided by default, string
 *  The 'savings' price item contains the following in addition to the above:
 *    - percent: An array containing percent information
 *        -- value: The decimal value of the savings percent, float
 *        -- formatted: The formatted percent provided by default, string
 *
 * @param $context
 *  A context array of information for the display:
 *    - settings: The field formatter settings
 *    - entity_type: The entity type that the field is attached to
 *    - entity: The entity object
 *    - field: The field structure being rendered.
 *    - instance: The instance structure being rendered.
 *    - langcode: The field language.
 *    - display: The display settings to use, as found in the 'display' entry
 *      of instance definitions
 */
function hook_commerce_price_savings_formatter_prices_alter(&$formatted_prices, $context) {
  // Exit if custom setting is not there
  if (!isset($context['settings']['mymodule_custom_setting'])) {
    return;
  }

  foreach ($formatted_prices as $price_type => $formatted_price) {
    // Alter the formatted price
    $formatted_prices[$price_type]['formatted'] = mymodule_commerce_currency_format($formatted_price['amount'], $formatted_price['currency_code'], $context['settings'], $context['entity']);

    // Alter the percent rounding
    if ($price_type == 'savings' && !empty($formatted_prices[$price_type]['percent']['value'])) {
      $formatted_prices[$price_type]['percent']['formatted'] = round($formatted_prices[$price_type]['percent']['value'] * 100, 2) . '%';
    }
  }
}
