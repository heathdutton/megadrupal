<?php
namespace Drupal\go1_base\Cache\Warming\Warmer;

class SimpleWarmer implements WarmerInterface {
  public function validateTag($tag) {
    return TRUE;
  }

  public function processTag($tag, $context = array()) {
    return $tag;
  }
}
