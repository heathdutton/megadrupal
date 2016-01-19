<?php

$api = 1;

$tasks['simple'] = array(
  'action' => 'context_check',
  'to-check' => drake_argument(1, "string to print"),
);

$tasks['simple-optional'] = array(
  'action' => 'context_check',
  'to-check' => drake_argument_optional(1, "string2 to print"),
);

$tasks['simple-optional-default'] = array(
  'action' => 'context_check',
  'to-check' => drake_argument_optional(1, "string3 to print", 'default-value'),
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
