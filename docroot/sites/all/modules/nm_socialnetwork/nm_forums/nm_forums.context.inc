<?php
/**
 * @file
 * nm_forums.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function nm_forums_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'nm_forums_node';
  $context->description = 'On Forum nodes.';
  $context->tag = 'forums';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'forum' => 'forum',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'menu' => 'forum',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('On Forum nodes.');
  t('forums');
  $export['nm_forums_node'] = $context;

  return $export;
}
