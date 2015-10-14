<?php
/**
 * @file
 * nm_activity.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_activity_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_member_activity';
  $context->description = '';
  $context->tag = 'members';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'all',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-nm_member_activity-block_1' => array(
          'module' => 'views',
          'delta' => 'nm_member_activity-block_1',
          'region' => 'sidebar_first',
          'weight' => '27',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('members');
  $export['nm_member_activity'] = $context;

  return $export;
}
