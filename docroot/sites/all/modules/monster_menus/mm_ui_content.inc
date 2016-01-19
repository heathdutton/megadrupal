<?php

/**
 * @file
 * User interface routines for editing/deleting MM content
 */

/**
 * Perform an operation on a tree entry
 *
 * @param $mmtid
 *   MM tree ID of entry to modify
 * @param $op
 *   Operation to perform
 * @return
 *   The HTML code for the results
 */
function mm_ui_content($mmtid, $op = '') {
  global $user;

  $perms = mm_content_user_can($mmtid);
  if (empty($op)) {
    if ($perms[MM_PERMS_IS_RECYCLE_BIN]) {
      $op = user_access('delete permanently') ? 'empty' : 'search';
    }
    elseif ($perms[MM_PERMS_WRITE]) {
      $op = 'edit';
    }
    else {
      drupal_set_message(t('You do not have permission to change the settings. Please choose another option from the menu.'));
      return ' ';
    }

    if ($op != 'edit') {
      menu_set_active_item("mm/$mmtid/settings/$op");
    }
  }

  $params = array(
    MM_GET_TREE_DEPTH        => $op == 'delete' || $op == 'empty' ? -1 : 0,
    MM_GET_TREE_RETURN_FLAGS => TRUE,
    MM_GET_TREE_RETURN_PERMS => TRUE,
    MM_GET_TREE_RETURN_MTIME => TRUE,
  );
  $tree = mm_content_get_tree($mmtid, $params);

  if (!count($tree)) {
    return MENU_NOT_FOUND;
  }

  if (!$tree[0]->perms[MM_PERMS_WRITE] && !$tree[0]->perms[MM_PERMS_SUB] && !$tree[0]->perms[MM_PERMS_APPLY]) {
    return MENU_ACCESS_DENIED;
  }

  $x = mm_ui_strings($is_group = $tree[0]->is_group);
  $x['%name'] = mm_content_get_name($tree[0]);

  switch ($op) {
    case 'edit':
      mm_content_add_breadcrumb(t('Settings for %name', $x));
      _mm_ui_is_user_home($tree[0]);
      return _mm_ui_content_edit_form($tree[0], $mmtid, $is_group);

    case 'copymove':
      mm_content_add_breadcrumb(t('Copy/move %name', $x));
      _mm_ui_is_user_home($tree[0]);
      module_load_include('inc', 'monster_menus', 'mm_ui_content_copymove');
      return drupal_get_form('mm_ui_content_copymove', $tree[0]);

    case 'restore':
      $next_par = FALSE;
      foreach (array_reverse(mm_content_get_parents($tree[0]->mmtid)) as $t) {
        if ($t < 0) { // virtual user dir
          continue;
        }
        elseif ($next_par) {
          $par = mm_content_get($t);
          $pperms = mm_content_user_can($t);
          break;
        }
        elseif (mm_content_user_can($t, MM_PERMS_IS_RECYCLE_BIN)) {
          $next_par = TRUE;
        }
      }

      $x['!name'] = $par ? l(mm_content_get_name($par), mm_content_get_mmtid_url($par->mmtid)) : '';
      if (!$pperms[MM_PERMS_SUB]) {
        return t('You cannot restore this @thing because you do not have permission to add to the parent @thing, !name. You may be able to copy or move this @thing to another location, however.', $x);
      }
      module_load_include('inc', 'monster_menus', 'mm_ui_content_restore');
      return drupal_get_form('mm_ui_content_restore_confirm', $tree[0], $par->mmtid, $x);

    case 'empty':
      if (!mm_content_user_can_recycle($mmtid, 'EMPTY')) {
        return MENU_ACCESS_DENIED;
      }
      // intentionally fall through to 'delete'

    case 'delete':
      module_load_include('inc', 'monster_menus', 'mm_ui_content_delete');
      return mm_ui_content_delete($tree, $x, $is_group);

    case 'sub':
      mm_content_add_breadcrumb($mmtid != 1 ? t('Create a new @subthing of %name', $x) : t('Create a new site', $x));
      $sub = clone $tree[0];
      $sub->name = $is_group ? t('(new group)') : ($mmtid != 1 ? t('(new page)') : t('(new site)'));
      $sub->alias = '';
      $sub->uid = $user->uid;
      $sub->theme = '';
      $sub->hidden = FALSE;
      $sub->flags = array();
      foreach (module_invoke_all('mm_tree_flags') as $flag => $elem) {
        if (isset($elem['#flag_inherit']) && $elem['#flag_inherit'] === TRUE && isset($tree[0]->flags[$flag])) {
          $sub->flags[$flag] = $tree[0]->flags[$flag];
        }
      }

      return _mm_ui_content_edit_form($sub, $mmtid, $is_group, TRUE);

    case 'search':
      mm_content_add_breadcrumb(t('Search')); // eventually Search/Replace
      module_load_include('inc', 'monster_menus', 'mm_search_replace');
      return mm_search_get_form($mmtid);

    case 'search result':
      mm_content_add_breadcrumb(t('Search Results')); // eventually Search/Replace
      module_load_include('inc', 'monster_menus', 'mm_search_replace');
      return mm_search_results();

    case 'search result csv':
      module_load_include('inc', 'monster_menus', 'mm_search_replace');
      return mm_search_results_csv();

    default:
      return MENU_NOT_FOUND;
  }
}

