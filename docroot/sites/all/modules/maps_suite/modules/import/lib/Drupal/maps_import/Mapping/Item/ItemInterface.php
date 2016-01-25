<?php

/**
 * @file
 * Interface for mapping item.
 */

namespace Drupal\maps_import\Mapping\Item;

interface ItemInterface {

  /**
   * The class constructor.
   */
  public function __construct($definition = array());

  /**
   * Return the item id.
   *
   * @return int
   */
  public function getId();

  /**
   * Return the item related property.
   *
   * @return Drupal\\maps_import\\Mapping\\Source\\MapsSystem\\Property\\PropertyInterface
   */
  public function getProperty();

  /**
   * Retun the item related field.
   *
   * @return Drupal\\maps_import\\Mapping\\Target\\Drupal\\Field\\FieldInterface
   */
  public function getField();

  /**
   * Return the item weight.
   *
   * @return int
   */
  public function getWeight();

  /**
   * Whether the item is required or not.
   *
   * @return bool
   */
  public function isRequired();

  /**
   * Whether the item is static or not.
   *
   * @return bool
   */
  public function isStatic();

  /**
   * Return the item options
   *
   * @return array
   */
  public function getOptions();

  /**
   * Save the mapping item into database.
   */
  public function save();

  /**
   * Delete the mapping item from the database.
   */
  public function delete();

  /**
   * Whether the field or the property has options.
   *
   * @return boolean
   */
  public function hasOptions();

  /**
   * Get the item source options.
   *
   * @return array
   *   The source options.
   */
  public function getSourceOptions();

  /**
   * Get the item target options.
   *
   * @return array
   *   The target options.
   */
  public function getTargetOptions();

  /**
   * Get the source options form.
   *
   * @return array
   *   The form elements to display.
   */
  public function getSourceOptionsForm($form, &$form_state);

  /**
   * Get the target options form.
   *
   * @return array
   *   The form elements to display.
   */
  public function getTargetOptionsForm($form, &$form_state);

  /**
   * Get the options form.
   *
   * @return array
   *   The form elements to display.
   */
  public function optionsForm($form, &$form_state);

  /**
   * Manage the validation of the options form.
   *
   * @return boolean
   *   The validation status.
   */
  public function optionsFormValidate($form, &$form_state);

  /**
   * Manage the submission of the options form.
   *
   * @return boolean
   *   Whether the submit process with success.
   */
  public function optionsFormSubmit($form, &$form_state);

}
