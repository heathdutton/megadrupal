<?php

/**
 * @file
 * Class kaltura_views_handler_field_kaltura_duration.
 */

/**
 * Duration field handler class.
 */
class kaltura_views_handler_field_kaltura_duration extends views_handler_field {

  function render($values) {
    $secs = $values->node_kaltura_kaltura_duration;
    if ($secs > 60 * 60) {
      $hr             = (int) ($secs / 60);
      $hr             = ($hr < 10) ? '0' . $hr : $hr;
      $min            = (int) ($secs / 60 / 60);
      $min            = ($min < 10) ? '0' . $min : $min;
      $sec            = (int) (($secs / 60) % 60);
      $sec            = ($sec < 10) ? '0' . $sec : $sec;
      $durationoutput = ($hr . ':' . ($min) . ':' . ($sec));
    }
    else {
      $min            = (int) ($secs / 60);
      $min            = ($min < 10) ? '0' . $min : $min;
      $sec            = (int) ($secs % 60);
      $sec            = ($sec < 10) ? '0' . $sec : $sec;
      $durationoutput = ($min . ':' . ($sec));
    }
    return $durationoutput;
  }
}
