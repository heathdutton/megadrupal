<?php
/**
 * @file
 * Contains the interface and base for flexible pricing handlers.
 */

/**
 * Interface for flexible pricing handlers.
 */
interface CommerceFlexiblePricingHandlerInterface {


  /**
   * Get a constructed price handler for an entity.
   *
   * @param object $entity
   *   The entity we want a price for.
   *
   * @return CommerceFlexiblePricingHandlerInterface
   */
  public static function getHandler($entity);

  /**
   * Construct a new flexible pricing handler.
   */
  public function __construct($field, $instance);

  /**
   * Calculate the price.
   *
   * @param object $entity
   *   The entity we want the price for.
   * @param array $options
   *   Optionally provide additional options for the price calculation.
   *
   * @return array
   *   A commerce price field array or NULL if there is no valid value.
   */
  public function calculatePrice($entity, $options = array());

  /**
   * Build a field widget element for flexible pricing.
   *
   * @see hook_field_widget_form()
   * @see commerce_booking_flexible_pricing_fields_field_widget_form()
   */
  public function fieldWidget(&$form, &$form_state, $langcode, $items, $delta, $element);

  /**
   * Validation handler for price fields using this handler.
   *
   * @see hook_field_widget_form_validate()
   * @see commerce_booking_flexible_pricing_fields_field_widget_form_validate()
   */
  public function fieldWidgetValidate($element, &$form_state, &$form);

  /**
   * Build the field widget settings form.
   *
   * @return array
   *   The form definition for the widget settings.
   *
   * @see hook_field_widget_settings_form()
   * @see commerce_booking_flexible_pricing_fields_field_widget_settings_form()
   */
  public function fieldWidgetSettings();

}
