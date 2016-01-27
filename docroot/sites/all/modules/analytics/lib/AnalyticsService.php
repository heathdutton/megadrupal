<?php

abstract class AnalyticsService implements AnalyticsServiceInterface {
  public $service;
  protected $hasMultiple;

  public function __construct($service) {
    $this->service = $service;
    $this->service->options += $this->getDefaultOptions();
  }

  public function getDefaultOptions() {
    return array();
  }

  public function getFormOptions(&$form, &$form_state) {

  }

  public function canTrack(array $context) {
    return !path_is_admin($_GET['q']);
  }

  abstract public function getOutput(array $context);

  protected function hasMultipleInstances() {
    if (!isset($this->hasMultiple)) {
      ctools_include('export');
      $services = ctools_export_crud_load_all('analytics_service', array('service' => $this->service->service));
      $this->hasMultiple = count($services) >= 2;
    }
    return $this->hasMultiple;
  }

  public function getCacheableUrls() {
    return array();
  }
}
