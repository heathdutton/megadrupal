<?php

/**
 * Truncates a string to a maximum specified length on the last space character.
 * @param  [type]  $input    [description]
 * @param  integer $length   [description]
 * @param  array   $options  [description]
 * @return [type]            [description]
 */
function truncate($input, $length = 140, $options = array()) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $output = $input;

    if (strlen($output) > $length) {
      $options += array(
        'ellipsis' => '...',
        'format' => 'plain_text',
      );

      if ($options['format'] == 'plain_text') {
        $output = strip_tags($output);
      }

      $output = check_markup($output, $options['format']);
      $output = substr($output, 0, $length);

      if ($space = strrpos($output, ' ')) {
        $output = substr($output, 0, $space);
      }

      $output .= $options['ellipsis'];
    }

    return $output;
  }
}

/**
 * Runs the specified text value/format through check_markup().
 * @param  [type] $text [description]
 * @return [type]       [description]
 */
function filter_text($text) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    return check_markup($text['value'], $text['format']);
  }
}

/**
 * Provides default input format settings for Full HTML filter.
 * @return [type] [description]
 */
function input_default_full_html() {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $defaults = array(
      'value' => '',
      'format' => 'full_html',
    );

    return $defaults;
  }
}