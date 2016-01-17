<?php

/**
 * @file
 * Single class that is a helper to deal with steps.
 */

/**
 * Object that deals with form steps.
 */
class MformsSteps {

  private $store;
  private static $instances = array();

  /**
   * Singleton.
   *
   * @param MformsIstore $store
   *   Store object.
   *
   * @return MformsSteps
   *   Steps object.
   * @throws Exception
   */
  public static function getInstance(MformsIstore $store) {
    if (empty($store)) {
      throw new Exception('Store not provided');
    }

    if (empty(self::$instances[$store->getKey()])) {
      self::$instances[$store->getKey()] = new MformsSteps($store);
    }

    return self::$instances[$store->getKey()];
  }

  /**
   * Inits steps instance.
   *
   * @param MformsIstore $store
   *   Store object
   */
  private function __construct(MformsIstore $store) {
    $this->store = $store;
  }

  /**
   * Gets all steps.
   *
   * @return array
   *   Form steps.
   */
  function getSteps() {
    $steps = $this->store->getStore('steps');
    if (is_array($steps)) {
      return $steps;
    }
    return array();
  }

  /**
   * Gets current step.
   *
   * @return string
   *   Current step.
   */
  function getCurr() {
    return $this->store->getStore('curr_step');
  }

  /**
   * Gets next step.
   *
   * @return string
   *   Next step.
   */
  function getNext() {
    $steps = $this->getSteps();
    $i = array_search($this->getCurr(), $steps);

    if (isset($steps[$i + 1])) {
      return $steps[$i + 1];
    }

    return NULL;
  }

  /**
   * Overrides next step in case it exists.
   *
   * @param string $step
   *   Step name.
   */
  function overrideNext($step) {
    $steps = $this->getSteps();
    $i = array_search($this->getCurr(), $steps);

    if (isset($steps[$i + 1])) {
      $steps[$i + 1] = $step;
      $this->setSteps($steps);
    }
  }

  /**
   * Removes step from steps sequence.
   *
   * @param $step
   */
  function remove($step) {
    $steps = array();

    foreach ($this->getSteps() as $s) {
      if ($s != $step) {
        $steps[] = $s;
      }
    }

    $this->setSteps($steps);
  }

  /**
   * Gets previous step.
   *
   * @return string
   *   Previous step.
   */
  function getPrev() {
    $steps = $this->getSteps();
    $i = array_search($this->getCurr(), $steps);

    if (isset($steps[$i - 1])) {
      return $steps[$i - 1];
    }

    return NULL;
  }

  /**
   * Sets current step.
   *
   * @param string $step
   *   Current step.
   */
  function setCurr($step) {
    $this->store->setStore('curr_step', $step);
  }

  /**
   * Adds step to the steps chain.
   *
   * @param string $step
   *   Step name.
   */
  function addStep($step) {
    if (empty($step)) {
      return;
    }

    $steps = $this->getSteps();
    if (empty($steps) || !in_array($step, $steps)) {
      $steps[] = $step;
      $this->setSteps($steps);
    }
  }

  /**
   * Sets steps.
   *
   * @param array $steps
   *   Steps array.
   */
  private function setSteps($steps) {
    $this->store->setStore('steps', $steps);
  }
}
