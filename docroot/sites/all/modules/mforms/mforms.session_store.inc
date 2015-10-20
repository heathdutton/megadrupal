<?php

/**
 * @file
 * Single class that is an implementation of store.
 * This store may be used for multi-step forms as it persists
 * submitted data in session so user will not loose the data if
 * leaves the page on accidence.
 */

/**
 * Implementation of MformsIstore
 *
 * It uses as store the $_SESSION object. This store is
 * suitable for creating complex multistep forms.
 */
class MformsSessionStore implements MformsIstore {

  private $store_key;
  private $form_state;
  private static $instance;

  static function getInstance($store_key) {
    if (self::$instance == NULL || self::$instance->getKey() != $store_key) {
      self::$instance = new MformsSessionStore($store_key);
    }
    return self::$instance;
  }

  /**
   * Inits the store object.
   *
   * @param string $store_key
   *   Store key.
   */
  protected function __construct($store_key) {
    $this->store_key = $store_key;
  }

  function getKey() {
    return $this->store_key;
  }

  function setStore($key, $data) {
    $_SESSION[$this->store_key][$key] = $data;
  }

  function getStore($key) {
    if (isset($_SESSION[$this->store_key][$key])) {
      return $_SESSION[$this->store_key][$key];
    }
    return NULL;
  }

  function clearStore() {
    $_SESSION[$this->store_key] = NULL;
  }

  function setFormState(array &$form_state) {
    $this->form_state = $form_state;
  }

  function getFormState() {
    return $this->form_state;
  }
}
