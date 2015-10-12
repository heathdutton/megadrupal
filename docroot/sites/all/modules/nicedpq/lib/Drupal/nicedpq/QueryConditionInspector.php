<?php

namespace Drupal\nicedpq;

/**
 * We use inheritance to crack open the protected attributes.
 * Yes, this is blasphemic, but the only way to do this in contrib.
 */
class QueryConditionInspector extends \DatabaseCondition {

  static function extractAttributes($cond) {
    $result = new \stdClass;
    foreach ($cond as $key => $value) {
      $result->$key = $value;
    }
    return $result;
  }
}
