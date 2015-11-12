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
