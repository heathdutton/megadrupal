<?php

/**
 * @file
 * This file contains all of the functions that deal with the database
 * for Date Reminder.
 */

module_load_include('inc', 'anon_datereminder', 'includes/defines');

/**
 * Clean up any references to a given node or nodes.
 *
 * @param array $nids
 *   nid (or array of nids) to clean up.
 * @param int $en
 *   If not NULL, also remove this nid from {anon_datereminder_enable}.
 *   Defaults to NULL.
 *
 * Note, we'll clean reminders even if this type didn't acutally have
 * reminders enabled. It's always possible that something messed up and
 * left some reminders lying around.
 */
function _anon_datereminder_clean_node_reminders($nids, $en = NULL) {
  if (!is_array($nids)) {
    $nids = array($nids);
  }
  db_delete('anon_datereminder')
    ->condition('nid', $nids, 'IN')
    ->execute();

  if (isset($en)) {
    db_delete('anon_datereminder_enable')
      ->condition('nid', $nids, 'IN')
      ->execute();
  }
}

/**
 * Clean out reminders for everything associated with this type.
 *
 * @todo
 *   Note that this is for when reminders are disabled for a type.
 */
function _anon_datereminder_clean_type_reminders($type) {
  if (_anon_datereminder_type_enabled($type) != ANON_DATEREMINDER_TYPE_DISABLED) {

    // There must be a way to do this using a join, but I can't figure it out.
    $q = db_select('node', 'n')
            ->fields('n', 'nid')
            ->condition('type', $type)
            ->execute();
    $nids = array();
    while ($n = $q->fetchObject()) {
      $nids[] = $n->nid;
    }
    if (count($nids) > 0) {
      _anon_datereminder_clean_node_reminders($nids);
    }
  }
}

/**
 * Enable/disable reminders for a node.
 *
 * Note that this should only be used for node types with reminders
 * enabled. (Or at least in RETAIN state.) If a type is set to DISASBLED,
 * all node-specific information should be cleaned out by calling
 * _anon_datereminder_clean_type_reminders().
 *
 * @param node $node
 *   The node
 * @param int $enabled
 *   Value to set enable flag to for this node. If NULL, remove all
 *   record of this node in the anon_datereminder tables.
 *
 * @todo
 *   This ought to use the database apis instead of the explicit
 *   insert...duplicate key. Probably best to try an update first,
 *   then an insert if that fails. It will only fail once per node.
 *
 *   Probably most of this except the actual database part should be moved
 *   to node.inc.
 */
function _anon_datereminder_set_node_enabled($node, $enabled = NULL) {
  $nid = $node->nid;
  switch ($enabled) {
    case NULL:
    case ANON_DATEREMINDER_TYPE_DISABLED:
      _anon_datereminder_clean_node_reminders($nid, $enabled);
      break;

    default:
      $d = db_merge('anon_datereminder_enable')
        ->key(array('nid' => $nid))
        ->fields(array('enabled' => $enabled));

      $d->execute();
      break;
  }
}


/**
 * Load node enable status from the database.
 *
 * @param int $nid
 *   nid of the node.
 *
 * @return int
 *   Returns the anon_datereminder_TYPE_* value for this node from
 *   {anon_datereminder_enable}. If NULL, this nid isn't in the database.
 */
function _anon_datereminder_get_node_enabled($nid) {

  $q = db_query("SELECT enabled FROM {anon_datereminder_enable}
    WHERE nid = :nid", array(
    ':nid' => $nid,
  ));

  $e = $q->fetchObject();

  if ($e) {
    return $e->enabled;
  }

  return NULL;
}

/**
 * Load reminders filtering one one or more keys.
 *
 * @param array $selectors
 *   a key:value array specifying what to load from
 *   the reminders table. Anything in the reminder table is fair game, but
 *   generally the keys are 'nid', 'rid',  or 'uid'.
 * @param node $node
 *   Optional parameter. If set, $r->node is set for any loaded reminder
 *   with the right nid.
 * @param string $sortkey
 *   Optional parameter. If set, returned nodes are sorted in this order.
 *
 * @return array
 *   an array of reminder objects as selected from the database, keyed by rid.
 *   Each object also includes the node title. Order is as
 *   specified by sortkey.
 *
 * @example:
 *   $reminders = _anon_datereminder_load_reminders(array('uid' => $uid));
 */
function _anon_datereminder_load_reminders(
        $selectors,
        $node = NULL,
        $sortkey = NULL) {
  $ret = array();

  $q = db_select('anon_datereminder', 'r');
  $q->join('node', 'n', 'r.nid = n.nid');
  $q->fields('r');
  $q->fields('n', array('title'));

  foreach ($selectors as $k => $v) {
    $q->condition("r.$k", $v);
  }

  if ($sortkey != NULL) {
    $q->orderBy("r.$sortkey");
  }

  $rems = $q->execute()->fetchAll();

  // Return is array keyed by rid.
  $ret = array();
  foreach ($rems as $r) {
    $ret[$r->rid] = $r;
  }

  return $ret;
}

/**
 * Write back reminder information.
 */
function _anon_datereminder_set_reminders(&$reminders) {
  foreach (array_keys($reminders) as $k) {
    $r = $reminders[$k];
    // Should we save this reminder?
    if (($r->leadtime == 0) || !isset($r->next_due)) {
      // No reminder, so delete any existing entry.
      if ($r->rid > 0) {
        db_delete('anon_datereminder')
          ->condition('rid', $r->rid)
          ->execute();
      }
      unset($reminders[$k]);
    }
    else {
      if (!isset($r->expired)) {
        $r->expired = 0;
      }
      $fvals = array(
        'nid' => $r->nid,
        'leadtime' => $r->leadtime,
        'next_due' => $r->next_due,
        'email' => $r->email,
        'expired' => $r->expired,
      );

      if ($r->rid > 0) {
        $q = db_merge('anon_datereminder')
          ->key(array('rid' => $r->rid))
          ->fields($fvals)
          ->execute();
      }
      else {
        // This is a new entry.
        $q = db_insert('anon_datereminder')
          ->fields($fvals);
        $rid = $q->execute();
        if ($rid > 0) {
          $r->rid = $rid;
          // Update worked, so adjust array indix to reflect new rid.
          unset($reminders[$k]);
          $reminders[$rid] = $r;
        }
      }
    }
  }
}

/**
 * Delete a list of reminder ids.
 *
 * @param array $rids
 *   List of rids to delete.
 */
function _anon_datereminder_delete_rids($rids) {
  db_delete('anon_datereminder')
    ->condition('rid', $rids, 'IN')
    ->execute();
}

/**
 * Return array of reminders with next_due older than $timestamp.
 *
 * @param int $dt
 *   Timestamp for due reminders.
 *
 * @return array
 *   Array of past-due reminders, indexed by rid.
 */
function _anon_datereminder_get_due_reminders($dt) {

  $q = db_select('anon_datereminder', 'r')
          ->fields('r')
          ->condition('next_due', intval($dt), '<=');

  $rems = $q->execute()->fetchAll();

  // Return is array keyed by rid.
  $ret = array();
  foreach ($rems as $r) {
    $ret[$r->rid] = $r;
  }

  return $ret;
}
