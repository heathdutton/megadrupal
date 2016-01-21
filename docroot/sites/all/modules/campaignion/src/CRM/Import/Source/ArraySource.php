<?php

namespace Drupal\campaignion\CRM\Import\Source;

class ArraySource implements SourceInterface {
  protected $data;
  public function __construct($data) {
    $this->data = $data;
  }
  public function value($key) {
    return isset($this->data[$key]) ? $this->data[$key] : NULL;
  }
}
