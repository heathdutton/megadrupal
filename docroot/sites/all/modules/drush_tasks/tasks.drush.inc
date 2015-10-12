<?php

/**
 * Implementation of hook_drush_command().
 */
function tasks_drush_command() {
  $items['tasks-list'] = array(
    'description' => '',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
  );
  $items['tasks'] = array(
    'description' => '',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'taskname' => 'blah'
    ),
  );
  return $items;
}

/**
 * Ipmlementation of hook_drush_init().
 */
function tasks_drush_init() {
  if (file_exists($file)) {
    $file = getcwd() . '/Drushfile';
    $prev_funcs = get_defined_functions();
    include $file;
    $cur_funcs = get_defined_functions();
    $diff_funcs = array_diff($cur_funcs['user'], $prev_funcs['user']);
    tasks_tasks($diff_funcs);
  }
}

function tasks_tasks($new_tasks = array()) {
  static $tasks = array();
  foreach ($new_tasks as $function) {
    $task = new ReflectionFunction($function);
    $task_info = tasks_parse_annotations($task->getDocComment());
    if (isset($task_info['task'])) {
      $tasks[$function] = $task_info;
    }
  }
  return $tasks;
}

function drush_tasks($task_name) {
  $tasks = tasks_tasks();
  if (isset($tasks[$task_name])) {
    $task_name();
  }
}

function drush_tasks_list() {
  $tasks = tasks_tasks();
  $results = array();
  foreach ($tasks as $task => $info) {
    $results[$task] = $info['desc'][0];
  }
  drush_print_table(drush_key_value_to_array_table($results));
}

/**
 * Stolen from phpunit.
 *
 * @param  string $docblock
 * @return array
 */
function tasks_parse_annotations($docblock) {
  $annotations = array();

  if (preg_match_all('/@(?P<name>[A-Za-z_-]+)(?:[ \t]+(?P<value>.*?))?[ \t]*\r?$/m', $docblock, $matches)) {
    $numMatches = count($matches[0]);

    for ($i = 0; $i < $numMatches; ++$i) {
      $annotations[$matches['name'][$i]][] = $matches['value'][$i];
    }
  }

  return $annotations;
}
