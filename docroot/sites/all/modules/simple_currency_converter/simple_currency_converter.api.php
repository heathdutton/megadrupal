<?php
/**
 * @file
 * API documentation for the simple currency converter module.
 */

/**
 * Allows to override simple currency converter settings.
 */
function hook_simple_currency_converter_settings_alter(&$settings) {
  // Available settings.
  $settings['scc_modal_window_trigger'];
  $settings['scc_modal_window_id'];
  $settings['scc_element_to_convert'];
  $settings['scc_country_info'];
  $settings['scc_modal_window_title'];
  $settings['scc_default_conversion_currency'];
  $settings['scc_user_cookie_expiration'];
  $settings['scc_country_info']['USD']['code'];
  $settings['scc_country_info']['USD']['symbol'];
  $settings['scc_country_info']['USD']['symbol_placement'];
}

/**
 * Allows to override conversion result.
 */
function hook_simple_currency_converter_result_alter($from_currency, $to_currency, &$conversion_ratio) {
  // You can apply conversion fee in some way like this.
  if ($from_currency == 'USD') {
    $conversion_ratio = $conversion_ratio * 1.3;
  }
}
