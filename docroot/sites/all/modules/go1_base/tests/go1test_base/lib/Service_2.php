<?php
namespace Drupal\go1test_base;

class Service_2 {
  private $service_1;

  public function __construct($service_1) {
    $this->service_1 = $service_1;
  }

  public function getService1() {
    return $this->service_1;
  }
}
