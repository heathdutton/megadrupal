<?php

$api = 1;

$tasks['optional-set'] = array(
  'action' => 'parameter_check',
  'optional' => 'set_value',
);

$tasks['optional-unset'] = array(
  'action' => 'parameter_check',
);

$tasks['optional-context'] = array(
  'action' => 'parameter_check',
  'optional' => context_optional('optional_context'),
);

$actions['parameter_check'] = array(
  'callback' => 'drake_parameter_check',
  'parameters' => array(
    'optional' => array(
      'description' => 'an optional parameter',
      'default' => 'default_value',
    ),
  ),
);

function drake_parameter_check($context) {
  if (isset($context['optional'])) {
    if (is_null($context['optional'])) {
      drush_print('Optional is NULL.');
    }
    else {
      drush_print('Optional has the value ' . $context['optional']. '.');
    }
  }
  else {
    drush_print('Optional is unset.');
  }
}
