<?php

/**
 * @file
 * Menu-specific functions
 */

function _mm_menu_access_any_admin() {
  return
    user_access('administer all groups') ||
    user_access('administer all users') ||
    user_access('administer all menus');
}

function _mm_menu_access_all_admin() {
  return
    user_access('administer all groups') &&
    user_access('administer all users') &&
    user_access('administer all menus');
}

function _mm_menu_access_is_admin() {
  global $user;
  return $user->uid == 1;
}

function _mm_menu_access_node_delete($node) {
  return node_access('delete', $node) && mm_content_user_can_node($node, MM_PERMS_APPLY) && (mm_content_node_is_recycled($node, MM_NODE_RECYCLED_MMTID_CURR) ? user_access('delete permanently') : TRUE);
}

function _mm_menu_access_node_restore($mmtid, $node) {
  return mm_content_node_is_recycled($node, $mmtid) && node_access('delete', $node) && mm_content_user_can($mmtid, MM_PERMS_IS_RECYCLE_BIN);
}

function _mm_menu_title_node_delete($node) {
  return mm_content_node_is_recycled($node, MM_NODE_RECYCLED_MMTID_CURR) ? 'Delete permanently' : 'Delete';
}

function _mm_menu_access_node_revisions($node) {
  if (!node_access('update', $node) || !node_access('view', $node) ||
      (!user_access('view revisions') && !user_access('bypass node access'))) {
    return FALSE;
  }
  $num_versions = db_select('node_revision', 'r')
    ->fields('r', array('vid'))
    ->condition('r.nid', $node->nid)
    ->countQuery()->execute()->fetchField();
  return $num_versions > 1;
}

function _mm_menu_access_group_viewall($mmtid) {
  global $user;
  $perms = mm_content_user_can($mmtid);
  return $user->uid > 0 && $perms[MM_PERMS_READ] && $perms[MM_PERMS_IS_GROUP];
}

