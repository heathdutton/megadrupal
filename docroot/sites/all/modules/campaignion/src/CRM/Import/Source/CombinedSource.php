<?php

namespace Drupal\campaignion\CRM\Import\Source;

class CombinedSource implements SourceInterface {
  protected $a;
  protected $b;
  public function __construct(SourceInterface $a, SourceInterface $b) {
    $this->a = $a;
    $this->b = $b;
  }

  public function value($key) {
    if (!is_null($value = $this->a->value($key))) {
      return $value;
    }
    if (!is_null($value = $this->b->value($key))) {
      return $value;
    }
  }
}
