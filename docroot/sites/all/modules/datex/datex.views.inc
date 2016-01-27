<?php

/**
 * Helper function for (hook) datex_views_data_alter.
 */
function _datex_views_data_alter_recurse(&$data) {
  foreach ($data as $key => &$value) {
    if(is_array($value)) {
      _datex_views_data_alter_recurse($value);
    }
    elseif ($value == 'views_handler_field_date') {
      $value = 'datex_handler_field_date';
    }
  }
}

/**
 * Implements hook_views_data_alter().
 *
 * For swapping out date handlers with our handler.
 */
function datex_views_data_alter(&$data) {
  // In patching mode, All is handled with datex_format hook, Nothing to do
  // here.
  if (_datex_cfg('mode') == DATEX_NONPATCHING_MODE) {
    _datex_views_data_alter_recurse($data);
  }
}

