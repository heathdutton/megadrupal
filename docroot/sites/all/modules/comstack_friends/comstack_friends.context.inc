<?php
/**
 * @file
 * comstack_friends.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function comstack_friends_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'comstack_friends';
  $context->description = '';
  $context->tag = 'comstack_friends';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'friends/*' => 'friends/*',
        '~friends/*/pending' => '~friends/*/pending',
        '~friends/*/blocked' => '~friends/*/blocked',
        'friends/*/find' => 'friends/*/find',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-5ef96dfba4d649b7fa2e34af088d2d64' => array(
          'module' => 'views',
          'delta' => '5ef96dfba4d649b7fa2e34af088d2d64',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('comstack_friends');
  $export['comstack_friends'] = $context;

  return $export;
}
