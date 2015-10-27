<?php
/**
 * @file
 * Miscellaneous tools
 */

/**
 * Trim and strip text
 *
 * @param $value
 * @param $length
 *
 * @return mixed|string
 */
function xu_trim_and_strip($value, $length) {
  $value = strip_tags($value);
  // Use views to do this if we have is insatlled
  if (function_exists('views_trim_text')) {
    return module_invoke('views', 'trim_text', array('max_length'   => $length,
      'word_boundary' => 1,
      'ellipsis'      => 1,
      'html'          => 1), $value);
  }
  // If not, we'll do it ourseleves, it a slightly more messy way, not going to
  // do much here, because we should always have views
  else {
    return trim(substr($value, 0, $length)) . '...';
  }
}
