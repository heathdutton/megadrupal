<?php
/**
 * @file
 * Class to work out the stock adding all the values
 */

/**
 * Stock calculation plugin for commerce_stock_sources module.
 */

$plugin = array(
  'label' => t('Stock sources sum'),
  'handler' => array(
    'class' => 'SumStockCalculation',
  ),
);

class SumStockCalculation extends BaseStockCalculation {

  /**
   * Calculates the total stock for this method.
   */
  public function calculate() {

    $settings = $this->getSettings();
    $product = $this->getProduct();

    $quantities = commerce_stock_sources_load_quantities_by_product($product->product_id);

    $quantity = 0;

    foreach ($quantities as $stock) {
      $id = $stock->source_id;

      if (!empty($settings[$id])) {
        $quantity += $stock->quantity;
      }
    }

    return $quantity;
  }

  /**
   * Settings form.
   */
  public function settingsForm($form, &$form_state) {

    $sources = commerce_stock_sources_load_all_as_options();

    return array(
      '#title' => t('Please, select the available sources'),
      '#type' => 'checkboxes',
      '#options' => $sources,
      '#default_value' => $this->getSettings(),
    );
  }
}
