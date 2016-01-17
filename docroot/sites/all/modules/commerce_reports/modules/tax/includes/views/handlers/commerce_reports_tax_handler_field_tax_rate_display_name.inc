<?php

/**
 * Field handler to translate a line item type into its readable form.
 */
class commerce_reports_tax_handler_field_tax_rate_display_name extends views_handler_field {
  /**
   * Add some required fields needed on render().
   */
  function construct() {
    parent::construct();

    // Bring the tax_rate field into scope.
    $this->additional_fields['tax_rate'] = 'tax_rate';
  }
  /**
   * Loads additional fields.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Load the tax rate components in the system
    $commerce_tax_rates = commerce_tax_rates();
    // Get our tax_rate name
    $tax_rate_name = $this->get_value($values, 'tax_rate');

    // Return the display_title attribute from the component array.
    return $commerce_tax_rates[$tax_rate_name]['display_title'];
  }
}
