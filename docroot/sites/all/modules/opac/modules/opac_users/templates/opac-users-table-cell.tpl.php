<?php

/**
 * @file
 * Template file for opac_users tables cells.
 */

if (isset($value) && $value != ''):
  if (isset($field['type']) && $field['type'] == 'date'):
    $value = format_date($value, variable_get('opac_date_type'));
  endif;
endif;

if (isset($value)):
  print ($value);
endif;
