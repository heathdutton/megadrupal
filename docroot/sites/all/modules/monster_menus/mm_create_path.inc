<?php
// $Id: mm_create_path.inc 5403 2011-05-11 13:47:37Z dan $

/**
 * @file
 * Functions to create all needed parts of a path in the MM tree
 */

class MMCreatePathCat {
  public
    $alias = '',
    $comment = 0,
    $default_mode = MM_PERMS_READ,
    $flags = '',
    $hidden = FALSE,
    $hover = '',
    $max_depth = -1,
    $max_parents = -1,
    $menu_start = -1,
    $mmtid,
    $name = '',
    $no_update,
    $node_info = 3,
    $perms,
    $previews = 0,
    $reset,
    $rss = 0,
    $theme = '',
    $type = 'cat',
    $uid,
    $weight = 0;
  public $cascaded = array(
    'allow_reorder' => -1,
    'allowed_themes' => NULL,
    'allowed_node_types' => NULL
  );

  public function __construct($arr) {
    foreach ($arr as $key => $val)
      if ($key == 'cascaded') {
        foreach ($val as $ckey => $cval)
          $this->cascaded[$ckey] = $cval;
      }
      else $this->$key = $val;
  }
}

class MMCreatePathGroup {
  public
    $alias = '',
    $comment = 0,
    $default_mode,
    $flags = '',
    $hidden = FALSE,
    $hover = '',
    $mmtid,
    $name = '',
    $no_update,
    $perms,
    $reset,
    $rss = 0,
    $theme = '',
    $type = 'group',
    $uid,
    $weight = 0;
  public
    $qfield,
    $members,
    $qfrom,
    $vgroup;

  public function __construct($arr) {
    // Work around PHP's inability to assign a default value using concatenation
    // to a public class variable.
    $this->default_mode = MM_PERMS_APPLY . ',' . MM_PERMS_READ;
    foreach ($arr as $key => $val)
      $this->$key = $val;
  }
}

/**
 * Create an entire path of MM tree IDs, including any missing parents
 *
 * @param $items
 *   An array of MMCreatePathCat and/or MMCreatePathGroup objects, in path order
 *
 *   In the perms field, a group can be set to 'self' to include the outer group
 *   in the list or just the mmtid, instead of its full path.
 *
 *   The uid field can be either numeric or the username.
 *
 *   In groups, the members list can contain either numeric uids or usernames.
 *
 *   Normally, each visited entry is cached and therefore only updated once per
 *   run. Set the 'reset' flag to force an update, even if the mmtid was cached.
 *
 *   Set 'no_update' to an array of field names which should only be changed if
 *   the tree ID is new, and not if it already exists.
 * @param $clear_cache
 *   If TRUE, clear the cache of items that have already been touched and
 *   return immediately.
 * @param $test
 *   If TRUE, go through the motions, but do not make any changes.
 * @param $add_only
 *   If TRUE, don't update existing items, just add anything new.
 * @param $stats
 *   (optional) Array with which to populate statistics:
 *   - pages:
 *     An array indexed by mmtid, containing an array of sub-arrays each with
 *     the elements "message" and "vars", which describe the pages that were
 *     acted upon.
 *   - groups:
 *     An array indexed by mmtid, containing an array of sub-arrays each with
 *     the elements "message" and "vars", which describe the groups that were
 *     acted upon.
 *   - errors:
 *     An array containing sub-arrays with the elements "message" and "vars",
 *     which describe any errors that occurred.
 *   A count of the number of pages acted upon can be derived using the count()
 *   function.
 * @return bool
 *   TRUE on success
 */
