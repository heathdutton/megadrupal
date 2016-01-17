<?php

/**
 * @file
 *   Provides theme functions for pgapi module.
 */

/**
 * Renders PGAPI payment form.
 *
 * Process form and output it as table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: The payment form to theme.
 *
 * @ingroup themeable
 */
function theme_pgapi_payment($variables) {
  $form = $variables['form'];
  // Set table headers
  $header = array(
    '',
    t('Payment Gateway'),
    t('Price'),
  );

  // Create table from rendered form elements
  $rows = array();
  foreach (element_children($form) as $element_name) {
    if (isset($form[$element_name]['modulename'])) {
      $row    = array();
      $row[]  = drupal_render($form['gateway'][$element_name]);
      $row[]  = drupal_render($form[$element_name]['modulename']);
      $row[]  = drupal_render($form[$element_name]['price']);
      $rows[] = $row;
    }
  }

  // Return form table and render remain part of form
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    // If no payment gateways.
    'empty' => t('No payment gateway available.'),
  ));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Returns formatted price for payment system.
 *
 * @param $variables
 *   An associative array containing:
 *   - prices: Array with prices to format.
 *   - service: The payment system name.
 *
 * @ingroup themeable
 */
function theme_pgapi_gateway_price($variables) {
  $prices = $variables['prices'];
  $service = $variables['service'];
  $output = '';
  if (is_array($prices)) {
    foreach ($prices as $cur => $gross) {
      if (is_array($gross)) {
        $output .= "<div style='float:left;'>" . $cur . "&nbsp</div>" . pgapi_format_price($service, $gross['price'], $gross['currency']);
      }
      else {
        $output .= pgapi_format_price($service, $gross, $cur);
      }
    }
  }
  return $output;
}

/**
 * Returns formated price as html.
 *
 * @param $variables
 *   An associative array containing:
 *   - price: A price to format.
 *   - symbol: A symbol of currency.
 *   - position: Symbol is leftmost if TRUE.
 *
 * @ingroup themeable
 */
function theme_pgapi_format_price($variables) {
  $price = $variables['price'];
  $symbol = $variables['symbol'];
  $position = $variables['position'];
  if ($position) {
    $output = '<div class="price"><span class="symbol">' . $symbol . '</span> ' . $price . '</div>';
  }
  else {
    $output = '<div class="price">' . $price . '<span class="symbol">' . $symbol . '</span></div>';
  }
  return $output;
}

/**
 * Returns formated price without html.
 *
 * @param $variables
 *   An associative array containing:
 *   - price: A price to format.
 *   - symbol: A symbol of currency.
 *   - position: Symbol is leftmost if TRUE.
 *
 * @ingroup themeable
 */
function theme_pgapi_format_price_plain($variables) {
  $price = $variables['price'];
  $symbol = $variables['symbol'];
  // Position before price if TRUE.
  $position = $variables['position'];
  if ($position) {
    $output = $symbol . ' ' . $price;
  }
  else {
    $output = $price . ' ' . $symbol;
  }
  return $output;
}
