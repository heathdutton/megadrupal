<?php

/**
 * @file
 * Drush commands for Context Per Organic Group.
 *
 * @package context_per_og
 */

/**
 * Implements hook_drush_command().
 */
function context_per_og_drush_command() {
  $items = array();

  $items['cpo-regenerate'] = array(
    'description' => "Regenerates all contexts created by context_per_og.",
    'aliases' => array('cpor'),
  );

  return $items;
}

/**
 * Implements drush_COMMANDFILE_COMMANDNAME().
 */
function drush_context_per_og_cpo_regenerate() {
  drush_print(dt('This will delete and recreate all contexts created by Context Per OG. All customizations to those contexts will be lost.'));
  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_user_abort();
  }

  module_load_include('inc', 'context_per_og');
  ctools_include('export');
  ctools_include('cleanstring');

  // Load array of all existing context_per_og contexts.
  $query = db_select('context_per_og', 'cpo');
  $query->fields('cpo', array(
    'context_name',
    'group_eid',
    'entity_type',
  ));
  $context_records = $query->execute()->fetchAllAssoc('context_name');

  foreach ($context_records as $context_name => $context_record) {
    // Load existing context into memory.
    $context = context_load($context_name);
    if ($context) {
      // Load organic group entity for this context.
      $group = entity_load_single($context_record->entity_type, $context_record->group_eid);
      if ($group) {
        // Delete and re-create context.
        context_delete($context);
        $context = context_per_og_generate_context($group, $context_record->entity_type);
        $saved = context_save($context);
        if (empty($saved)) {
          watchdog('context_per_og', 'Cannot re-create context @context_name, could not save context: @context.', array(
            '@context_name' => $context_name,
            '@context' => print_r($context, TRUE),
          ));
        }
      }
      else {
        watchdog('context_per_og', 'Cannot re-create context @context, entity with ID @entity_id no longer exists. Removing record from context_per_og.', array('@context' => $context_name, '@entity_id' => $context_record->group_eid));
        context_per_og_delete_context_by_name($context_name);
      }
    }
    else {
      watchdog('context_per_og', 'Cannot re-create context @context_name, context does not exist. Removing record from context_per_og.', array('@context_name' => $context_name));
      context_per_og_delete_context_by_name($context_name);
    }
  }
}
