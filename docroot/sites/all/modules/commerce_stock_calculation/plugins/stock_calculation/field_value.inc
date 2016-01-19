<?php
/**
 * @file
 * Class to work out the stock adding all the values
 */

/**
 * Stock calculation plugin for commerce_stock_sources module.
 *
 * Calculate sum of two numbers.
 */

$plugin = array(
  'label' => t('Field value'),
  'handler' => array(
    'class' => 'FieldValueStockCalculation',
  ),
);

class FieldValueStockCalculation extends BaseStockCalculation {

  /**
   * The field_instance.
   * @var array
   */
  protected $field_instance;

  /**
   * Inject the contexts.
   */
  public function __construct($contexts) {
    parent::__construct($contexts);
    $this->field_instance = $contexts['field_instance'];
  }

  /**
   * Calculates the total stock for this method.
   */
  public function calculate() {

    $entity_type = $this->field_instance['entity_type'];
    $field_name = $this->field_instance['field_name'];

    $items = field_get_items($entity_type, $this->product, $field_name);

    $stock = $items[0]['value'];

    return $stock;
  }
}
