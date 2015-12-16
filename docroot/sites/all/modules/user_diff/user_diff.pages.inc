<?php

/**
 * @file
 * Menu callbacks for hook_menu().
 */

/**
 * Menu callback - show latest diff for a given node.
 */
function user_diff_latest($user) {
  $revisions = user_revision_list($user);
  $new = array_shift($revisions);
  $old = array_shift($revisions);
  drupal_goto("user/{$user->uid}/revisions/view/{$old->vid}/{$new->vid}");
}

/**
 * Generate an overview table of older revisions of a node and provide
 * an input form to select two revisions for a comparison.
 */
function user_diff_diffs_overview($user) {
  drupal_set_title(t('Revisions for @title', array('@title' => $user->name)));
  return drupal_get_form('user_diff_user_revisions', $user);
}

/**
 * Input form to select two revisions.
 *
 * @param $viewed_user
 *   User whose revisions are displayed for selection.
 */
function user_diff_user_revisions($form, $form_state, $viewed_user) {
  global $user;

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $viewed_user->uid,
  );

  $revision_list = user_revision_list($viewed_user);

  if (count($revision_list) > REVISION_LIST_SIZE) {
    // If the list of revisions is longer than the number shown on one page split the array.
    $page = isset($_GET['page']) ? $_GET['page'] : '0';
    $revision_chunks = array_chunk(user_revision_list($viewed_user), REVISION_LIST_SIZE);
    $revisions = $revision_chunks[$page];
    // Set up global pager variables as would 'pager_query' do.
    // These variables are then used in the theme('pager') call later.
    global $pager_page_array, $pager_total, $pager_total_items;
    $pager_total_items[0] = count($revision_list);
    $pager_total[0] = ceil(count($revision_list) / REVISION_LIST_SIZE);
    $pager_page_array[0] = max(0, min($page, ((int)$pager_total[0]) - 1));
  }
  else {
    $revisions = $revision_list;
  }
  $revert_permission = FALSE;
  $delete_permission = FALSE;
  if($user->uid == $viewed_user->uid){
    if (user_access('revert own user revisions') || user_access('revert user revisions')) {
      $revert_permission = TRUE;
    }
    if (user_access('delete own user revisions') || user_access('delete user revisions')) {
      $delete_permission = TRUE;
    }
  }
  else {
    if (user_access('revert user revisions')) {
      $revert_permission = TRUE;
    }
    if (user_access('delete user revisions')) {
      $delete_permission = TRUE;
    }
  }
  $revision_ids = array();
  foreach ($revisions as $revision) {
    $operations = array();
    $revision_ids[$revision->vid] = '';
    if ($revision->current_vid > 0) {
      $form['info'][$revision->vid] = array(
        '#markup' => t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'small'), "user/$viewed_user->uid"),
          '!username' => theme('username', array('account' => $revision))))
        . (($revision->log != '') ? '<p class="revision-log">'. filter_xss($revision->log) .'</p>' : ''),
      );
    }
    else {
      $form['info'][$revision->vid] = array(
        '#markup' => t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'small'), "user/$viewed_user->uid/revisions/$revision->vid/view"),
          '!username' => theme('username', array('account' => $revision))))
        . (($revision->log != '') ? '<p class="revision-log">'. filter_xss($revision->log) .'</p>' : '')
      );
      if ($revert_permission) {
        $operations[] = array('#markup' => l(t('revert'), "user/$viewed_user->uid/revisions/$revision->vid/revert"));
      }
      if ($delete_permission) {
        $operations[] = array('#markup' => l(t('delete'), "user/$viewed_user->uid/revisions/$revision->vid/delete"));
      }
      // Set a dummy, even if the user has no permission for the other
      // operations, so that we can check if the operations array is
      // empty to know if the row denotes the current revision.
      $operations[] = array();
    }
    $form['operations'][$revision->vid] = $operations;

  }
  $new_vid = key($revision_ids);
  next($revision_ids);
  $old_vid = key($revision_ids);
  $form['diff']['old'] = array(
    '#type' => 'radios',
    '#options' => $revision_ids,
    '#default_value' => $old_vid
  );
  $form['diff']['new'] = array(
    '#type' => 'radios',
    '#options' => $revision_ids,
    '#default_value' => $new_vid
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Show diff'));

  if (count($revision_list) > REVISION_LIST_SIZE) {
    $form['#suffix'] = theme('pager', NULL, REVISION_LIST_SIZE, 0);
  }

  return $form;
}

