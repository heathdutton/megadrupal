<?php

$api = 1;

$tasks['run-tests'] = array(
  'action' => 'shell',
  'command' => 'phpunit',
  'args' => array(
    '--colors',
    '--bootstrap=' . dirname(DRUSH_COMMAND) . '/tests/drush_testcase.inc',
    context_optional('--filter=test[filter]'),
    context_optional('--coverage-html=[coverage]'),
    'tests/drakeTest.php',
  ),
);

$tasks['coverage'] = array(
  'depends' => 'run-tests',
  'context' => array(
    'coverage' => '/tmp/drake_coverage',
  ),
);
