<?php
/**
 * @file
 * Contains a flexible pricing handler base for booking windows.
 */

/**
 * Booking window base flexible pricing handler.
 */
abstract class CommerceFlexiblePricingHandlerBookingWindowBase extends CommerceFlexiblePricingHandlerTableBase {

  /**
   * The key for the X-axis.
   */
  protected $x_key = 'window';

  /**
   * {@inheritdoc}
   */
  protected function getXOptions($entity) {
    $items = $this->getBookingWindows($entity);

      if (empty($items)) {
        // Return a single fake window.
        return array(
          1 => t('Price'),
        );
      }
      // Otherwise build our options array.
      else {
        $windows = array();
        foreach ($items as $item) {
          $windows[$item['window_id']] = $item['label'];
        }
      return $windows;
    }
  }

  /**
   * Figure out the booking window (X-axis).
   *
   * @param object $entity
   *   The entity we want the value for.
   * @param array $options
   *   Optionally provide additional options. Possible keys include:
   *     - date: The date to use for calculating the current booking window.
   *       Defaults to REQUEST_TIME.
   *
   * @return int
   *   The booking window id.
   */
  public function getXValue($entity, $options = array()) {
    $price_entity = $this->getPriceEntity($entity);

    // Get hold fo the date, defaulting to REQUEST_TIME.
    $date = isset($options['date']) ? $options['date'] : REQUEST_TIME;

    // Convert the date for comparison.
    $date = date('Y-m-d', $date);

    // Get hold of the booking windows.
    $windows = $this->getBookingWindows($price_entity);
    $window = array_shift($windows);
    while (isset($window['cut_off']) && $window['cut_off'] < $date) {
      $window = array_shift($windows);
    }

    return $window['window_id'];
  }

  /**
   * {@inheritdoc}
   */
  public function fieldWidgetSettings() {
    $form['booking_window_field'] = array(
      '#type' => 'select',
      '#title' => t('Select the field containing the booking windows.'),
      '#description' => t('This field must be a booking window field.'),
      '#options' => array(
        NULL => t('- None -'),
      ),
      '#default_value' => isset($this->instance['widget']['settings']['flexible_pricing']['booking_window_field']) ? $this->instance['widget']['settings']['flexible_pricing']['booking_window_field']: NULL,
    );

    // Loop over the instances on this entity/bundle to find possible fields.
    foreach (field_info_instances($this->instance['entity_type'], $this->instance['bundle']) as $instance) {
      $field = field_info_field($instance['field_name']);
      if ($field['type'] == 'commerce_booking_window') {
        $form['booking_window_field']['#options'][$instance['field_name']] = $instance['label'];
      }
    }

    return $form;
  }

  /**
   * Return booking windows.
   *
   * @param object $entity
   *   The entity containing the price field.
   * @param bool $sort
   *   Whether to sort the windows by cut off date. Defaults to TRUE.
   *
   * @return array
   *   An array of booking window items.
   */
  protected function getBookingWindows($entity, $sort = TRUE) {
    $field_name = isset($this->instance['widget']['settings']['flexible_pricing']['booking_window_field']) ? $this->instance['widget']['settings']['flexible_pricing']['booking_window_field']: NULL;
    if (empty($field_name)) { return array(); }
    $items = field_get_items($this->instance['entity_type'], $entity, $field_name);

    if ($sort && is_array($items)) {
      usort($items, array($this, 'sortBookingWindows'));
    }

    return $items;
  }

  /**
   * Comparison callback for sorting booking windows.
   */
  protected function sortBookingWindows($a, $b) {
    // NULL indicates the last booking window.
    if ($a['cut_off'] === NULL) {
      return 1;
    }
    elseif ($b['cut_off'] == NULL) {
      return -1;
    }

    // Otherwise compare the dates.
    return $a['cut_off'] > $b['cut_off'] ? 1: -1;
  }

}
