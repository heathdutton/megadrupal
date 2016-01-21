<?php
/**
 * @file
 * Monitoring sensor settings interface.
 */

namespace Drupal\monitoring\Sensor;

/**
 * Interface for a configurable sensor.
 *
 * Base interface defining implicit operations for a monitoring sensor exposing
 * custom settings.
 *
 * @todo more
 */
interface SensorConfigurableInterface {

  /**
   * Gets settings form for a specific sensor.
   *
   * @param $form
   *   Drupal $form structure.
   * @param array $form_state
   *   Drupal $form_state object. Carrying the string sensor_name.
   *
   * @return array
   *   Drupal form structure.
   */
  function settingsForm($form, &$form_state);

  /**
   * Form validator for a sensor settings form.
   *
   * @param $form
   *   Drupal $form structure.
   * @param array $form_state
   *   Drupal $form_state object. Carrying the string sensor_name.
   */
  function settingsFormValidate($form, &$form_state);

}
