<?php

/**
 * Performs a touch() on specified path.
 * @param  [type] $path [description]
 * @return [type]       [description]
 */
function cdn_touch_path($path) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    // suppress possible permission errors
    if (@touch($path)) {
      drupal_set_message(t('Successfully touched %path.', array('%path' => $path)));
    }
    else {
      drupal_set_message(t('Failed to touch %path. Do you have sufficient permissions?', array('%path' => $path)), 'error');
    }
  }
}
