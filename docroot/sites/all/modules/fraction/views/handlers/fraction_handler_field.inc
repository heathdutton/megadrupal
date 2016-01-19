<?php

/**
 * @file
 * Contains the Fraction Views field handler.
 */

/**
 * Field handler for Fraction database columns.
 */
class fraction_handler_field extends views_handler_field {

  /**
   * @inheritdoc
   */
  function option_definition() {
    $options = parent::option_definition();

    // Default to automatic precision.
    $options['precision'] = array('default' => 0);
    $options['auto_precision'] = array('default' => TRUE, 'bool' => TRUE);

    return $options;
  }

  /**
   * @inheritdoc
   */
  function options_form(&$form, &$form_state) {

    // Add fields for configuring precision and auto_precision.
    $form['precision'] = array(
      '#type' => 'textfield',
      '#title' => t('Precision'),
      '#description' => t('Specify the number of digits after the decimal place to display when converting the fraction to a decimal. When "Auto precision" is enabled, this value essentially becomes a minimum fallback precision.'),
      '#default_value' => $this->options['precision'],
    );
    $form['auto_precision'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto precision'),
      '#description' => t('Automatically determine the maximum precision if the fraction has a base-10 denominator. For example, 1/100 would have a precision of 2, 1/1000 would have a precision of 3, etc.'),
      '#default_value' => $this->options['auto_precision'],
    );

    // Merge into the parent form.
    parent::options_form($form, $form_state);

    // Remove the 'click_sort_column' form element, because we provide a custom
    // click_sort function below to use the numerator and denominator columns
    // simultaneously.
    unset($form['click_sort_column']);
  }

  /**
   * Use a query formula for click sorting.
   */
  function click_sort($order) {

    // Ensure the main table for this field is included.
    $this->ensure_my_table();

    // Formula for calculating the final value, by dividing numerator by denominator.
    // These are added as additional fields in hook_field_views_data_alter().
    $formula = $this->table_alias . '.' . $this->definition['additional fields']['numerator'] . ' / ' . $this->table_alias . '.' . $this->definition['additional fields']['denominator'];

    // Add the orderby.
    $this->query->add_orderby(NULL, $formula, $order, $this->table_alias . '_decimal');
  }

  /**
   * Load the numerator and denominator values and perform conversion to decimal.
   */
  function get_value($values, $field = NULL) {

    // Find the numerator and denominator field aliases.
    $numerator_alias = $this->aliases[$this->definition['additional fields']['numerator']];
    $denominator_alias = $this->aliases[$this->definition['additional fields']['denominator']];

    // If both values are available...
    if (isset($values->{$numerator_alias}) && isset($values->{$denominator_alias})) {

      // Convert to decimal.
      $numerator = $values->{$numerator_alias};
      $denominator = $values->{$denominator_alias};
      $precision = $this->options['precision'];
      $auto_precision = $this->options['auto_precision'];
      return fraction($numerator, $denominator)->toDecimal($precision, $auto_precision);
    }
  }
}