/**
 * Submit code for input form to select two revisions.
 */
function user_diff_user_revisions_submit($form, &$form_state) {
  // the ids are ordered so the old revision is always on the left
  $old_vid = min($form_state['values']['old'], $form_state['values']['new']);
  $new_vid = max($form_state['values']['old'], $form_state['values']['new']);
  $form_state['redirect'] =  'user/'. $form_state['values']['uid'] .'/revisions/view/'. $old_vid .'/'. $new_vid;
}

/**
 * Validation for input form to select two revisions.
 */
function user_diff_user_revisions_validate($form, &$form_state) {
  $old_vid = $form_state['values']['old'];
  $new_vid = $form_state['values']['new'];
  if ($old_vid==$new_vid || !$old_vid || !$new_vid) {
    form_set_error('diff', t('Select different revisions to compare.'));
  }
}

/**
 * Create output string for a comparison of 'user' between
 * versions 'old_vid' and 'new_vid'.
 *
 * @param $user
 *   User on which to perform comparison
 * @param $old_vid
 *   Version ID of the old revision.
 * @param $new_vid
 *   Version ID of the new revision.
 */
function user_diff_diffs_show($user, $old_vid, $new_vid) {
  module_load_include('inc', 'diff', 'diff.pages');
  // Set same title as on the 'Revisions' tab for consistency
  drupal_set_title(t('Revisions for @name', array('@name' => $user->name)));

  $user_revisions = user_revision_list($user);
  $old_user = user_revision_load($user->uid, $old_vid);
  $new_user = user_revision_load($user->uid, $new_vid);
  // Generate table header (date, username, logmessage).
  $old_header = t('!date by !username', array(
    '!date' => l(format_date($old_user->revision_timestamp), "user/$user->uid/revisions/$old_user->vid/view"),
    '!username' => theme('username', array('account' => $user_revisions[$old_vid])),
  ));
  $new_header = t('!date by !username', array(
    '!date' => l(format_date($new_user->revision_timestamp), "user/$user->uid/revisions/$new_user->vid/view"),
    '!username' => theme('username', array('account' => $user_revisions[$new_vid])),
  ));

  $old_log = $old_user->log != '' ? '<p class="revision-log">'. filter_xss($old_user->log) .'</p>' : '';
  $new_log = $new_user->log != '' ? '<p class="revision-log">'. filter_xss($new_user->log) .'</p>' : '';

  // Generate previous diff/next diff links.
  $next_vid = _diff_get_next_vid($user_revisions, $new_vid);
  if ($next_vid) {
    $next_link = l(t('next diff >'), 'user/'. $user->uid .'/revisions/view/'. $new_vid .'/'. $next_vid);
  }
  else {
    $next_link = '';
  }
  $prev_vid = _diff_get_previous_vid($user_revisions, $old_vid);
  if ($prev_vid) {
    $prev_link = l(t('< previous diff'), 'user/'. $user->uid .'/revisions/view/'. $prev_vid .'/'. $old_vid);
  }
  else {
    $prev_link = '';
  }

  $cols = _diff_default_cols();
  $header = _diff_default_header($old_header, $new_header);
  $rows = array();
  if ($old_log || $new_log) {
    $rows[] = array(
      array(
        'data' => $old_log,
        'colspan' => 2
      ),
      array(
        'data' => $new_log,
        'colspan' => 2
      )
    );
  }
  $rows[] = array(
    array(
      'data' => $prev_link,
      'class' => array('diff-prevlink'),
      'colspan' => 2
    ),
    array(
      'data' => $next_link,
      'class' => array('diff-nextlink'),
      'colspan' => 2
    )
  );
  $rows = array_merge($rows, _user_diff_body_rows($old_user, $new_user));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('diff')), 'cols' => $cols));

  if ($user->vid == $new_vid) {
    $output .= '<div class="diff-section-title">'. t('Current revision:') .'</div>';
  }
  else {
    $output .= '<div class="diff-section-title">'. t('Revision of !new_date:', array('!new_date' => format_date($new_user->revision_timestamp))) .'</div>';
  }
  // Don't include node links (final argument) when viewing the diff.
  $user_view = user_view($new_user, 'full');
  $output .= drupal_render($user_view);
  return $output;
}