function mm_ui_content_revisions($mmtid) {
  function __mm_ui_content_date($data) {
    return format_date($data, 'short');
  }
  function __mm_ui_content_page($data) {
    return $data ? l(mm_content_get_name($data), "mm/$data") : '';
  }
  function __mm_ui_content_mode($data) {
    static $modes;
    if (!$modes) {
      $modes = array(MM_PERMS_WRITE => t('delete/edit'), MM_PERMS_APPLY => t('add content'), MM_PERMS_SUB => t('add sub-pages'), MM_PERMS_READ => t('read'));
    }
    $out = array();
    foreach (explode(',', $data) as $mode) {
      $out[] = __mm_ui_content_from_list($modes, $mode);
    }
    return empty($out) ? t('(none)') : implode(', ', $out);
  }
  function __mm_ui_content_yesno($data) {
    return empty($data) ? t('no') : t('yes');
  }
  function __mm_ui_content_nodeinfo($data) {
    static $modes;
    if (!$modes) {
      $dummy = array();
      $modes = _mm_ui_node_info_values($dummy);
    }
    return __mm_ui_content_from_list($modes, $data);
  }
  function __mm_ui_content_comment($data) {
    static $modes;
    if (!$modes) {
      $modes = _mm_ui_comment_write_setting_values();
    }
    return __mm_ui_content_from_list($modes, $data);
  }
  function __mm_ui_content_from_list($modes, $data) {
    if (is_null($data) || $data === '') {
      return '';
    }
    if (isset($modes[$data])) {
      return $modes[$data];
    }
    return t('Unknown value "@value"', array('@value' => $data));
  }
  $header = array(
    array('data' => t('Date'),        '_field' => 'mtime',       '_fmt' => '__mm_ui_content_date', 'field' => 'mtime', 'sort' => 'asc'),
    array('data' => t('Modified By'), '_field' => 'muid',        '_fmt' => 'mm_content_uid2name'),
    array('data' => t('Page Name'),   '_field' => 'name',        '_fmt' => 'check_plain'),
    array('data' => t('Page URL'),    '_field' => 'alias',       '_fmt' => 'check_plain'),
    array('data' => t('Parent'),      '_field' => 'parent',      '_fmt' => '__mm_ui_content_page'),
    array('data' => t('All Users'),   '_field' => 'default_mode','_fmt' => '__mm_ui_content_mode'),
    array('data' => t('Owner'),       '_field' => 'uid',         '_fmt' => 'mm_content_uid2name'),
    array('data' => t('Hidden'),      '_field' => 'hidden',      '_fmt' => '__mm_ui_content_yesno'),
    array('data' => t('Theme'),       '_field' => 'theme',       '_fmt' => 'check_plain'),
    array('data' => t('Attributions'),'_field' => 'node_info',   '_fmt' => '__mm_ui_content_nodeinfo'),
    array('data' => t('Summaries'),   '_field' => 'previews',    '_fmt' => '__mm_ui_content_yesno'),
    array('data' => t('Comments'),    '_field' => 'comment',     '_fmt' => '__mm_ui_content_comment'),
    array('data' => t('RSS'),         '_field' => 'rss',         '_fmt' => '__mm_ui_content_yesno'),
    array('data' => t('Hover'),       '_field' => 'hover',       '_fmt' => 'check_plain'),
  );
  $rows = array();

  $query = db_select('mm_tree_revisions', 'r')
    ->fields('r')
    ->condition('mmtid', $mmtid)
    ->extend('TableSort')
    ->orderByHeader($header)
    ->orderBy('vid', drupal_strtoupper(tablesort_get_sort($header)))
    ->execute();
  foreach ($query as $result) {
    $row = array();
    foreach ($header as $col) {
      $_field = $col['_field'];
      $func = $col['_fmt'];
      $data = $result->{$_field} = $func($result->{$_field});
      $class = 'mm-revisions-changed';
      if (!isset($col['field']) && !empty($last)) {
        $old = $last->{$_field};
        if ($old === $data) {
          $class = 'mm-revisions-same';
        }
        else if (is_null($data) || $data === '') {
          $class = 'mm-revisions-deleted';
          $data = $old;
        }
        else if (is_null($old) || $old === '') {
          $class = 'mm-revisions-added';
        }
      }
      $row[] = array('data' => $data, 'class' => array($class));
    }
    $last = $result;
    $rows[] = $row;
  }

  $css = drupal_get_path('module', 'monster_menus') . '/css/mm.css';
  $build['#attached'] = array('css' => array($css));
  $build['revisions'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  return $build;
}

/**
 * Display the form for editing or creating a tree entry
 */
function _mm_ui_content_edit_form($item, $mmtid, $is_group = FALSE, $is_new = FALSE) {
  module_load_include('inc', 'monster_menus', 'mm_ui_content_edit');
  return drupal_get_form('mm_ui_content_edit', $item, $mmtid, $is_group, $is_new);
}
