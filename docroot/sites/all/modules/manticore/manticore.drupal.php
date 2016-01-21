<?php

/**
 * @file
 * Provides the Manticore PHP API using drupal_http_request().
 */

/**
 * The Manticore PHP API, using drupal_http_request().
 */
class ManticoreDrupal extends Manticore {
  public function call(array $arguments) {
    // Construct the arguments.
    $args = '';
    if (!isset($arguments['MTC_KEY'])) {
      $arguments['MTC_KEY'] = $this->key;
    }
    if (!isset($arguments['MTC_GROUP'])) {
      $arguments['MTC_GROUP'] = $this->group;
    }
    if (!isset($arguments['MTC_ID'])) {
      $arguments['MTC_ID'] = $this->id;
    }
    foreach ($arguments as $argument => $value) {
      if (!empty($value)) {
        $args .= $argument .'='. urlencode($value) .'&';
      }
    }
    $url = "http://{$this->server}/Data/{$arguments['MTC_GROUP']}/{$arguments['MTC_ID']}/{$arguments['MTC_KEY']}/mtcContactReg.aspx?$args";
    $result = drupal_http_request($url);

    // Check the result.
    if ($result->code == 200) {
      if ($result->data == Manticore::ErrorNone) {
        return $result->data;
      }
      else {
        throw new ManticoreException(Manticore::getErrorString($result->data), $result->data, $result, $result->data);
      }
    }
    else {
      throw new ManticoreException('Error contacting Manticore.', $result->code, $result, $result->data);
    }
  }
}