function mm_create_path(&$items, $clear_cache = FALSE, $test = FALSE, $add_only = FALSE, &$stats = 'undef') {
  $did_existing_perms = &drupal_static(__FUNCTION__, array());

  if ($clear_cache) {
    $did_existing_perms = array();
    $mmcp = &drupal_static('_mm_create_path', array());
    $mmcp = array();
    return TRUE;
  }

  if (is_array($items) && is_array($items[0])) {
    foreach ($items as &$item)
      if (!mm_create_path($item, FALSE, $test, $add_only)) return FALSE;

    return TRUE;
  }

  if (!count($items)) {
    _mm_report_error('Tried to create an empty item in mm_create_path()', array(), $stats);
    return FALSE;
  }

  $last = $items[count($items) - 1];
  if (!isset($last->mmtid)) {
    $path = $parms = $exists = array();
    foreach ($items as $item) {
      if (isset($item->mmtid)) {
        // PHP doesn't have a way to keep string keys that look like integers
        // from being converted to ints, so flag the true ints with \001.
        $path["\001" . $item->mmtid] = $item->alias;
        $did_existing_perms[$item->mmtid] = TRUE;
      }
      elseif (empty($item->name)) {
        _mm_report_error('This item has no name: !item', array('!item' => mm_var_export_html($item)), $stats);
        return FALSE;
      }
      else $path[$item->name] = $item->alias;
      $parms[$item->name][$item->alias] = $item;
    }

    $mmtid = _mm_create_path($path, '_mm_create_path_create', $parms, $exists, $test, $add_only, $stats);
    if (!$mmtid && $mmtid !== 'test') {
      $msg = $last->type == 'group' ? 'Failed to create group described by !item' : 'Failed to create entry described by !item';
      _mm_report_error($msg, array('!item' => mm_var_export_html($items)), $stats);
      return FALSE;
    }

    if ($exists) {
      $i = 0;
      foreach ($exists as $k => $v) {
        if (!$v) break;
        $items[$i++]->mmtid = $k;
      }
    }
    $last->mmtid = $mmtid;

    foreach ($items as $item) {
      if (isset($exists[$item->mmtid]) && $exists[$item->mmtid] && (!isset($did_existing_perms[$item->mmtid]) || !empty($item->reset))) {
        $item->uid = _mm_create_path_get_uid($item->uid, 1, $stats);

        if (is_array($item->no_update) && ($tree = mm_content_get($item->mmtid))) {
          unset($block);
          unset($vquery);
          foreach ($item->no_update as $nu) {
            if ($nu == 'mmtid') continue;
            if ($nu == 'menu_start' || $nu == 'max_depth' || $nu == 'max_parents') {
              if (empty($block)) {
                $select = db_select('mm_tree_block', 'b')
                  ->condition('b.mmtid', $item->mmtid);
                $select->addField('b', 'bid', 'menu_start');
                $select->fields('b', array('max_depth', 'max_parents'));
                $block = $select->execute()->fetchObject();
              }

              if (isset($block->$nu)) $item->$nu = $block->$nu;
              else $item->$nu = -1;
            }
            elseif ($nu == 'members') {
              $item->members = '';
            }
            elseif ($nu == 'qfrom' || $nu == 'qfield') {
              if (empty($vquery)) {
                $select = db_select('mm_group', 'g');
                $select->join('mm_vgroup_query', 'v', 'g.vgid = v.vgid');
                $select->fields('v');
                $select->condition('g.gid', $item->mmtid);
                $vquery = $select->execute()->fetchObject();
                if ($vquery) $vquery->qfield = $vquery->field;
              }
              if ($vquery && isset($vquery->$nu)) $item->$nu = $vquery->$nu;
            }
            elseif (isset($tree->$nu)) $item->$nu = $tree->$nu;
          }
        }
        $parms = _mm_create_path_clear_parms(clone($item), $stats);
        $parms->recurs_perms = FALSE;
        if (($parms->perms = _mm_create_path_create_perms($item, $test, $add_only, $stats)) === FALSE) {
          return FALSE;
        }
        if (!$add_only) {
          if ($test) {
            _mm_report_stat($item->type == 'group', $item->mmtid, 'Would have updated the @thing with mmtid=!mmtid', array(), $stats);
          }
          else if (!mm_content_insert_or_update(FALSE, $item->mmtid, $parms, $stats)) {
            return FALSE;
          }
        }

        $did_existing_perms[$item->mmtid] = TRUE;
      }
    }
  }

  return TRUE;
}

// ****************************************************************************
// * Private functions start here
// ****************************************************************************

