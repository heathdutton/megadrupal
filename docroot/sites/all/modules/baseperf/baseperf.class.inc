<?php

/**
 * @file
 * Provides a base class for baseline performance tests.
 */

abstract class BasePerf {

  protected $plugin;
  protected $memory_t0;
  protected $memory_t1;
  protected $memory_peak;
  protected $time;

  function __construct($plugin) {
    $this->plugin = $plugin;
  }

  public function getName() {
    return $this->plugin['name'];
  }

  /**
   * Returns a description of the test.
   */
  public function getDescription() {
    return $this->plugin['description'];
  }

  /**
   * Returns required modules.
   */
  protected function requiredModules() {
    return array();
  }

  /**
   * Returns a requirements array as hook_requirements().
   */
  public function requirements() {
    $req = array();
    $missing = array_diff($this->requiredModules(), module_list());
    if ($missing) {
      $req['dependencies'] = array(
        'title' => t('Module dependencies'),
        'value' => t('This plugin depends on the following modules: @modules.', array('@modules' => implode(', ', $missing))),
        'severity' => REQUIREMENT_ERROR,
      );
    }
    return $req;
  }

  /**
   * Config form builder.
   */
  abstract public function configForm();

  /**
   * Config form validation handler.
   */
  public function configFormValidate($form, &$values) {
  }

  /**
   * Config form submission handler.
   */
  public function configFormSubmit($form, $values) {
    $this->run($values);
  }

  /**
   * Ejecutes the test operations.
   */
  abstract protected function execute($config);

  /**
   * Run the test.
   */
  final public function run($config) {
    $this->memory_t0 = memory_get_usage();
    timer_start($this->getName());

    $this->execute($config);

    $timer = timer_stop($this->getName());
    $this->time = $timer['time'];

    $this->memory_t1 = memory_get_usage();
    $this->memory_peak = memory_get_peak_usage(TRUE);
  }

  /**
   * Returns measures.
   */
  final public function getResults() {
    return array($this->time, $this->memory_t0, $this->memory_t1, $this->memory_peak);
  }
}