function _mm_menu_access_add($mmtid) {
  $perms = mm_content_user_can($mmtid);
  if ($perms[MM_PERMS_APPLY] && !$perms[MM_PERMS_IS_GROUP] && !$perms[MM_PERMS_IS_RECYCLED] && !mm_content_is_archive($mmtid)) {
    // Make sure the user can create at least one type of content here
    $allowed_node_types = mm_content_resolve_cascaded_setting('allowed_node_types', $mmtid, $types_at, $types_parent);
    $types = node_type_get_types();
    if (!user_access('administer all menus')) {
      $types = array_intersect_key($types, array_flip($allowed_node_types));
    }
    foreach ($types as $type) {
      if (node_access('create', $type->type)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

function _mm_menu_access_settings_tab($mmtid) {
  global $user;
  $perms = mm_content_user_can($mmtid);
  return $perms[MM_PERMS_WRITE] || ($perms[MM_PERMS_SUB] || $perms[MM_PERMS_APPLY] && (!isset($user->user_portal_mmtid) || $mmtid != $user->user_portal_mmtid) && (!$perms[MM_PERMS_IS_GROUP] || $perms[MM_PERMS_READ]) && !$perms[MM_PERMS_IS_RECYCLED]);
}

function _mm_menu_access_empty_bin($mmtid) {
  $perms = mm_content_user_can($mmtid);
  return $perms[MM_PERMS_IS_RECYCLE_BIN] && mm_content_user_can_recycle($mmtid, 'EMPTY');
}

function _mm_menu_access_edit($mmtid) {
  $perms = mm_content_user_can($mmtid);
  return !$perms[MM_PERMS_IS_RECYCLE_BIN] && $perms[MM_PERMS_WRITE];
}

function _mm_menu_access_copy($mmtid) {
  $perms = mm_content_user_can($mmtid);
  return !$perms[MM_PERMS_IS_RECYCLE_BIN] && ($perms[MM_PERMS_WRITE] || $perms[MM_PERMS_SUB] || $perms[MM_PERMS_APPLY]) && !mm_content_is_archive($mmtid) && user_access('use tree browser');
}

function _mm_menu_access_restore($mmtid) {
  $parent = mm_content_get_parent($mmtid);
  $perms = mm_content_user_can($mmtid);
  return $perms[MM_PERMS_WRITE] && $perms[MM_PERMS_IS_RECYCLED] && mm_content_user_can($parent, MM_PERMS_IS_RECYCLE_BIN);
}

function _mm_menu_title_settings_delete($mmtid) {
  $perms = mm_content_user_can($mmtid);
  return $perms[MM_PERMS_IS_RECYCLED] || $perms[MM_PERMS_IS_GROUP] ? 'Delete permanently' : 'Delete';
}

function _mm_menu_access_delete($mmtid) {
  $tree = mm_content_get_tree($mmtid, array(MM_GET_TREE_RETURN_PERMS => TRUE, MM_GET_TREE_RETURN_FLAGS => TRUE, MM_GET_TREE_DEPTH => 0));
  return isset($tree[0]) && $tree[0]->perms[MM_PERMS_WRITE] && (user_access('administer all menus') || !isset($tree[0]->flags['limit_delete'])) && !$tree[0]->perms[MM_PERMS_IS_RECYCLE_BIN] && (!$tree[0]->perms[MM_PERMS_IS_RECYCLED] || user_access('delete permanently'));
}

function _mm_menu_title_settings_sub($mmtid) {
  return t('Add @subthing', mm_ui_strings(mm_content_is_group($mmtid)));
}

function _mm_menu_access_sub($mmtid) {
  $perms = mm_content_user_can($mmtid);
  return $perms[MM_PERMS_SUB] && !mm_content_is_node_content_block($mmtid) && $perms[MM_PERMS_SUB] && !$perms[MM_PERMS_IS_RECYCLED];
}

function _mm_menu_access_node_reorder($mmtid) {
  $perms = mm_content_user_can($mmtid);
  return $mmtid > 0 && !$perms[MM_PERMS_IS_RECYCLED] && $perms[MM_PERMS_WRITE] && $mmtid != mm_content_users_mmtid() && !mm_content_is_group($mmtid) && !mm_content_is_archive($mmtid) && !mm_content_is_archive($mmtid, TRUE);
}

function _mm_menu_access_menu_reorder($mmtid) {
  $perms = mm_content_user_can($mmtid);
  if ($mmtid > 0 && !$perms[MM_PERMS_IS_RECYCLED] && $perms[MM_PERMS_WRITE] && $mmtid != mm_content_users_mmtid() && !mm_content_is_group($mmtid) && !mm_content_is_node_content_block($mmtid) && !mm_content_is_archive($mmtid)) {
    return user_access('administer all menus') || mm_content_resolve_cascaded_setting('allow_reorder', $mmtid, $reorder_at, $reorder_parent);
  }
  return FALSE;
}

function _mm_menu_access_solver($mmtid) {
  return user_access('use permissions solver') && user_access('access user profiles') && mm_content_user_can($mmtid, MM_PERMS_APPLY);
}

function _mm_menu_show_node($node) {
  $add = '';
  if (strncmp($_GET['q'], 'mm/', 3) !== 0) {
    $add = "/$_GET[q]";
  }
  mm_redirect_to_node($node->nid, $add);
  return mm_node_page_view($node);
}

function _mm_menu_active_item($path = NULL) {
  $_mm_page_args = &drupal_static('_mm_page_args');
  $_mm_mmtid_of_node = &drupal_static('_mm_mmtid_of_node');

  module_load_include('module', 'node', 'node');  // needed for node_load_multiple() in mm_content_user_can_recycle() and below

  if (!is_scalar($path)) $path = NULL;

  $cache = &drupal_static(__FUNCTION__, array());
  if (!isset($cache[$path])) {
    $cache[$path] = (object)array();
    $arg = arg(NULL, $path);
    if ($arg[0] == 'node') {
      if (isset($arg[1])) {
        $nid = $arg[1];
      }
    }
    elseif ($arg[0] == 'comment' && isset($arg[2])) {
      if ($arg[1] == 'reply') {
        $nid = $arg[2];
      }
      elseif (in_array($arg[2], array('delete', 'edit', 'approve', 'view'))) {
        if (intval($arg[1]) && function_exists('comment_load') && ($row = comment_load(intval($arg[1])))) {
          $nid = $row->nid;
        }
      }
    }

    if (isset($nid)) {
      $nid = intval($nid);
      if ($nid) {
        $mmtids = mm_content_get_by_nid($nid);
        if (!empty($mmtids)) {
          $found_nid = $nid;
          $cache[$path]->mmtid = $mmtids[0];
          if (!$path) $_mm_page_args = 'mm/' . $mmtids[0];
        }
      }
    }
    elseif ($arg[0] == 'mm') {
      $resolved = FALSE;
      if (isset($arg[2]) && $arg[2] == 'node' && isset($arg[3]) && is_numeric($arg[3])) {
        $found_nid = db_select('node', 'n')
          ->fields('n', array('nid'))
          ->condition('n.nid', $arg[3])
          ->execute()->fetchField();
        if ($found_nid && $found_nid == $arg[3]) {
          $resolved = TRUE;
          if (_mm_resolve_archive($arg[1]) && !$path) {
            $_mm_page_args = "mm/$arg[1]";
          }
          $_mm_mmtid_of_node[$found_nid] = $arg[1];
        }
        else {
          unset($found_nid);
        }
      }

      // If $resolved==TRUE, _mm_resolve_archive() was already called.
      if (!isset($found_nid) && ($resolved || _mm_resolve_archive($arg[1]))) {
        $cache[$path]->mmtid = $arg[1];
        if (empty($path)) $_mm_page_args = "mm/$arg[1]";
      }
    }
    $cache[$path]->nid = isset($found_nid) ? $found_nid : NULL;
  }
  return $cache[$path];
}

function mm_mmtid_load($mmtid) {
  $set_breadcrumb = &drupal_static(__FUNCTION__, FALSE);

  $mmtid = intval($mmtid);
  if ($mmtid) {
    $test = "mm/$mmtid";
    if (!$set_breadcrumb && ($test == $_GET['q'] || drupal_substr($_GET['q'], 0, drupal_strlen($test) + 1) == "$test/")) {
      $mmtids = mm_content_get_parents_with_self($mmtid);
      array_shift($mmtids);
      mm_content_fix_breadcrumbs($mmtids);
      $set_breadcrumb = TRUE;
    }
    return $mmtid;
  }
  return FALSE;
}

function mm_form_token_load($token, $mmtid, $dummy, $is_new = '') {
  return drupal_valid_token($token, MM_LARGE_GROUP_TOKEN . $mmtid . $is_new) ? $token : FALSE;
}
