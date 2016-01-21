<?php

/**
 * @file
 * User interface routines for copying/moving MM content
 */

/**
 * Display the form for copying/moving a tree entry
 */
function mm_ui_content_copymove($form, &$form_state, $item) {
  $x = mm_ui_strings($is_group = $item->is_group);

  if ($item->parent > 0) {
    $pitem = mm_content_get_tree($item->parent, array(
        MM_GET_TREE_DEPTH        => 0,
        MM_GET_TREE_RETURN_PERMS => TRUE,
      )
    );
  }

  $form['#attributes'] = array('class' => array('mm-ui-copymove'));
  $form['mmtid'] = array(
    '#type' => 'value',
    '#value' => $item->mmtid
  );

  $form['mode'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mode'),
    '#collapsible' => FALSE);
  $form['mode']['mode'] = array(
    '#type' => 'radios',
    '#default_value' => 'copy',
    '#options' => array('copy' => t('Copy'), 'move' => t('Move')));

  $form['copy'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'copydiv'),
    '#title' => t('What to copy'),
    '#collapsible' => FALSE);
  $mustcheck = $is_group ? '' : "alert('" . t('You must choose "This @thing", "Contents", or both.', $x) . "');";
  $form['copy']['copy_page'] = array(
    '#type' => 'checkbox',
    '#default_value' => TRUE,
    '#title' => t('This @thing', $x));
  $form['copy']['copy_subpage'] = array(
    '#type' => 'checkbox',
    '#default_value' => FALSE,
    '#attributes' => array('style' => 'margin-left: 20px'),
    '#title' => t('and any @subthings', $x));
  if (!$is_group) {
    $form['copy']['copy_nodes'] = array(
      '#type' => 'checkbox',
      '#default_value' => FALSE,
      '#title' => t('Contents'));
    $form['copy']['desc'] = array(
      '#type' => 'item',
      '#description' => t('When contents are copied without their current @thing, the new copies take on the permissions of the destination @thing.', $x));
  }

  $form['move'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'movediv'),
    '#title' => t('What to move'),
    '#collapsible' => FALSE);
  $form['move']['move_mode'] = array(
    '#type' => 'radios',
    '#default_value' => 'page',
    '#description' => $is_group ? NULL : t('Moved contents keep their original permissions.'),
    '#options' => array(
      'page' => t($is_group ? 'This @thing and any @subthings' : 'This @thing, any @subthings, and all their contents', $x),
      'nodes' => t('Just the contents appearing on this @thing', $x)));

  $limit_move = isset($item->flags['limit_move']) && !user_access('administer all menus');
  if ($is_group) {
    unset($form['move']['move_mode']['#options']['nodes']);
    if ($limit_move) {
      unset($form['mode']['mode']['#options']['move']);
      $form['mode']['mode']['#description'] = t('You are not allowed to move this group.') . ' ' . $form['mode']['mode']['#description'];
    }
    elseif (mm_content_is_vgroup($item->mmtid)) {
      $form['mode']['mode']['#default_value'] = 'move';
      unset($form['mode']['mode']['#options']['copy']);
    }
  }
  elseif ($item->perms[MM_PERMS_IS_RECYCLED]) {
    unset($form['mode']['mode']['#options']['move']);
    $form['mode']['mode']['#description'] = t('This @thing is in the recycle bin. You can copy it, but it cannot be moved.', $x);
  }
  elseif ($limit_move) {
    $form['move']['move_mode']['#default_value'] = 'nodes';
    unset($form['move']['move_mode']['#options']['page']);
    $form['move']['move_mode']['#description'] = t('You are not allowed to move this page, only its contents.') . ' ' . $form['move']['move_mode']['#description'];
  }
  else {
    $prevent_mode = variable_get('mm_prevent_showpage_removal', MM_PREVENT_SHOWPAGE_REMOVAL_WARN);
    if ($prevent_mode != MM_PREVENT_SHOWPAGE_REMOVAL_NONE && ($showpage_mmtid = mm_content_test_showpage_mmtid($item->mmtid))) {
      if ($prevent_mode == MM_PREVENT_SHOWPAGE_REMOVAL_WARN || user_access('administer all menus')) {
        $form['move']['move_mode']['#description'] = $showpage_mmtid == $item->mmtid ?
          t('<strong>Warning:</strong> This page contains dynamic content which may no longer appear if it is moved to another location.') :
          t('<strong>Warning:</strong> The sub-page !page contains dynamic content which may no longer appear if this page is moved to another location.', array('!page' => l(mm_content_get_name($showpage_mmtid), "mm/$showpage_mmtid")));
      }
      else {
        $form['move']['move_mode']['#default_value'] = 'nodes';
        unset($form['move']['move_mode']['#options']['page']);
        $form['move']['move_mode']['#description'] = $showpage_mmtid == $item->mmtid ?
          t('This page contains dynamic content which will no longer appear if it is moved to another location. You may only move the contents.') :
          t('The sub-page !page contains dynamic content which will no longer appear if this page is moved to another location. You may only move the contents.', array('!page' => l(mm_content_get_name($showpage_mmtid), "mm/$showpage_mmtid")));
      }
    }
  }

  $form['dest'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination'),
    '#collapsible' => FALSE);

  $parents = mm_content_get_parents($item->mmtid);
  if (!user_access('administer all menus')) array_shift($parents);  // skip root
  $pop_start = implode('/', $parents) . "/$item->mmtid";

  $form['dest']['dest'] = array(
    '#title' => t('Destination'),
    '#type' => $is_group ? 'mm_grouplist' : 'mm_catlist',
    '#required' => $item->parent <= 0,
    '#default_value' => isset($pitem) ?
      array($pitem[0]->mmtid => mm_content_get_name($pitem[0])) :
      array('' => t('(choose the destination)')),
    '#mm_list_popup_start' => $pop_start,
    '#mm_list_min' => 1,
    '#mm_list_max' => 1,
    '#mm_list_selectable' => 'au',
    '#description' => t('Choose where to copy/move the @thing. The default value is the @thingpos current parent.', $x)
  );

  $form['name_alias'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('id' => 'namediv'),
    '#title' => $is_group ? t('Name') : t('Name and URL name'),
    '#collapsible' => FALSE);
  $form['name_alias']['name'] = array(
    '#type' => 'mm_fake_required',
    '#mm_orig_type' => 'textfield',
    '#title' => t('New @thing name', $x),
    '#default_value' => $item->name,
    '#size' => 40, '#maxlength' => 128,
    '#description' => t('If you do not change the Destination above, you must modify the page name and URL name here.')
  );

  if (!$is_group) {
    $form['name_alias']['alias'] = array(
      '#type' => !empty($item->is_user_home) || user_access('administer all menus') ? 'machine_name' : 'mm_fake_required',
      '#mm_orig_type' => 'machine_name',
      '#machine_name' => array(
        'exists' => 'mm_ui_machine_name_exists',
        'source' => array('name_alias', 'name'),
        'label' => t('New URL name'),
        'replace_pattern' => '[^-.\w]+',
        'replace' => '-',
        'error' => t('The URL name must contain only letters, numerals, hyphens, periods and underscores.'),
        'standalone' => TRUE,
      ),
      '#title' => t('New URL name'),
      '#default_value' => $item->alias,
      '#size' => 20, '#maxlength' => 128,
      '#description' => t('The name that will be used in the Web address of the page. ' .
        'Make this a shortened version of the Page name, using only lowercase letters, ' .
        'numerals, hyphens, periods and underscores.')
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go!'),
  );
  mm_static('copymove', TRUE, $mustcheck);

  return $form;
}

