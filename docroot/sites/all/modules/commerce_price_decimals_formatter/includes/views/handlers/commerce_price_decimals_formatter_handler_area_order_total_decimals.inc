<?php

/**
 * @file
 * Defines an order total area handler that shows the order total field with its
 * components listed in the footer of a View.
 */

class commerce_price_decimals_formatter_handler_area_order_total_decimals extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();

    $options['currencies']['default'] = commerce_price_decimals_formatter_get_default_currencies_settings();

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['empty']['#description'] = t("Even if selected, this area handler will never render if a valid order cannot be found in the View's arguments.");

    $form = array_merge($form, commerce_price_decimals_formatter_get_settings_form($this->options));
  }

  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {
      // First look for an order_id argument.
      foreach ($this->view->argument as $name => $argument) {
        if ($argument instanceof commerce_order_handler_argument_order_order_id) {
          // If it is single value...
          if (count($argument->value) == 1) {
            // Load the order.
            if ($order = commerce_order_load(reset($argument->value))) {

              // Prepare a display settings array.
              $display = array(
                'label' => 'hidden',
                'type' => 'commerce_price_decimals_formatter_components',
                'settings' => array(
                  'calculation' => FALSE,
                  'currencies' => $this->options['currencies'],
                ),
              );

              // Render the order's order total field with the current display.
              $field = field_view_field('commerce_order', $order, 'commerce_order_total', $display);

              return '<div class="commerce-order-handler-area-order-total">' . drupal_render($field) . '</div>';
            }
          }
        }
      }
    }

    return '';
  }
}
