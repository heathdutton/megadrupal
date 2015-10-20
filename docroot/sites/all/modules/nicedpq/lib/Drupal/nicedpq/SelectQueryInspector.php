<?php

namespace Drupal\nicedpq;

/**
 * We use inheritance to crack open the protected attributes.
 * Yes, this is blasphemic, but the only way to do this in contrib.
 */
class SelectQueryInspector extends \SelectQuery {

  static function extractAttributes($q) {
    $result = new \stdClass;
    foreach ($q as $key => $value) {
      $result->$key = $value;
    }
    return $result;
  }
}
