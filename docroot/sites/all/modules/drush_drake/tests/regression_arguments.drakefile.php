<?php

$api = 1;

$tasks['multiple'] = array(
  'action' => 'context_check',
  'to-check' => array(
    drake_argument(1, "string to print"),
    drake_argument(2, "second string to print"),
  ),
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
