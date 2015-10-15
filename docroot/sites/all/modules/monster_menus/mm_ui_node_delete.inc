<?php

/**
 * @file
 * User interface routines for deleting nodes
 */

/**
 * Confirm that the user wants to delete a node
 *
 * @param $mmtid
 *   MM tree ID containing the node. Can be NULL, if there is none.
 * @param $node
 *   Node to delete
 * @return
 *   The HTML code for the results
 */
function mm_ui_node_delete_confirm($form, $form_state, $mmtid, $node) {
  $title = mm_ui_fix_node_title($node->title);
  if (!mm_content_user_can_node($node, MM_PERMS_APPLY)) {
    return drupal_access_denied();
  }

  $permanent = mm_content_node_is_recycled($node, $mmtid) || !mm_content_recycle_enabled();
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['permanent'] = array(
    '#type' => 'value',
    '#value' => $permanent,
  );
  if (isset($mmtid)) {
    $form['mmtid'] = array(
      '#type' => 'value',
      '#value' => $mmtid,
    );
    $this_name = l(mm_content_get_name(mm_content_get($mmtid)), mm_content_get_mmtid_url($mmtid));
  }

  $names = !empty($this_name) ? array($mmtid => $this_name) : array();
  $other_bins = 0;
  if ($current_pages = mm_content_get_by_nid($node->nid)) {
    foreach (mm_content_get($current_pages) as $tree) {
      if ($tree->mmtid != $mmtid) {
        if (mm_content_user_can($tree->mmtid, MM_PERMS_IS_RECYCLED)) {
          $other_bins++;
        }
        else {
          $names[$tree->mmtid] = l(mm_content_get_name($tree), mm_content_get_mmtid_url($tree->mmtid));
        }
      }
    }
  }

  $button = t('Delete');
  if ($permanent) {
    $other_pages = count($names) - 1;
    if ($other_bins || $other_pages) {
      $button = t('Remove');
      $msg_short = t('Permanently remove %title?', array('%title' => $title));
      $msg = t('Are you sure you want to permanently remove %title from this page?', array('%title' => $title));
      if ($other_bins && $other_pages) {
        $msg .= ' ' . t('(It will continue to appear on @pages other page(s) and to exist in @bins recycle bin(s).)', array('@pages' => $other_pages, '@bins' => $other_bins));
      }
      else if ($other_bins) {
        $msg .= ' ' . t('(It will continue to exist in @bins recycle bin(s).)', array('@bins' => $other_bins));
      }
      else {
        $msg .= ' ' . t('(It will continue to appear on @pages other page(s).)', array('@pages' => $other_pages));
      }
    }
    else {
      $msg_short = t('Permanently delete %title?', array('%title' => $title));
      $msg = t('Are you sure you want to permanently delete %title?', array('%title' => $title));
    }
  }
  else {
    $msg_short = t('Move %title to the recycle bin?', array('%title' => $title));
    if (!$names || empty($this_name)) {
      $msg = t('Are you sure you want to move %title to the recycle bin?', array('%title' => $title));
    }
    else if (count($names) == 1) {
      $msg = t('Are you sure you want to move %title from !page to the recycle bin?', array('%title' => $title, '!page' => $names[$mmtid]));
    }
    else if ($names) {
      $msg = t('%title appears on multiple pages: !list', array('%title' => $title, '!list' => implode(', ', $names)));
      $form['mode'] = array(
        '#type' => 'radios',
        '#options' => array(
          'one' => t('Move from <strong>just this page</strong> to the recycle bin'),
          'all' => t('Move from <strong>all pages</strong> to the recycle bin'),
        ),
        '#default_value' => 'one',
        '#weight' => 0.5,
      );
    }
  }

  if ($mmtid) {
    mm_content_add_breadcrumb($msg);
  }

  $cancel = $mmtid ? mm_content_get_mmtid_url($mmtid) : 'node/' . $node->nid;
  if (!empty($_GET['destination'])) {
    $parsed = parse_url(urldecode($_GET['destination']));
    if (!empty($parsed['path'])) {
      $cancel = array(
        'path' => $parsed['path'],
        'query' => empty($parsed['query']) ? '' : $parsed['query'],
        'fragment' => empty($parsed['fragment']) ? '' : $parsed['fragment'],
      );
    }
  }

  return confirm_form($form, $msg_short, $cancel, $msg, $button, t('Cancel'));
}

/**
 * Execute node deletion
 */
function mm_ui_node_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm'] && ($node = node_load($form_state['values']['nid'])) && $node->nid) {
    $redirect = $page_mmtid = !empty($form_state['values']['mmtid']) ? $form_state['values']['mmtid'] : FALSE;

    if ($form_state['values']['permanent']) {
      if ($page_mmtid) {
        if (mm_content_is_recycle_bin($page_mmtid)) {
          $redirect = mm_content_get_parent($page_mmtid);
        }
        if (array_diff(mm_content_get_by_nid($node->nid), array($page_mmtid))) {
          // If the node is on other pages, just remove the reference.
          mm_content_remove_node_from_page($node->nid, $page_mmtid);
        }
        else {
          node_delete($node->nid);
        }
      }
      else {
        node_delete($node->nid);
      }
    }
    else {
      $nids = $page_mmtid && !empty($form_state['values']['mode']) && $form_state['values']['mode'] == 'one' ? array($node->nid => array($page_mmtid)) : $node->nid;
      $err = mm_content_move_to_bin(NULL, $nids);
    }

    if (!empty($err) && is_string($err)) {
      drupal_set_message(t($err), 'error');
      return;
    }

    if ($redirect) {
      mm_redirect_to_mmtid($redirect);
    }
  }
  $form_state['redirect'] = '';
}
