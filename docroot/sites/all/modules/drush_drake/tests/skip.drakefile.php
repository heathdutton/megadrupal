<?php

$api = 1;

$tasks['default'] = array(
  'depends' => array('task1', 'task2', 'task3', 'task4'),
);

$tasks['task1'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array('Task 1.'),
);

$tasks['task2'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array('Task 2.'),
);

$tasks['task3'] = array(
  'depends' => array('subtask1', 'subtask2'),
);

$tasks['subtask1'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array('Subtask 1.'),
);

$tasks['subtask2'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array('Subtask 2.'),
);

$tasks['task4'] = array(
  'action' => 'shell',
  'command' => 'echo',
  'args' => array('Task 4.'),
);
