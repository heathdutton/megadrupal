<?php

/**
 * @file
 * Contains the Fraction Views field handler.
 */

/**
 * Field handler for Fraction fields.
 */
class fraction_handler_field extends views_handler_field_field {

  /**
   * Use a query formula for click sorting.
   */
  function click_sort($order) {

    // Ensure the main table for this field is included.
    $this->ensure_my_table();

    // Formula for calculating the final value, by dividing numerator by denominator.
    // These are added as additional fields in hook_field_views_data_alter().
    $formula = $this->table_alias . '.' . $this->definition['additional fields'][0] . ' / ' . $this->table_alias . '.' . $this->definition['additional fields'][1];

    // Add the orderby.
    $this->query->add_orderby(NULL, $formula, $order, $this->table_alias . '_decimal');
  }

  /**
   * Remove the 'click_sort_column' option.
   */
  function options_form(&$form, &$form_state) {

    // Inherit the parent options form.
    parent::options_form($form, $form_state);

    // Remove the 'click_sort_column' form element.
    unset($form['click_sort_column']);
  }
}

