<?php

$api = 1;

$tasks['recursive-def'] = array(
  'depends' => 'recursive',
  'context' => array(
    'value' => 'recursive-value',
  ),
);

$tasks['recursive'] = array(
  'depends' => array('check-value'),
  'to-check' => context('value'),
  'context' => array(
    'value' => context('value'),
  ),
);

$tasks['recursive-undef'] = array(
  'depends' => array('check-value'),
  'context' => array(
    'value' => context('value'),
  ),
);

$tasks['check-value'] = array(
  'action' => 'context_check',
  'to-check' => context('value'),
);

$actions['context_check'] = array(
  'callback' => 'drake_context_check',
);

function drake_context_check($context) {
  $checks = $context['to-check'];
  if (!is_array($checks)) {
    $checks = array($checks);
  }
  foreach ($checks as $check) {
    if (isset($check)) {
      drush_print('Context is set with value: ' . $check);
    }
    else {
      drush_print('Context is not set.');
    }
  }
}
