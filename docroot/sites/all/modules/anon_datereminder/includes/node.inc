<?php

/**
 * @file
 * Support for node operations for Date Reminder. These are all administrative
 * operations (insert, update, delete). The common view operations are in
 * anon_datereminder.module.
 *
 * These are D7-format functions. They'll be called directly from core
 * in D7. They're called from anon_datereminder_node_api() in D6.
 */

/**
 * Implements hook_node_insert().
 */
function _anon_datereminder_node_insert($node) {
  if (isset($node->anon_datereminder_enabled)) {
    module_load_include('inc', 'anon_datereminder', 'includes/defines');

    $en = $node->anon_datereminder_enabled;
    if ($en != ANON_DATEREMINDER_TYPE_DISABLED) {
      module_load_include('inc', 'anon_datereminder', ANON_DATEREMINDER_DB);
      _anon_datereminder_set_node_enabled($node, $en);
    }
  }
}

/**
 * Implements hook_node_update().
 */
function _anon_datereminder_node_update($node) {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  $enabled = _anon_datereminder_type_enabled($node->type);

  if ($enabled >= ANON_DATEREMINDER_TYPE_ALLOWED) {
    module_load_include('inc', 'anon_datereminder', ANON_DATEREMINDER_DB);
    _anon_datereminder_set_node_enabled($node, $node->anon_datereminder_enabled);
    _anon_datereminder_update_node_reminders($node);
  }
}


/**
 * Update all reminders for a node if the node is updated.
 *
 * @param node $node
 *   Node that's being written back.
 */
function _anon_datereminder_update_node_reminders($node) {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  module_load_include('inc', 'anon_datereminder', ANON_DATEREMINDER_DB);
  $rems = _anon_datereminder_load_reminders(array('nid' => $node->nid), $node);
  _anon_datereminder_update_reminder_nexts($rems);
}

/**
 * Recompute "next" time and write back a group of reminders.
 *
 * This might be reminders associated with a node when the node changes,
 * or a set of reminders whose time has expired and we need to move on
 * to the next occurrence.
 *
 * Note that reminders that are past last occurrence of this node
 * will be deleted.
 *
 * @param array $rems
 *   list of reminders
 */
function _anon_datereminder_update_reminder_nexts($rems) {
  module_load_include('inc', 'anon_datereminder', 'includes/date');
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  module_load_include('inc', 'anon_datereminder', ANON_DATEREMINDER_DB);
  foreach ($rems as $r) {
    _anon_datereminder_complete_reminder($r);
    _anon_datereminder_get_next_reminder($r);
  }
  _anon_datereminder_set_reminders($rems);
}
