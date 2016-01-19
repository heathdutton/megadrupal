<?php
/**
 * @file
 * Interface to be implemented for all the stock_calculation plugin classes.
 */

interface StockCalculationInterface {

  /**
   * Validate arguments. Return error message if validation failed.
   */
  public function validate();

  /**
   * Main operation. Calculate the stock and return the result.
   */
  public function calculate();

  /**
   * Main operation. Calculate the stock and return the result.
   */
  public function settingsForm($form, &$form_state);
}