/**
 * Helper function
 *
 * @param $path
 *   Array of name => alias pairs, or just names as the array keys, defining the
 *   path to create. If an array key starts with "\001", it is assumed to be
 *   the mmtid of an existing entry, which is then ignored.
 * @param $func
 *   Function to call, in order to create a given entry. The first parameter
 *   must be the mmtid of the new entry's parent. The second parameter is the
 *   new entry's name, and the third is its alias. The function must return the
 *   tree ID of the entry that was created, or 0 if there was an error.
 * @param $func_parms
 *   Parameters to pass to $func. This is passed by reference, so any changes
 *   made to it in $func are permanent.
 * @param $exists
 *   On completion, this array contains elements, in order, for each segment of
 *   the path, with TRUE for any that already existed.
 * @param $test
 *   If TRUE, go through the motions, but do not make any changes.
 * @param $add_only
 *   If TRUE, don't update existing items, just add anything new.
 * @param $stats
 *   Array to contain any error messages/statistics
 * @return
 *   The tree ID of the newly-created (or previously existing) entry
 */
function _mm_create_path($path, $func, &$func_parms, &$exists, $test, $add_only, &$stats) {
  $cache = &drupal_static(__FUNCTION__, array());

  if (!count($path)) return 1;       // topmost mmtid is root node

  $no_alias = FALSE;
  $elem = array_keys($path);         // name=>alias
  if ($elem[0] === 0) {              // no alias
    $no_alias = TRUE;
    $elem = array_values($path);
  }
  elseif ($elem[count($elem) - 1][0] === "\001") {
    // PHP doesn't have a way to keep string keys that look like integers
    // from being converted to ints, so this code expects mmtids to be preceded
    // with \001, instead. That way we can tell the difference between an mmtid
    // and an entry name containing only numbers.
    $string = substr($elem[count($elem) - 1], 1);
    $mmtid = $string === 'test' ? 'test' : intval($string);
    if (is_array($exists)) {
      array_pop($path);
      _mm_create_path($path, $func, $func_parms, $exists, $test, $add_only, $stats);
      $exists[$mmtid] = TRUE;
    }
    return $mmtid;
  }

  $longpath = implode('|:', $elem);
  if (!isset($cache[$longpath])) {
    if ($no_alias) {
      $current_name = array_pop($path);
      $current_alias = '';
    }
    else {
      $current_name = array_pop($elem);
      $current_alias = array_pop($path);
    }

    $parent = _mm_create_path($path, $func, $func_parms, $exists, $test, $add_only, $stats);
    if (!$parent) return $cache[$longpath] = 0;    // error

    if ($current_alias != '') {
      $tree = mm_content_get(array('parent' => $parent, 'alias' => $current_alias), array(), 1);
    }
    else {
      $tree = mm_content_get(array('parent' => $parent, 'name' => $current_name), array(), 1);
    }

    if (count($tree)) {
      if (is_array($exists)) $exists[$tree[0]->mmtid] = TRUE;
      $cache[$longpath] = $tree[0]->mmtid;
    }
    else {
      $cache[$longpath] = $func($parent, $current_name, $current_alias, $func_parms, $test, $add_only, $stats);
      if (is_array($exists) && $cache[$longpath]) $exists[$cache[$longpath]] = FALSE;
    }
  }
  elseif (is_array($exists)) {
    array_pop($path);
    _mm_create_path($path, $func, $func_parms, $exists, $test, $add_only, $stats);
    $exists[$cache[$longpath]] = $cache[$longpath] != 0;
  }

  return $cache[$longpath];
}

/**
 * Create a new MM tree entry
 *
 * @param $parent
 *   Tree ID of the parent, under which to create a new child
 * @param $name
 *   Human-readable name of the new child
 * @param $alias
 *   URL alias of the new child
 * @param $parms
 *   Other parameters passed to mm_content_insert_or_update()
 * @param $test
 *   If TRUE, go through the motions, but do not make any changes.
 * @param $add_only
 *   If TRUE, don't update existing items, just add anything new.
 * @param $stats
 *   Array to contain any error messages/statistics
 * @return int, bool
 *   The tree ID of the newly-created (or previously existing) entry, or FALSE
 *   on error.
 */
