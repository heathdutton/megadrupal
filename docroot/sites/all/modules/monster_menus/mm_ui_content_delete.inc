<?php

/**
 * @file
 * User interface routines for deleting MM content
 */

function mm_ui_content_delete($tree, $x, $is_group) {
  if (isset($tree[0]->flags['limit_delete']) && !user_access('administer all menus')) {
    return MENU_ACCESS_DENIED;
  }

  if ($tree[0]->mmtid == mm_home_mmtid()) {
    return t('The @thing %name cannot be deleted.', $x);
  }

  $del_perm = !mm_content_recycle_enabled() ||
      $tree[0]->perms[MM_PERMS_IS_RECYCLED] || $tree[0]->perms[MM_PERMS_IS_GROUP];

  $mmtids = array();
  $kids = 0;
  $x['%sub'] = '';
  foreach ($tree as $t) {
    if (!$t->perms[MM_PERMS_IS_RECYCLE_BIN]) {
      if ($t != $tree[0]) $kids++;
      if ($t->level == 1 && $x['%sub'] == '') $x['%sub'] = mm_content_get_name($t);
    }

    if ($del_perm && !$t->perms[MM_PERMS_WRITE] && !$t->perms[MM_PERMS_IS_RECYCLE_BIN] || isset($t->flags['limit_delete']) && !user_access('administer all menus')) {
      $x['%name'] = mm_content_get_name($t);
      return t('You cannot delete this @thing because you do not have permission to delete the @subthing %name', $x);
    }

    $mmtids[] = $t->mmtid;
  }
  $x['@num'] = $kids;

  $nodes = $excl_nodes = array();
  if (!$is_group) {
    $nodes = mm_content_get_nids_by_mmtid($mmtids);
    $excl_nodes = mm_content_get_nids_by_mmtid($mmtids, 0, TRUE);
  }

  return drupal_get_form('mm_ui_content_delete_confirm', $tree, $x, $is_group, $mmtids, $del_perm, $kids, $nodes, $excl_nodes);
}

function mm_ui_content_delete_confirm($form, &$form_state, $tree, $x, $is_group, $mmtids, $del_perm, $kids, $nodes, $excl_nodes) {
  $form['mmtid'] = array('#type' => 'value', '#value' => $tree[0]->mmtid);
  $message = '';

  if ($tree[0]->perms[MM_PERMS_IS_RECYCLE_BIN]) {
    mm_content_add_breadcrumb(t('Are you sure you want to empty this recycle bin?'));

    $n = array('@pages' => $kids, '@nodes' => count($nodes));
    if ($kids && $n) {
      $message = t('There are currently @pages page(s) and @nodes piece(s) of content in this recycle bin. ', $n);
    }
    elseif ($kids) {
      $message = t('There are currently @pages page(s) in this recycle bin. ', $n);
    }
    elseif ($n) {
      $message = t('There are currently @nodes piece(s) of content in this recycle bin. ', $n);
    }
    $message .= t('Are you sure you want to empty it?');
    $action = t('Empty Bin');
    $cancel_url = 'mm/' . $tree[0]->mmtid;
  }
  else {  // !MM_PERMS_IS_RECYCLE_BIN
    if (!$kids) {
      $message = $del_perm ?
        t('The @thing %name will be permanently deleted.', $x) :
        t('The @thing %name will be moved to the recycle bin.', $x);
    }
    elseif ($kids == 1) {
      $message = $del_perm ?
        t('The @thing %name and its @subthing %sub will be permanently deleted.', $x) :
        t('The @thing %name and its @subthing %sub will be moved to the recycle bin.', $x);
    }
    else {
      $message = $del_perm ?
        t('The @thing %name and its @num @subthings will be permanently deleted.', $x) :
        t('The @thing %name and its @num @subthings will be moved to the recycle bin.', $x);
    }

    if ($is_group) {
      $num_mmtids = db_select('mm_tree_access', 'a')
        ->fields('a', array('mmtid'))
        ->distinct()
        ->condition('a.gid', $mmtids)
        ->countQuery()->execute()->fetchField();
      $num_nodes = db_select('mm_node_write', 'w')
        ->fields('w', array('nid'))
        ->distinct()
        ->condition('w.gid', $mmtids)
        ->countQuery()->execute()->fetchField();
      $x['@mmtids'] = $num_mmtids;
      $x['@nodes'] = $num_nodes;
      if ($num_mmtids && $num_nodes) $message .= ' ' . t('The permissions of @mmtids page(s) and @nodes piece(s) of content will be affected.', $x);
      elseif ($num_mmtids) $message .= ' ' . t('The permissions of @mmtids page(s) will be affected.', $x);
      elseif ($num_nodes) $message .= ' ' . t('The permissions of @nodes piece(s) of content will be affected.', $x);
    }
    elseif ($del_perm) {   // !$is_group
      if ($nodes) {
        if (!$excl_nodes) {
          $message .= '<p>' . t('Any page contents appear on other pages as well, and will therefore not be permanently deleted.') . '</p>';
        }
        elseif (!mm_content_recycle_enabled() && user_access('bypass node access')) {
          $x['@kidthings'] = $kids ? $x['@things'] : $x['@thing'];
          $x['@nodes'] = count($excl_nodes);
          $form['delnodes'] = array(
            '#type' => 'checkbox',
            '#title' => t('Also permanently delete the @nodes piece(s) of content appearing on the @kidthings, which are not assigned elsewhere', $x)
          );
        }
        else {
          $can_del = 0;
          foreach (node_load_multiple($excl_nodes) as $node) {
            if (node_access('delete', $node)) {
              $can_del++;
            }
          }
          $message .= '<p>';
          if ($can_del == count($excl_nodes)) {
            if (!mm_content_recycle_enabled()) {
              $message .= t('All contents on the page(s) not present on other pages will be permanently deleted.');
            }
            else {
              $message .= t('All contents on the page(s) will be permanently deleted.');
            }
          }
          elseif (!mm_content_recycle_enabled()) {
            $message .= t('Any contents you have permission to delete and are not present on other pages will be permanently deleted.');
          }
          else {
            $message .= t('Any contents you have permission to delete will be permanently deleted.');
          }
          $message .= '</p>';
        }
      }
    }

    $action = t('Delete');
    $cancel_url = 'mm/' . $tree[0]->mmtid . '/settings';
  }   // !MM_PERMS_IS_RECYCLE_BIN

  $form['del_perm'] = array('#type' => 'value', '#value' => $del_perm);

  if ($del_perm) {
    $short = t('Permanently delete %name?', $x);
    $message .= ' ' . t('This action cannot be undone.');
  }
  else {
    $short = t('Move %name to the recycle bin?', $x);
  }

  if (!empty($_GET['destination'])) $cancel_url = $_GET['destination'];
  return confirm_form($form, $short, $cancel_url, $message, $action);
}

