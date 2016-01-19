<?php

/**
 * Alternative to dpm() or debug() for printing just data within a single
 * iteration.
 *
 * This function should be used when attempting to debug information from
 * within loops that iterate several times. This function is most useful in the
 * cases where the data needed to be debugged are very large data structures
 * and take up a lot of memory.
 *
 * @param $elements
 *   The data to sample. Could be any data-type.
 *
 * @param $iteration
 *   The zero-based iteration count you want to sample.
 */
function hap_debug_sample($elements = array(), $iteration = 0) {
  // If we are sampling scalar values (bool, string, integer, etc.) we convert
  // it to an array and assign a generic $key value. This will prevent
  // accidental "already sampled" situations when we are attempting to sample
  // something with a common key (such as an integer value.)
  if (is_scalar($elements)) {
    $sample_delta = &drupal_static(__FUNCTION__ . '_delta', 0);
    $elements = array('Sample ' . $sample_delta => $elements);
  }

  foreach ($elements as $key => $element) {
    // Here we manage which iteration to debug. The iteration is specific to
    // the $key value of each element passed to this function.
    $i = &drupal_static(__FUNCTION__ . $key . $iteration, 0);
    if ($i < $iteration) {
      $i++;
    }
    else {

      // Here is where we manage whether the iteration we are seaking to sample
      // has been executed. This variable here is the heart of this function.
      $sampled = &drupal_static(__FUNCTION__ . $key, FALSE);

      if (!$sampled) {
        // We attempt to use Devel's dpm if it is available, but we don't want
        // this tool to depend on it entirely, so we use Drupal 7's built-in
        // debug() function.
        $func = function_exists('dpm') ? 'dpm' : 'debug';
        $func(element, $key);
        $sampled = TRUE;
      }
    }
  }
}
