<?php

namespace Drupal\campaignion\CRM\Import\Field;

class Date extends Field {
  public function preprocessField($value) {
    if (is_numeric($value)) {
      return (int) $value;
    } elseif (($date = strtotime($value)) != FALSE) {
      return $date;
    } elseif (($date = \DateTime::createFromFormat('d/m/Y', $value)) != FALSE) {
      $date->setTime(0,0,0);
      return $date->getTimestamp();
    } else {
      watchdog('campaignion_webform2redhen', 'Tried to import date with an invalid format "!field".', array('!field' => $value), WATCHDOG_WARNING);
      return NULL;
    }
  }
}