/**
 * Validate tree entry deletion
 */
function mm_ui_content_delete_confirm_validate($form, &$form_state) {
  $mmtid = $form_state['values']['mmtid'];

  $tree = mm_content_get_tree($mmtid, array(MM_GET_TREE_RETURN_PERMS => TRUE, MM_GET_TREE_RETURN_FLAGS => TRUE));
  if (!$tree) {
    form_set_error('', t('Node not found.'));
  }

  if (!user_access('administer all menus')) {
    if (isset($tree[0]->flags['limit_delete'])) {
      drupal_access_denied();
      return;
    }

    $x = mm_ui_strings($tree[0]->is_group);
    foreach ($tree as $t) {
      if ($form_state['values']['del_perm'] && !$t->perms[MM_PERMS_WRITE] && !$t->perms[MM_PERMS_IS_RECYCLE_BIN] || isset($t->flags['limit_delete'])) {
        $x['%name'] = mm_content_get_name($t);
        form_set_error('', t('You cannot delete this @thing because you do not have permission to delete the @subthing %name', $x));
      }
    }
  }

  if ($mmtid == mm_home_mmtid()) {
    $x['%name'] = mm_content_get_name($tree[0]);
    form_set_error('', t('The %name @thing cannot be deleted.', $x));
  }
}

/**
 * Process tree entry deletion
 */
function mm_ui_content_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $parent = mm_content_get_parent($mmtid = $form_state['values']['mmtid']);

    if ($form_state['values']['del_perm']) {
      $pars = mm_content_get_parents($mmtid);
      $err = mm_content_delete($mmtid, mm_content_recycle_enabled() || !empty($form_state['values']['delnodes']), TRUE);
      if (!$err) {
        // Try to delete the bin, but only bother trying if it's this entry's immediate parent.
        if (count($pars) && mm_content_user_can($pars[count($pars) - 1], MM_PERMS_IS_RECYCLE_BIN)) {
          $err = mm_content_delete_bin($pars[count($pars) - 1]);
          $parent = count($pars) >= 2 ? $pars[count($pars) - 2] : mm_home_mmtid();
        }
      }
    }
    else {
      $err = mm_content_move_to_bin($mmtid);
    }

    if (is_string($err)) {
      drupal_set_message(t($err), 'error');
    }
    else {
      mm_redirect_to_mmtid($parent);
    }
  }
  $form_state['redirect'] = '';
}
