<?php
/**
 * @file
 * nm_events.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_events_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_events_landing';
  $context->description = 'Blocks for Events on home page';
  $context->tag = 'events';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'landing/events' => 'landing/nm_events',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_events-mini_calendar' => array(
          'module' => 'views',
          'delta' => 'nm_events-mini_calendar',
          'region' => 'sidebar_first',
          'weight' => '-35',
        ),
        'views-nm_galleries-recent' => array(
          'module' => 'views',
          'delta' => 'nm_galleries-recent',
          'region' => 'sidebar_first',
          'weight' => '-34',
        ),
        'views-nm_testimonials-block' => array(
          'module' => 'views',
          'delta' => 'nm_testimonials-block',
          'region' => 'sidebar_first',
          'weight' => '-33',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  
  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Events on home page');
  t('events');

  $export['nm_events_landing'] = $context;

  $context = new stdClass();
  $context->disabled = TRUE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_events_map';
  $context->description = 'The map block';
  $context->tag = 'events';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'nm_event' => 'nm_event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'addressfield_staticmap-addressfield_staticmap' => array(
          'module' => 'addressfield_staticmap',
          'delta' => 'addressfield_staticmap',
          'region' => 'content',
          'weight' => '10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('The map block');
  t('events');
  $export['nm_events_map'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_events_pages';
  $context->description = 'Blocks for Events';
  $context->tag = 'events';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'events*' => 'events*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_events-mini_calendar' => array(
          'module' => 'views',
          'delta' => 'nm_events-mini_calendar',
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Blocks for Events');
  t('events');
  $export['nm_events_pages'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_events_node';
  $context->description = 'On Event nodes.';
  $context->tag = 'events';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'nm_event' => 'nm_event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'events',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('On Event nodes.');
  t('events');
  $export['nm_events_node'] = $context;

  return $export;
}
