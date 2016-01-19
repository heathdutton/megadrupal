<?php

$api = 1;

$context = array(
  'to_echo' => 'Marvin',
);

$tasks['recur1'] = array(
  'depends' => array('recur2'),
);

$tasks['recur2'] = array(
  'depends' => array('recur3'),
);

$tasks['recur3'] = array(
  'depends' => array('recur1'),
);

$tasks['actionless'] = array(
);

$tasks['unknown-action'] = array(
  'action' => 'unknown', // Undefined action.
);

$tasks['unknown-action-callback'] = array(
  'action' => 'bad-callback',
);

$tasks['task-with-working-action'] = array(
  'action' => 'good-action',
);

$tasks['task-with-failing-action'] = array(
  'action' => 'failing-action',
);

$tasks['shell-action'] = array(
  'action' => 'shell',
  'command' => 'echo "Slartibartfast"',
);

$tasks['bad-arg-shell-action'] = array(
  'action' => 'shell',
  'cmd' => 'echo "Slartibartfast"',
);

$tasks['multiple-shell-action-simple'] = array(
  'action' => 'shell',
  'commands' => array('echo "Slartibartfast"', 'echo "Deriparamaxx"'),
);

$tasks['multiple-shell-action-params-keyed'] = array(
  'action' => 'shell',
  'commands' => array(
    'echo "Slartibartfast"' => array(),
    'echo "Deriparamaxx"' => array(),
  ),
);

$tasks['multiple-shell-action-params-flat'] = array(
  'action' => 'shell',
  'commands' => array(
    array('command' => 'echo', 'args' => array("Slartibartfast")),
    array('command' => 'echo', 'args' => array("Deriparamaxx")),
  ),
);

$tasks['failing-shell-action'] = array(
  'action' => 'shell',
  // The Unix false command which returns a non-zero exit code.
  'command' => 'false',
);

$tasks['param-echo'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array(context('to_echo')),
);

$tasks['context-shell-action'] = array(
  'depends' => 'param-echo',
  'context' => array(
    'to_echo' => 'Arthur Dent',
  ),
);

$tasks['context-shell-action2'] = array(
  'depends' => 'param-echo',
  'context' => array(
    'to_echo' => 'Ford Prefect',
  ),
);

$tasks['global-context-shell-action'] = array(
  'depends' => 'param-echo',
);

$tasks['multiple-contexts'] = array(
  'depends' => array('context-shell-action', 'context-shell-action2'),
);

$tasks['string-dependency'] = array(
  'depends' => 'shell-action',
);

$actions['good-action'] = array(
  'callback' => 'test_good_action',
);

$actions['failing-action'] = array(
  'callback' => 'test_failing_action',
);

$actions['bad-callback'] = array(
  'callback' => 'does not exist',
);

function test_good_action($context) {
  drush_print('Test..');
}

function test_failing_action($context) {
  return drush_set_error('NOGOOD', dt('Arg, failure.'));
}
