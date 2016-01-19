<?php
/**
 * @file
 * openlucius_events.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function openlucius_events_extras_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'event';
  $context->description = 'Displays the attend block for an event';
  $context->tag = 'event';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'ol_event' => 'ol_event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'openlucius_events_extras-openlucius_events_flag' => array(
          'module' => 'openlucius_events_extras',
          'delta' => 'openlucius_events_flag',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'openlucius_events_extras-openlucius_events_location' => array(
          'module' => 'openlucius_events_extras',
          'delta' => 'openlucius_events_location',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Displays the attend block for an event');
  t('event');
  $export['event'] = $context;

  return $export;
}
