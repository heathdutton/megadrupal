<?php

/**
 * @file
 * Provides store class that internaly uses Drupal's $form_state object
 * to store submitted data from indivitual steps. This implementation
 * of store is suitable for simple forms that incorporate no more than
 * three steps. When using this store implementation note that submitted
 * data is not persisted in any way and as soon as user jumps out from the
 * form lifecycle (i.e. visites different page and returns) the submitted
 * data is lost.
 */

/**
 * Implementation of MformsIstore.
 *
 * It uses $form_state for data storage. This storage is
 * suitable for few-step forms.
 */
class MformsFormStateStore implements MformsIstore {

  /**
   * Pointer to Drupal form_state array.
   *
   * @var array
   */
  private $form_state;

  /**
   * Store key.
   *
   * @var string
   */
  private $store_key;

  /**
   * This instance.
   *
   * @var MformsFormStateStore
   */
  private static $instance;

  /**
   * @static
   *
   * @param string $store_key
   *   Store key.
   *
   * @return MformsFormStateStore|MformsIstore
   *   Instance of MformsFormStateStore.
   */
  static function getInstance($store_key) {

    if (self::$instance == NULL || self::$instance->getKey() != $store_key) {

      self::$instance = new MformsFormStateStore();
      self::$instance->store_key = $store_key;

    }

    return self::$instance;
  }

  function getKey() {
    return $this->store_key;
  }

  function setStore($key, $data) {
    $this->form_state['storage'][$this->getKey()][$key] = $data;
  }

  function getStore($key) {
    if (isset($this->form_state['storage'][$this->getKey()][$key])) {
      return $this->form_state['storage'][$this->getKey()][$key];
    }
    return NULL;
  }

  function clearStore() {
    $this->form_state['rebuild'] = FALSE;
    unset($this->form_state['storage']);
  }

  function setFormState(array &$form_state) {

    if (!isset($form_state['storage'])) {
      $form_state['storage'] = array();
      $form_state['storage'][$this->getKey()] = array();
    }

    $this->form_state = &$form_state;
  }

  function getFormState() {
    return $this->form_state;
  }
}
