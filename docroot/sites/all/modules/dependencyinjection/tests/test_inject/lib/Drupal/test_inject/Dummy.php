<?php

namespace Drupal\test_inject;

class Dummy {

  var $dummy;

  public function __construct($dummy = NULL) {
    if ($dummy != 'dummy') {
      throw new \RuntimeException('Parameter is not dummy.');
    }

    $this->dummy = $dummy;
  }

  public function getParameter() {
    return $this->dummy;
  }

}
