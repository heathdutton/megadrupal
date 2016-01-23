<?php

/**
 * @file
 * Interface for store implementations.
 */

/**
 * Interface defines "must have" methods for a store implementation
 */
interface MformsIstore {

  /**
   * Instantiation of store object should be handeled through factory
   * to secure correct object initiation. Also there is much doubt you
   * should make it singleton.
   *
   * @param string $store_key
   *   Store key.
   *
   * @return MformsIstore
   *   Instance of MformsIstore.
   */
  static function getInstance($store_key);

  /**
   * Gets store key.
   *
   * @return string
   *   Current store instance key.
   */
  function getKey();

  /**
   * Sets value into store under key.
   *
   * @param string $key
   *   Key under which to store data.
   * @param mixed $data
   *   Data to be stored.
   */
  function setStore($key, $data);

  /**
   * Gets value from store stored under supplied key.
   *
   * @param string $key
   *   Key under which data is stored.
   *
   * @return mixed
   *   Stored data.
   */
  function getStore($key);

  /**
   * Deletes data from store. Each implementation should define a way
   * how to dump data from a store. This is the place to do so.
   */
  function clearStore();

  /**
   * Method to set Drupal $form_state array to be used or manipulated
   * further on in other mforms objects.
   *
   * @param array $form_state
   *   Drupal form_state array.
   */
  function setFormState(array &$form_state);

  /**
   * Gets Drupal $form_state array.
   *
   * @return array
   *   Drupal form_state array.
   */
  function getFormState();
}