/**
 * Validate the form for copying/moving a tree entry
 */
function mm_ui_content_copymove_validate($form, &$form_state, $restore = FALSE) {
  _mm_ui_content_copymove_get_values($form_state['values'], $src_mmtid, $modes, $src_parent_mmtid, $dest_mmtid);
  $nodes_only = $modes['move_nodes'] || $modes['copy_nodes'] && !$modes['copy_page'];

  if (!$restore && ($modes['copy_page'] || $modes['move_page'])) {
    mm_ui_validate_fake_required($form['name_alias']['name'], trim($form_state['values']['name']));
    if (isset($form['name_alias']['alias'])) {
      mm_ui_validate_fake_required($form['name_alias']['alias'], trim($form_state['values']['alias']));
    }
  }

  if (!$src_mmtid || !is_numeric($src_mmtid)) {
    form_set_error('', t('Missing one or more required fields'));
    return;
  }

  $x = mm_ui_strings(mm_content_is_group($src_mmtid));

  if ($modes['move_page'] && !mm_content_user_can($src_mmtid, MM_PERMS_WRITE))
    $message = t('You do not have permission to modify this @thing.', $x);

  if ($modes['move_page'] && !$restore && !mm_content_user_can($src_parent_mmtid, MM_PERMS_SUB))
    $message = t('You do not have permission to modify the source @thingpos parent.', $x);

  if (!is_numeric($dest_mmtid) || !mm_content_user_can($dest_mmtid, $nodes_only ? MM_PERMS_APPLY : MM_PERMS_SUB))
    $message = t('You do not have permission to modify the destination @thing.', $x);

  if (mm_content_is_normal($src_mmtid) != ($dest_mmtid == 1 || mm_content_is_normal($dest_mmtid)) || mm_content_is_group($src_mmtid) != mm_content_is_group($dest_mmtid))
      $message = t('Source and destination are not of the same type.');

  if (mm_content_user_can($dest_mmtid, MM_PERMS_IS_RECYCLED))
    if ($modes['copy_nodes'] || $modes['copy_page']) {
      $message = 'You cannot copy to a recycle bin.';
    }
    else {
      $message = 'You cannot move to a recycle bin using this option. Please use the Delete option, instead.';
    }

  if (!empty($message)) {
    watchdog('mm', $message, array(), WATCHDOG_WARNING, "source: $src_mmtid, dest: $dest_mmtid");
    form_set_error('', t($message));
    return;
  }

  if (mm_content_is_vgroup($src_mmtid) != mm_content_is_vgroup($dest_mmtid)) {
    form_set_error('dest', mm_content_is_vgroup($src_mmtid) ? t('Pre-defined groups can only be moved to create sub-groups of other pre-defined groups.') : t('You cannot copy/move a regular group inside the pre-defined groups list.'));
  }

  if (!$nodes_only && !_mm_ui_validate_entry($src_mmtid, $dest_mmtid, $form_state['values'], TRUE)) {
    return;
  }

  if ($dest_mmtid == $src_parent_mmtid && $modes['move_page']) {
    form_set_error('', t('Instead of moving a @thing within the same parent @thing, rename it using Settings-&gt;Edit.', $x));
  }
  elseif ($dest_mmtid == $src_mmtid && $nodes_only && $modes['move_nodes']) {
    form_set_error('', t('Moving just a @thingpos contents to itself results in no change.', $x));
  }
  elseif ($dest_mmtid == $src_mmtid && !$modes['move_nodes'] && ($modes['move_page'] || $modes['copy_recur'])) {
    form_set_error('', t('You cannot copy or move a @thing within itself.', $x));
  }
  elseif (mm_content_is_child($dest_mmtid, $src_mmtid)) {
    if ($modes['move_page']) {
      form_set_error('dest', t('You cannot move a @thing into a @subthing of itself. Please choose another destination.', $x));
    }
    elseif ($modes['copy_recur']) {
      form_set_error('dest', t('You cannot copy a @thing and any @subthings into a child of itself. Please choose another destination.', $x));
    }
  }
  elseif ($modes['move_nodes'] && mm_content_is_archive($dest_mmtid)) {
    form_set_error('dest', t('You cannot move content into an archive. Move the content into the main @thing, instead, and the archive will be updated automatically.', $x));
  }
}

