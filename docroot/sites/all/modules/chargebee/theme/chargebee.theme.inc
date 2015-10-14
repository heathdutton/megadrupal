<?php
/**
 * @file
 * Module theme functions.
 */

/**
 * Checkout hostedpage theme.
 *
 * @param array $variables
 *   Theme variables.
 *
 * @return string
 *   Hostedpage HTML.
 */
function theme_chargebee_hostedpage($variables) {
  $hostedpage = $variables['hostedpage'];
  $url = isset($hostedpage['url']) ? $hostedpage['url'] : '';
  $output = '<iframe src="' . $url . '"></iframe>';
  return $output;
}