function _mm_create_path_create($parent, $name, $alias, $parms, $test, $add_only, &$stats) {
  // Note: $parms is already a reference, and PHP passes it to this function that way
  $item = &$parms[$name][$alias];
  if (isset($item->mmtid)) return $item->mmtid;

  $parms = _mm_create_path_clear_parms(clone($item), $stats);
  $parms->name = $name;
  $parms->alias = $alias;
  if (($parms->perms = _mm_create_path_create_perms($item, $test, $add_only, $stats)) === FALSE) {
    return FALSE;
  }
  $parms->uid = _mm_create_path_get_uid($parms->uid, 1, $stats);
  $parms->recurs_perms = FALSE;

  if ($test) {
    _mm_report_stat($item->type == 'group', $parent, 'Would have created the @thing with name=@name, alias=@alias', array('@name' => $name, '@alias' => $alias), $stats);
    return $item->mmtid = 'test';
  }
  return $item->mmtid = mm_content_insert_or_update(TRUE, $parent, $parms, $stats);
}

/**
 * Remove unneeded parameters which could cause mm_content_insert_or_update() to
 * complain
 *
 * @param $parms
 *   Parameters object to modify
 * @param $stats
 *   Array to contain any error messages/statistics
 * @return
 *   The modified parameters object
 */
function _mm_create_path_clear_parms($parms, &$stats) {
  if (isset($parms->members) && $parms->members !== '') {
    $new_members = array();
    if (is_array($parms->members)) {
      foreach ($parms->members as $m) {
        $uid = _mm_create_path_get_uid($m, FALSE, $stats);
        if ($uid !== FALSE) $new_members[] = $uid;
      }
    }
    $parms->members = $new_members;
  }
  unset($parms->mmtid);
  unset($parms->type);
  unset($parms->no_update);
  unset($parms->reset);
  unset($parms->vgroup);
  return $parms;
}

/**
 * Get the uid associated with a username
 *
 * @param $uid
 *   uid or username to resolve
 * @param $failure
 *   Message to return upon failure
 * @param $stats
 *   Array to contain any error messages/statistics
 * @return
 *   The resolved uid, or $failure
 */
function _mm_create_path_get_uid($uid, $failure, &$stats) {
  if (!isset($uid)) return 1;
  if (is_numeric($uid)) return $uid;

  $cache = &drupal_static(__FUNCTION__, array());

  if (isset($cache[$uid])) {
    if ($cache[$uid] === 'fail') return $failure;
    return $cache[$uid];
  }

  $u = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('u.name', $uid)
    ->execute()->fetchField();
  if ($u == '') {
    _mm_report_error("Could not find user '!uid' in users table", array('!uid' => $uid), $stats);
    $cache[$uid] = 'fail';
    return $failure;
  }

  return $cache[$uid] = $u;
}

/**
 * Create any groups referred to by another group or entry's permissions
 *
 * @param $item
 *   MMCreatePathCat or MMCreatePathGroup object describing the item for which
 *   groups are to be created
 * @param $test
 *   If TRUE, go through the motions, but do not make any changes.
 * @param $add_only
 *   If TRUE, don't update existing items, just add anything new.
 * @param $stats
 *   Array to contain any error messages/statistics
 * @return array
 *   An updated list of permissions, with the resulting group IDs
 */
function _mm_create_path_create_perms(&$item, $test, $add_only, &$stats) {
  $perms = array();
  foreach (array(MM_PERMS_WRITE, MM_PERMS_SUB, MM_PERMS_APPLY, MM_PERMS_READ) as $m) {
    if (is_array($item->perms) && isset($item->perms[$m]['groups'])) {
      foreach ($item->perms[$m]['groups'] as &$g) {
        if ($g === 'self') {
          $perms[$m]['groups'][] = 'self';
        }
        elseif (is_numeric($g)) {
          $perms[$m]['groups'][] = $g;
        }
        elseif (is_array($g) && $g) {
          $last = &$g[count($g) - 1];
          if (!isset($last->mmtid) && !mm_create_path($g, FALSE, $test, $add_only, $stats)) {
            return FALSE;
          }

          $perms[$m]['groups'][] = $last->mmtid;
        }
      }
    }

    if (isset($item->perms[$m]['users'])) {
      foreach ($item->perms[$m]['users'] as &$u) {
        if (($uid = _mm_create_path_get_uid($u, FALSE, $stats)) !== FALSE) {
          $u = $uid;
        }
      }

      $perms[$m]['users'] = $item->perms[$m]['users'];
    }
  }

  return $perms;
}
