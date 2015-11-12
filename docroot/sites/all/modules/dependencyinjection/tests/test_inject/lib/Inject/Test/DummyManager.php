<?php

namespace Inject\Test;

use Drupal\test_inject\Dummy;

class DummyManager {

  var $dummy;

  public function setDummy($dummy) {
    if ($dummy instanceof Dummy) {
      // Everything is fine.
      $this->dummy = $dummy;
    }
    else {
      throw new \RuntimeException('Parameter is not Dummy.');
    }
  }

  public function getDummy() {
    return $this->dummy;
  }

}