/**
 * Creates an array of rows which represent a diff between $old_user and $new_user.
 * The rows can be used via theme('diff_table') to be displayed.
 *
 * @param $old_user
 *   User for comparison which will be displayed on the left side.
 * @param $new_user
 *   User for comparison which will be displayed on the right side.
 */
function _user_diff_body_rows($old_user, $new_user) {
  drupal_add_css(drupal_get_path('module', 'diff') .'/diff.css', array('type' => 'module', 'media' => 'all', 'preprocess' => FALSE));
  module_load_include('inc', 'user_diff', 'includes/user');

  $rows = array();
  $any_visible_change = FALSE;
  // @todo quick workaround for PHP >= 5.3.0 date_diff() conflict.
  $user_diffs = _user_diff_module_invoke_all($old_user, $new_user);

  // We start off assuming all form elements are in the correct order.
  $user_diffs['#sorted'] = TRUE;

  // Recurse through all child elements.
  $count = 0;
  foreach (element_children($user_diffs) as $key) {
    // Assign a decimal placeholder weight to preserve original array order.
    if (!isset($user_diffs[$key]['#weight'])) {
      $user_diffs[$key]['#weight'] = $count/1000;
    }
    else {
      // If one of the child elements has a weight then we will need to sort
      // later.
      unset($user_diffs['#sorted']);
    }
    $count++;
  }

  // One of the children has a #weight.
  if (!isset($user_diffs['#sorted'])) {
    uasort($user_diffs, "element_sort");
  }

  // Render diffs for each.
  foreach ($user_diffs as $user_diff) {
    $show_header = isset($user_diff['#format']['show_header']) ? $user_diff['#format']['show_header'] : FALSE;
    $old_user = $user_diff['#old'];
    $new_user = $user_diff['#new'];
    $user_diff_rows = diff_get_rows($old_user, $new_user);
    if (count($user_diff_rows)) {
      $rows[] = array(array(
        'data' => t('Changes to %name', array('%name' => $user_diff['#name'])),
        'class' => 'diff-section-title',
        'colspan' => 4
      ));
      $rows = array_merge($rows, $user_diff_rows);
      $any_visible_change = TRUE;
    }
  }

  if (!$any_visible_change) {
    $rows[] = array(array(
      'data' => t('No visible changes'),
      'class' => 'diff-section-title',
      'colspan' => 4
    ));
    // Needed to keep safari happy
    $rows[] = array(
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
    );
  }
  return $rows;
}

/**
 * Helper function to invoke hook_diff in all enabled modules that implement it.
 *
 * Don't use module_invoke_all() since if date.module is enabled will clash with
 * PHP 5.3's date_diff() function.
 *
 * @todo figure out any else possible solution but not workaround.
 * @link http://drupal.org/node/639320
 * @see module_invoke_all()
 */
function _user_diff_module_invoke_all($old_user, $new_user) {
  $return = array();
  foreach (module_implements('user_diff') as $module) {
    $result = module_invoke($module, 'user_diff', $old_user, $new_user);
    if (isset($result) && is_array($result)) {
      $return = array_merge_recursive($return, $result);
    }
    elseif (isset($result)) {
      $return[] = $result;
    }
  }
  return $return;
}

/**
 * Show the inline diff for a given user, vid. If vid = 0 or no previous vid
 * exists for the given revision returns the normally rendered content of the
 * specified revision. If metadata is TRUE a header will be added with a legend
 * explaining the color code. This function returns HTML code
 */
function user_diff_inline_show($user, $vid = 0, $metadata = TRUE) {
  $new_user = $vid ? user_revision_user_load($user->uid, $vid, TRUE) : clone $user;
  user_build_content($new_user);
  $new = drupal_render($new_user->content);
  module_load_include('inc', 'diff', 'diff.pages');
  $old = $vid ? _diff_get_previous_vid(user_revision_list($user), $vid) : 0;
  if ($old) {
    $old_user = user_revision_user_load($user->uid, $old, TRUE);
    user_build_content($old_user);
    $old = drupal_render($old_user->content);
    $output = $metadata ? theme('user_diff_inline_metadata', array('user' => $new_user)) : '';
    $output .= diff_get_inline($old, $new);
    return $output;
  }
  return $new;
}
