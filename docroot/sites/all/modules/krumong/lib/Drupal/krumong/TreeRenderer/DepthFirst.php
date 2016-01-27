<?php

namespace Drupal\krumong;


class TreeRenderer_DepthFirst extends TreeRenderer_Abstract {

  // hive management
  // ---------------------------------------------------------------------------

  protected function hiveDetectRecursion($data, $trail) {
    if (FALSE === $trail) {
      $this->hiveAddElement($data);
      return FALSE;
    }
    else {
      return TRUE;
    }
  }
}