/**
 * Process the form for copying/moving a tree entry
 */
function mm_ui_content_copymove_submit($form, &$form_state) {
  global $user;

  _mm_ui_content_copymove_get_values($form_state['values'], $src_mmtid, $modes, $src_parent_mmtid, $dest_mmtid);

  $name = trim($form_state['values']['name']);
  $alias = isset($form_state['values']['alias']) ? trim($form_state['values']['alias']) : NULL;
  $x = mm_ui_strings(mm_content_is_group($src_mmtid));

  if ($modes['copy_page'] || $modes['copy_nodes']) {
    if ($modes['copy_page'] && $modes['copy_nodes']) {
      $msg = $modes['copy_recur'] ?
        t('The @thing, any @subthings, and their contents were successfully copied.', $x) :
        t('The @thing and its contents were successfully copied.', $x);
    }
    elseif ($modes['copy_nodes']) {
      $msg = t('The @thingpos contents were successfully copied.', $x);
    }
    else {
      $msg = $modes['copy_recur'] ?
        t('The @thing and any @subthings were successfully copied.', $x) :
        t('The @thing was successfully copied.', $x);
    }

    $copy_params = array(
      MM_COPY_ALIAS =>    $alias,
      MM_COPY_CONTENTS => $modes['copy_nodes'],
      MM_COPY_NAME =>     $name,
      MM_COPY_OWNER =>    $user->uid,
      MM_COPY_READABLE => TRUE,
      MM_COPY_RECUR =>    $modes['copy_recur'],
      MM_COPY_TREE =>     $modes['copy_page'],
    );
    $new_mmtid = mm_content_copy($src_mmtid, $dest_mmtid, $copy_params);

    if (is_numeric($new_mmtid)) {
      if ($new_mmtid != $src_mmtid) $msg .= ' ' . t('You are now viewing the destination.');
      drupal_set_message($msg);
      $form_state['redirect'] = mm_content_get_mmtid_url($new_mmtid);
    }
    else {
      form_set_error('', t($new_mmtid));
    }
  }
  elseif ($modes['move_page']) {
    $old_tree = mm_content_get($src_mmtid);
    $error = mm_content_move($src_mmtid, $dest_mmtid, '');

    if (!$error) {
      if ($name != $old_tree->name || $alias != $old_tree->alias) {
        mm_content_update_quick(array('name' => $name, 'alias' => $alias), array('mmtid' => $src_mmtid), $dest_mmtid);
      }
      drupal_set_message(t('The @thing was successfully moved.', $x));
      $form_state['redirect'] = mm_content_get_mmtid_url($src_mmtid);
    }
    else {
      form_set_error('', t($error));
    }
  }
  elseif ($modes['move_nodes']) {
    $total = 0;
    $ok = array();
    foreach (mm_content_get_nids_by_mmtid($src_mmtid) as $nid) {
      $total++;
      $uid = db_select('node', 'n')
        ->fields('n', array('uid'))
        ->condition('n.nid', $nid)
        ->execute()->fetchField();
      if (!empty($uid) || $uid === 0) {
        $node = (object)array('nid' => $nid, 'uid' => $uid);
        if (mm_content_node_access($node, 'update')) {
          db_update('mm_node2tree')
            ->fields(array('mmtid' => $dest_mmtid))
            ->condition('mmtid', $src_mmtid)
            ->condition('nid', $nid)
            ->execute();
          mm_content_reset_custom_node_order($dest_mmtid, $nid);
          $ok[] = $nid;
        }
      }
      mm_content_notify_change('move_node', NULL, $ok, array('old_mmtid' => $src_mmtid, 'new_mmtid' => $dest_mmtid));
    }

    $failed = $total - count($ok);
    $x += array('@total' => $total, '@ok' => count($ok), '@failed' => $failed);
    if ($failed) {
      if ($failed == $total) {
        drupal_set_message(t('You do not have permission to move any of the @total piece(s) of content on the @thing.', $x));
        $form_state['redirect'] = mm_content_get_mmtid_url($src_mmtid);
      }
      else {
        drupal_set_message(t('Only @ok of the @total piece(s) of content on the @thing could be moved, due to lack of permission.', $x));
        $form_state['redirect'] = mm_content_get_mmtid_url($dest_mmtid);
      }
    }
    elseif (!$total) {
      drupal_set_message(t('There are no contents on this @thing to move.', $x));
      $form_state['redirect'] = mm_content_get_mmtid_url($src_mmtid);
    }
    else {
      drupal_set_message(t('@total piece(s) of content were successfully moved.', $x));
      $form_state['redirect'] = mm_content_get_mmtid_url($dest_mmtid);
    }
  }
}

function _mm_ui_content_copymove_get_values($form_values, &$src_mmtid, &$modes, &$src_parent_mmtid, &$dest_mmtid) {
  $src_mmtid =      $form_values['mmtid'];
  $copy_page =      $form_values['mode'] == 'copy' && $form_values['copy_page'];
  $modes = array(
    'copy_page' =>  $copy_page,
    'copy_recur' => $copy_page && $form_values['copy_subpage'],
    'copy_nodes' => $form_values['mode'] == 'copy' && $form_values['copy_nodes'],
    'move_page' =>  $form_values['mode'] == 'move' && $form_values['move_mode'] == 'page',
    'move_nodes' => $form_values['mode'] == 'move' && $form_values['move_mode'] == 'nodes'
  );
  $src_parent_mmtid = mm_content_get_parent($src_mmtid);
  $dest_mmtid = mm_ui_mmlist_key0($form_values['dest']);
}
