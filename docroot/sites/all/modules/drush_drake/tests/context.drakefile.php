<?php

$api = 1;

$context = array(
  'context1' => 'value1',
  'context2' => 'value2',
  'context3' => 'value3',
);

$tasks['simple'] = array(
  'action' => 'print',
  'output' => context('context1'),
);

$tasks['replaced'] = array(
  'action' => 'print',
  'output' => context('[context1]'),
);

$tasks['replaced-with-extra'] = array(
  'action' => 'print',
  'output' => context('before [context1] after'),
);

$tasks['string-manipulation'] = array(
  'action' => 'print',
  'output' => context('before [context1:string:upcase] after'),
);

$tasks['unknown-manipulation'] = array(
  'action' => 'print',
  'output' => context('before [context1:upcase] after'),
);

$tasks['optional-context-set'] = array(
  'depends' => 'optional-context',
  'context' => array(
    'test' => 'ocs-value',
  ),
);

$tasks['optional-context-unset'] = array(
  'depends' => 'optional-context',
  'context' => array(
  ),
);

$tasks['optional-context'] = array(
  'action' => 'context_check',
  'to-check' => context_optional('test'),
);

$tasks['optional-context-set-default'] = array(
  'depends' => 'optional-context-default',
  'context' => array(
    'test' => 'ocsd-value',
  ),
);

$tasks['optional-context-unset-default'] = array(
  'depends' => 'optional-context-default',
  'context' => array(
  ),
);

$tasks['optional-context-default'] = array(
  'action' => 'context_check',
  'to-check' => context_optional('test', 'ocd-default-value'),
);

$tasks['override-context'] = array(
  'action' => 'context_check',
  'to-check' => context('context1'),
);

$tasks['context-dependency'] = array(
  'depends' => array(context('context-dependency-[sub]')),
  'context' => array(
    'sub' => 'sub1',
  ),
);

$tasks['context-dependency2'] = array(
  'depends' => array(context('context-dependency-[sub]')),
);

$tasks['context-dependency-sub1'] = array(
  'action' => 'print',
  'output' => 'sub1 run',
);

$tasks['context-dependency-sub2'] = array(
  'action' => 'print',
  'output' => 'sub2 run',
);

$actions['print'] = array(
  'callback' => 'drake_print',
);

$actions['context_check'] = array(
  'callback' => 'drake_context_check',
);

function drake_print($context) {
  drush_print($context['output']);
}

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
