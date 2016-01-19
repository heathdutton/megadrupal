<?php
/**
 * @file
 * Base class for stock calculation.
 */

abstract class BaseStockCalculation implements StockCalculationInterface {

  /**
   * The product be stock is being calculated for.
   * @var product
   */
  protected $product;

  /**
   * The settings for this plugin.
   * @var product
   */
  protected $settings = array();

  /**
   * Inject the contexts.
   */
  public function __construct($contexts) {
    $this->setProduct($contexts['product']);
    $this->setSettings($contexts['settings']);
  }

  /**
   * Validate arguments. Return error message if validation failed.
   */
  public function validate() {
    return TRUE;
  }

  /**
   * Main operation. Calculate operation and return the result.
   */
  public function calculate() {
    return 0;
  }

  /**
   * Main operation. Calculate operation and return the result.
   */
  public function settingsForm($form, &$form_state) {
    return array(
      '#markup' => NULL,
    );
  }

  /**
   * Simple getter.
   */
  public function getProduct() {
    return $this->product;
  }

  /**
   * Simple getter.
   */
  public function getSettings() {
    return $this->settings;
  }

  /**
   * Simple setter.
   */
  public function setProduct($product) {
    $this->product = $product;
    return $this;
  }

  /**
   * Simple setter.
   */
  public function setSettings($settings) {
    return $this->settings = $settings;
    return $this;
  }
}
