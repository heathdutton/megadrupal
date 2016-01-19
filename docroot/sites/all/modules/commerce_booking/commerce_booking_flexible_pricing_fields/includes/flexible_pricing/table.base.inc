<?php
/**
 * @file
 * Contains a flexible pricing handler base for booking windows.
 */

/**
 * Booking window base flexible pricing handler.
 */
abstract class CommerceFlexiblePricingHandlerTableBase implements CommerceFlexiblePricingHandlerInterface {

  /**
   * The field structure.
   *
   * @var array
   */
  protected $field;

  /**
   * The field instance.
   *
   * @var array
   */
  protected $instance;

  /**
   * The key for the X-axis.
   */
  protected $x_key = 'x';

  /**
   * The key for the Y-axis.
   */
  protected $y_key = 'y';

  /**
   * Constructor for the flexible pricing handler.
   *
   * @param array $field
   *   The field structure.
   * @param array $instance
   *   The field instance.
   */
  public function __construct($field, $instance) {
    $this->field = $field;
    $this->instance = $instance;
  }

  /**
   * Return options for the X-axis.
   *
   * @param object $entity
   *   The entity containing the price field.
   *
   * @return array
   *   Labels keyed by value.
   */
  abstract protected function getXOptions($entity);

  /**
   * Return the value for the X-axis.
   *
   * @param object $entity
   *   The entity we want the value for.
   * @param array $options
   *   Optionally provide additional options.
   *
   * @return
   *   A scalar value for the X-axis.
   */
  abstract public function getXValue($entity, $options = array());

  /**
   * Return options for the Y-axis.
   *
   * @param object $entity
   *   The entity containing the price field.
   *
   * @return array
   *   Labels keyed by value.
   */
  abstract protected function getYOptions($entity);

  /**
   * Return the value for the Y-axis.
   *
   * @param object $entity
   *   The entity we want the value for.
   * @param array $options
   *   Optionally provide additional options.
   *
   * @return
   *   A scalar value for the Y-axis.
   */
  abstract public function getYValue($entity, $options = array());

  /**
   * Get the entity containing the price from the price entity.
   *
   * Classes working with items where the price field are not on the entity we
   * want the price for should override this.
   *
   * @param object $entity
   *   The entity we want the price for.
   *
   * @return object
   *   The entity containing the price field.
   */
  protected function getPriceEntity($entity) {
    return $entity;
  }

  /**
   * {@inheritdoc}
   */
  public function fieldWidget(&$form, &$form_state, $langcode, $items, $delta, $element) {
    // Set up some defaults.
    $element += array(
      '#theme' => 'commerce_booking_flexible_pricing_base_table',
      '#title' => NULL,
      '#headers' => array(),
      '#prefix' => '<div id="flexible-price-wrapper">',
      '#suffix' => '</div>',
    );

    // Retrieve our X-axis (header) information.
    $element['#headers']['_xheader'] = '';
    $element['#options_x'] = $this->getXOptions($element['#entity']);
    foreach ($element['#options_x'] as $value => $label) {
      $element['#headers'][$value] = $label;
    }

    // Retrieve our Y-axis (rows) information.
    $element['#options_y'] = $this->getYOptions($element['#entity']);

    // Prepare the initial map.
    $element['#map'] = $this->buildPriceMap($items, $next_delta);

    // Build enough price fields.
    $sub_instance = $this->instance;
    $sub_instance['widget']['type'] = 'commerce_price_simple';

    foreach (array_keys($element['#options_x']) as $x_value) {
      foreach (array_keys($element['#options_y']) as $y_value) {
        // If we don't already have a delta mapped for this item, add it.
        if (!isset($element['#map'][$x_value][$y_value])) {
          $element['#map'][$x_value][$y_value] = $next_delta;
          $next_delta++;
        }
        $delta = $element['#map'][$x_value][$y_value];

        // Build our simple price field.
        $form_element = array('#title' => NULL);
        $form_element = commerce_price_field_widget_form($form, $form_state, $this->field, $sub_instance, $langcode, $items, $delta, $form_element);

        // Add our mapping data.
        $form_element['data'] = array(
          '#type' => 'value',
          '#value' => array(
            $this->x_key => $x_value,
            $this->y_key => $y_value,
          ),
        );

        // Add it to our element.
        $element[$delta] = $form_element;
      }
    }

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function fieldWidgetValidate($element, &$form_state, &$form) {}

  /**
   * Get the price items.
   *
   * @param $entity
   *   The entity we are working on.
   *
   * @return array
   *   The price items.
   */
  protected function getPriceItems($entity) {
    return field_get_items($this->instance['entity_type'], $entity, $this->field['field_name']);
  }

  /**
   * Convert an array of items into a traversable tree.
   *
   * @param array $items
   *   An array of price field items.
   * @param int $next_delta
   *   Optionally provide a variable to be filled with the next available
   *   delta.
   *
   * @return array
   *   An array where values are the price item delta, keys are the booking
   *   window id and outer keys are the other identifier.
   */
  protected function buildPriceMap($items, &$next_delta = NULL) {
    $map = array();
    $next_delta = 0;

    // Check $items is an array as field_get_items() can return FALSE.
    if (is_array($items)) {
      foreach ($items as $delta => $item) {
        // We can't include this in the map if it's missing either key.
        if (isset($item['data'][$this->x_key]) && isset($item['data'][$this->y_key])) {
          // Set it correctly in the map.
          $map[$item['data'][$this->x_key]][$item['data'][$this->y_key]] = $delta;
        }

        // Increment the next delta.
        $next_delta = $delta + 1;
      }
    }

    return $map;
  }

  /**
   * {@inheritdoc}
   */
  public function calculatePrice($entity, $options = array()) {
    $price_entity = $this->getPriceEntity($entity);

    $x_value = $this->getXValue($entity, $options);
    $y_value = $this->getYValue($entity, $options);

    $price_items = $this->getPriceItems($price_entity);
    $map = $this->buildPriceMap($price_items);

    // If the price exists, return it.
    if (isset($map[$x_value][$y_value])) {
      $delta = $map[$x_value][$y_value];
      if (isset($price_items[$delta])) {
        return $price_items[$delta];
      }
    }

    // Otherwise return a NULL.
    return NULL;
  }

}
