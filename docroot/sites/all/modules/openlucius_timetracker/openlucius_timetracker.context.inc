<?php
/**
 * @file
 * openlucius_timetracker.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function openlucius_timetracker_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'active_timers';
  $context->description = '';
  $context->tag = 'Timetracker';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '*' => '*',
        '~user/dashboard/*' => '~user/dashboard/*',
        '~group-users/*' => '~group-users/*',
        '~recent-stuff' => '~recent-stuff',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'openlucius_timetracker-openlucius_active_timers' => array(
          'module' => 'openlucius_timetracker',
          'delta' => 'openlucius_active_timers',
          'region' => 'sidebar_second',
          'weight' => '9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Timetracker');
  $export['active_timers'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'budget_remaining';
  $context->description = '';
  $context->tag = 'Timetracker';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'ol_todo' => 'ol_todo',
        'ol_todo_list' => 'ol_todo_list',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
    'path' => array(
      'values' => array(
        'node/*/time' => 'node/*/time',
        'reports*' => 'reports*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'openlucius_timetracker-openlucius_budget_remaining' => array(
          'module' => 'openlucius_timetracker',
          'delta' => 'openlucius_budget_remaining',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Timetracker');
  $export['budget_remaining'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'openlucius_timetracker';
  $context->description = 'Places the openlucius_timetracker block in the secondary sidebar region.';
  $context->tag = 'Todo';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '*' => '*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'openlucius_timetracker-openlucius_timetracker_timer' => array(
          'module' => 'openlucius_timetracker',
          'delta' => 'openlucius_timetracker_timer',
          'region' => 'footer',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Places the openlucius_timetracker block in the secondary sidebar region.');
  t('Todo');
  $export['openlucius_timetracker'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'reports_filter';
  $context->description = '';
  $context->tag = 'Timetracker';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'reports' => 'reports',
        'node/*/time/*' => 'node/*/time/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'openlucius_timetracker-openlucius_filter' => array(
          'module' => 'openlucius_timetracker',
          'delta' => 'openlucius_filter',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Timetracker');
  $export['reports_filter'] = $context;

  return $export;
}
