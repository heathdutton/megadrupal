<?php

/**
 * @file
 * Custom node types for Monster Menus
 */

function mm_node_types() {
  $t = get_t();
  return array(
    'redirect' => array(
      'name' => $t('Redirector'),
      'base' => 'mm_node_redir',
      'perms' => array(),
      'menus' => array(),
      'themes' => array(),
      'no_body' => TRUE,
      'description' => $t('Sends the user directly to another page you specify, either within the CMS or anywhere else on the Web')
    ),
    'subpglist' => array(
      'name' => $t('Subpage list'),
      'base' => 'mm_node_subpglist',
      'perms' => array(),
      'menus' => array(),
      'themes' => array(),
      'body_title' => $t('Preamble'),
      'description' => $t('Allows you to create a list of subpages, usually in a multi-column table')
    ),
    'alert' => array(
      'name' => $t('System alert'),
      'base' => 'mm_node_alert',
      'perms' => array(),
      'menus' => array(),
      'themes' => array(),
      'description' => $t('Alerts are displayed in a popup window based on an appearance schedule')
    ),
    'mm_block' => array(
      'name' => $t('Block contents'),
      'base' => 'mm_node_mm_block',
      'has_title' => 0,
      'perms' => array(),
      'menus' => array(),
      'themes' => array(),
      'description' => $t('Displays the contents of a block as a node, which allows the block to appear on a single page, rather than on every page using a particular theme')
    ),
  );
}

/**
 * Generate a page displaying a single node, along with its comments. This
 * modified version of node_show() is used by MM to allow comments to only be
 * visible to certain roles, when specified by the node's editor. In order for
 * this to work, you must enable the "access comments" permission for all roles.
 *
 * @param $node
 *   The node object ot display
 * @param $cid
 *   The ID of any comment to render
 * @param $message
 *   If TRUE, set the page's title to the revision status
 * @return
 *   The HTML of the resulting page
 */
function mm_node_show($node, $message = FALSE) {
  $view_mode = 'full';
  drupal_alter('mm_node_show', $node, $view_mode);

  if ($message) {
    drupal_set_title(t('Revision of %title from %date', array('%title' => $node->title, '%date' => format_date($node->revision_timestamp))), PASS_THROUGH);
  }

// TODO: FIX ME
//  if (function_exists('comment_render') && $node->comment && _mm_content_comments_readable($node)) {
//    $output .= comment_render($node, $cid);
//  }

  // Update the history table, stating that this user viewed this node.
  node_tag_new($node);

  // For markup consistency with other pages, use node_view_multiple() rather than node_view().
  return node_view_multiple(array($node->nid => $node), $view_mode);
}

/**
 * Menu callback; view a single node (taken from node_page_view()).
 */
function mm_node_page_view($node, $cid = NULL) {
  drupal_set_title(mm_ui_hide_node_title($node->title));
  return mm_node_show($node);
}

function mm_node_all_nodes_hook() {
  $return = array();
  $args = func_get_args();
  $op = array_shift($args);
  $node = $args[0];
  if (is_object($node) && isset($node->type)) {
    $list = monster_menus_node_info();
    if (isset($list[$node->type])) {
      $mm_nodefunc = $list[$node->type]['base'] . '_all_nodes_' . $op;
      if (function_exists($mm_nodefunc)) {
        $result = call_user_func_array($mm_nodefunc, $args);
        if (isset($result) && is_array($result)) {
          $return = array_merge_recursive($return, $result);
        }
        elseif (isset($result)) {
          $return[] = $result;
        }
      }
    }
  }
  return $return;
}

/******************** Redirector functions start here *********************/

/**
 * Implements hook_form().
 */
function mm_node_redir_form(&$node) {
  drupal_set_title(!empty($node->nid) ? t('Edit a redirector') : t('Create a redirector'));
  $form = array();

  $form['title'] = array(
    '#type' => 'hidden',
    '#value' => t('Redirector'));

  $form['where'] = array(
    '#type' => 'fieldset',
    '#title' => t('Location to redirect to'),
    '#collapsible' => FALSE);
  $form['where']['redir_url'] = array(
    '#type' => 'textfield',
    '#maxlength' => 500,
    '#title' => t('Destination URL'),
    '#default_value' => isset($node->redir_url) ? $node->redir_url : '',
    '#attributes' => array('class' => array('node-redir-url')),
  );

  $mmlist = array();
  if (isset($node->nid)) {
    if ($node->redir_mmtid && ($tree = mm_content_get($node->redir_mmtid)) && mm_content_user_can($node->redir_mmtid, MM_PERMS_READ)) {
      $mmlist = array($node->redir_mmtid => mm_content_get_name($tree));
      $pop_mmtid = $node->redir_mmtid;
    }
    else{
      $select = db_select('mm_node2tree', 'n');
      $select->join('mm_tree', 't', 'n.mmtid = t.mmtid');
      $select->fields('t', array('mmtid', 'name'));
      $select->condition('n.nid', $node->nid);
      $r = $select->execute()->fetchObject();
      if ($r) {
        $pop_mmtid = $r->mmtid;
      }
    }

    $parents = mm_content_get_parents($pop_mmtid);
    array_shift($parents);  // skip root
    $pop_start = implode('/', $parents) . "/$pop_mmtid";
  }
  else {
    mm_parse_args($mmtids);
    $pop_start = implode('/', $mmtids);
  }

  $form['where']['redir_mmtid'] = array(
    '#title' => t('Destination page (internal redirect):'),
    '#type' => 'mm_catlist',
    '#default_value' => $mmlist,
    '#mm_list_popup_start' => $pop_start,
    '#mm_list_max' => 1,
    '#mm_list_selectable' => MM_PERMS_READ,
    '#mm_list_other_name' => 'redir_url',
    '#description' => t('Enter the absolute URL or choose the CMS page you want the user to go to when this page is accessed. When entering a URL be sure to use the format <code>http://...</code>')
  );
  mm_static('node_redir', TRUE);

  return $form;
}

/**
 * Implements hook_node_presave(), actually called by monster_menus_node_presave()
 */
function mm_node_redir_all_nodes_presave($node) {
  if (!empty($node->redir_mmtid)) {
    if (is_array($node->redir_mmtid)) {
      $node->redir_mmtid = mm_ui_mmlist_key0($node->redir_mmtid);
    }
  }
  elseif (!empty($node->redir_url)) {
    $node->redir_url = trim($node->redir_url);
  }
}

/**
 * Implements hook_node_revision_delete(), actually called by
 * monster_menus_node_revision_delete().
 */
function mm_node_redir_all_nodes_revision_delete($node) {
  // Notice that we're matching a single revision based on the node's vid.
  db_delete('mm_node_redir')
    ->condition('vid', $node->vid)
    ->execute();
}

/**
 * Implements hook_validate().
 */
function mm_node_redir_validate(&$node) {
  if (!empty($node->redir_mmtid)) {
    $mmtid = mm_ui_mmlist_key0($node->redir_mmtid);

    if (!isset($mmtid) || !mm_content_user_can($mmtid, MM_PERMS_READ)) {
      form_set_error('redir_mmtid',
          t('You are not allowed to redirect to page %cat.',
          array('%cat' => $node->redir_mmtid[$mmtid])));
    }

    $node->redir_mmtid = $mmtid;
  }
  elseif (!empty($node->redir_url)) {
    $redir_url = trim($node->redir_url);

    if (!valid_url($redir_url, TRUE))
      form_set_error('redir_url', t('The destination you entered does not seem to be a valid URL.'));
  }
  else {
    form_set_error('redir_url', t('You must either enter a URL or choose a page to redirect to.'));
  }
}

/**
 * Implements hook_insert().
 */
function mm_node_redir_insert($node) {
  $data = array(
    'nid' => $node->nid,
    'vid' => $node->vid,
    'url' => isset($node->redir_url) ? $node->redir_url : '',
    'mmtid' => empty($node->redir_mmtid) ? 0 : $node->redir_mmtid,
  );
  drupal_write_record('mm_node_redir', $data);
}

/**
 * Implements hook_update().
 */
function mm_node_redir_update($node) {
  // if this is a new node or we're adding a new revision,
  if ($node->revision) {
    mm_node_redir_insert($node);
  }
  else {
    $data = array(
      'vid' => $node->vid,
      'mmtid' => $node->redir_mmtid,
      'url' => $node->redir_url,
    );
    drupal_write_record('mm_node_redir', $data, 'vid');
  }
}

/**
 * Implements hook_delete().
 */
function mm_node_redir_delete($node) {
  // Notice that we're matching all revisions, by using the node's nid.
  db_delete('mm_node_redir')
    ->condition('nid', $node->nid)
    ->execute();
}

/**
 * Implements hook_load().
 */
function mm_node_redir_load($nodes) {
  $vids = array();
  foreach ($nodes as $node) {
    $vids[] = $node->vid;
  }
  $select = db_query('SELECT * FROM {mm_node_redir} WHERE vid IN (:vids)', array(':vids' => $vids));
  foreach ($select as $obj) {
    $nodes[$obj->nid]->redir_url = $obj->url;
    $nodes[$obj->nid]->redir_mmtid = $obj->mmtid;
  }
}

/**
 * Implements hook_view().
 */
function mm_node_redir_view($node, $view_mode) {
  if (node_access('update', $node)) {
    if (!empty($node->redir_mmtid)) {
      if (mm_content_get($node->redir_mmtid)) {
        $dest = 'mm/' . $node->redir_mmtid;
        $abs = FALSE;
      }
    }
    elseif (!empty($node->redir_url)) {
      $dest = $node->redir_url;
      $abs = TRUE;
    }

    if (isset($abs)) {
      $output = t('This page would normally redirect the user to !dest. You are seeing this page because you are allowed to change the redirection.',
        array('!dest' => l(t('this location'), $dest, array('absolute' => $abs))));
    }
    else {
      drupal_set_message(t('A redirector on this page seems to be broken. This can happen when it refers to another page in the CMS which has been deleted. Please click on the Edit link to correct the problem.'), 'error');
      $output = t('This is a broken redirector.');
    }
    if ($view_mode == 'teaser') {
      $node->content['teaser']['#markup'] = $output;
      $node->content['body']['#markup'] = '';
    }
    else {
      $node->content['teaser']['#markup'] = '';
      $node->content['body']['#markup'] = $output;
    }
  }
  elseif ($view_mode == 'teaser' || $view_mode == 'full') {
    if (!empty($node->redir_mmtid)) {
      if (mm_content_get($node->redir_mmtid)) {
        mm_redirect_to_mmtid($node->redir_mmtid);
      }
    }
    elseif (!empty($node->redir_url)) {
      drupal_goto($node->redir_url);
    }
    else {
      $node->no_attribution = TRUE;
      $node->title = '';
    }
  }
  return $node;
}

/**
 * Implements hook_mm_fix_node_urls_info().
 */
function mm_node_redir_mm_fix_node_urls_info() {
  return array(
    'redir_url' => array(
      'table' => 'mm_node_redir',
      'join on' => '%alias.vid = node.vid',
      'table field' => 'url',
      'get' => '_mm_node_redir_mm_fix_node_urls_get',
      'set' => '_mm_node_redir_mm_fix_node_urls_set',
    ),
  );
}

function _mm_node_redir_mm_fix_node_urls_get($node) {
  return isset($node->redir_url) ? $node->redir_url : NULL;
}

function _mm_node_redir_mm_fix_node_urls_set($value, $node) {
  $node->redir_url = $value;
}

/******************** Subpage List functions start here *********************/

/**
 * Implements hook_form().
 */
function mm_node_subpglist_form(&$node, &$form_state) {
  drupal_set_title(!empty($node->nid) ? t('Edit a subpage list') : t('Create a subpage list'));
  $defaults = _mm_node_subpglist_settings();
  $form = array();

  $type = node_type_get_type($node);
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => check_plain($type->title_label),
    '#required' => FALSE,
    '#default_value' => $node->title,
    '#weight' => -5
  );

  $use_node = !empty($node->nid) || isset($form_state['values']);

  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => FALSE
  );
  $form['appearance']['columns'] = array(
    '#type' => 'select',
    '#title' => t('Columns'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => $use_node ? $node->columns : $defaults->columns,
  );
  $form['appearance']['depth'] = array(
    '#type' => 'select',
    '#title' => t('Depth'),
    '#options' => drupal_map_assoc(range(1, 10)),
    '#default_value' => $use_node ? $node->depth : $defaults->depth,
  );
  foreach (array_keys((array)$defaults) as $key) {
    if ($key != 'columns' && $key != 'depth') {
      $form['appearance'][$key] = array(
        '#type' => 'textfield',
        '#title' => t(drupal_ucfirst(str_replace('_', ' ', $key))),
        '#default_value' => $use_node ? $node->$key : $defaults->$key,
        '#size' => 50,
        '#maxlength' => 255,
      );
    }
  }

  $form['postamble'] = array(
    '#type' => 'textarea',
    '#title' => t('Postamble'),
    '#default_value' => empty($node->postamble) ? '' : $node->postamble,
    '#rows' => 5
  );

  return $form;
}

function _mm_node_subpglist_settings() {
  return (object)array(
    'columns' => 2,
    'depth' => 1,
    'outer_prefix' => '<table border="0"><tbody><tr valign="top">',
    'column_prefix' => '<td><ul>',
    'section_prefix' => '<ul>',
    'row_prefix' => '<li>',
    'row_suffix' => '</li>',
    'section_suffix' => '</ul>',
    'column_suffix' => '</ul></td>',
    'outer_suffix' => '</tr></tbody></table>',
  );
}

/**
 * Implements hook_insert().
 */
function mm_node_subpglist_insert($node) {
  drupal_write_record('mm_node_subpglist', $node);
}

/**
 * Implements hook_update().
 */
function mm_node_subpglist_update($node) {
  // if this is not a new node or we're adding a new revision
  if (empty($node->revision)) {
    db_delete('mm_node_subpglist')
      ->condition('vid', $node->vid)
      ->execute();
  }
  mm_node_subpglist_insert($node);
}

/**
 * Implements hook_node_revision_delete(), actually called by
 * monster_menus_node_revision_delete().
 */
function mm_node_subpglist_all_nodes_revision_delete($node) {
  // Notice that we're matching a single revision based on the node's vid.
  db_delete('mm_node_subpglist')
    ->condition('vid', $node->vid)
    ->execute();
}

/**
 * Implements hook_delete().
 */
function mm_node_subpglist_delete($node) {
  // Notice that we're matching all revision, by using the node's nid.
  db_delete('mm_node_subpglist')
    ->condition('nid', $node->nid)
    ->execute();
}

/**
 * Implements hook_load().
 */
function mm_node_subpglist_load($nodes) {
  $vids = array();
  foreach ($nodes as $node) {
    $vids[] = $node->vid;
  }
  $select = db_query('SELECT * FROM {mm_node_subpglist} WHERE vid IN (:vids)', array(':vids' => $vids));
  foreach ($select as $obj) {
    foreach (array_keys((array)$obj) as $key) {
      if ($key != 'nid' && $key != 'vid') {
        $nodes[$obj->nid]->$key = $obj->$key;
      }
    }
  }
}

/**
 * Implements hook_view().
 */
function mm_node_subpglist_view($node, $view_mode) {
  $teaser = $view_mode == 'teaser';
  if ($teaser) {
    $node->readmore = TRUE;
    return $node;
  }

  mm_parse_args($mmtids, $oarg_list, $this_mmtid);
  if (!isset($this_mmtid)) {
    $with_node = mm_content_get_by_nid($node->nid);
    if (!count($with_node)) {
      return $node;
    }
    $this_mmtid = $with_node[0];
  }

  $tree_unfiltered = mm_content_get_tree($this_mmtid,
    array(
      MM_GET_TREE_RETURN_PERMS => TRUE,
      MM_GET_TREE_RETURN_BLOCK => TRUE,
      MM_GET_TREE_DEPTH        => $node->depth,
      MM_GET_TREE_ADD_TO_CACHE => TRUE,
    )
  );
  array_shift($tree_unfiltered);

  // There are two types of filtering that need to happen. One: Completely
  // remove (prune) all items (and their kids) that the user can't possibly see.
  // Two: Separate out the items that most users can't see, but the current
  // user can. Keep the topmost parent only.
  $shown = $hidden = array();
  $hide_skip_level = $prune_skip_level = 0;
  foreach ($tree_unfiltered as $item) {
    if ($item->level <= $prune_skip_level) $prune_skip_level = 0;
    if ($item->level <= $hide_skip_level) $hide_skip_level = 0;
    if (!$prune_skip_level) {
      if (($item->bid == '' || $item->bid == MM_MENU_BID || $item->bid < 0 ||
          $item->hidden && ($item->perms[MM_PERMS_WRITE] || $item->perms[MM_PERMS_SUB] || $item->perms[MM_PERMS_APPLY])) && ($item->name != MM_ENTRY_NAME_RECYCLE || mm_content_user_can_recycle($item->mmtid, MM_PERMS_READ))) {
        if (!$hide_skip_level) {
          if (!$item->perms[MM_PERMS_IS_RECYCLED] && !$item->hidden) {
            $shown[] = $item;
          }
          else {
            $hide_skip_level = $item->level;
            $hidden[] = $item;
          }
        }
      }
      else {
        $prune_skip_level = $item->level;
      }
    }
  }
  unset($tree_unfiltered);

  $link = "mm/$this_mmtid/";
  $out = array('subpglist' => array(), 'subpglist-hidden' => array());
  $section = 'subpglist';
  foreach (array($shown, $hidden) as $tree) {
    if (count($tree)) {
      $cols = min($node->columns, count($tree));
      $tree = array_values($tree);
      $branches = array();
      $cur_branch = $this_mmtid;
      foreach ($tree as $leaf) {
        if ($leaf->parent == $this_mmtid) $cur_branch = $leaf->mmtid;
        $branches[$cur_branch][] = $leaf;
      }

      $per_col = floor(count($tree) / $cols);
      $remainder = count($tree) - $per_col * $cols;
      $col = 0;
      $cur_in_col = 0;
      $rem_in_col = $remainder > 0;
      $cur_branch = 0;
      $out[$section][$col] = array(
        '#prefix' => $node->column_prefix,
        '#suffix' => $node->column_suffix,
      );
      foreach ($branches as $branch) {
        if (($cur_in_col >= $per_col + $rem_in_col && $col != $cols - 1) ||
            (count($branches) - $cur_branch == $cols - $col - 1)) {
          $cur_in_col = 0;
          $rem_in_col = --$remainder > 0;
          $col++;
          $out[$section][$col] = array(
            '#prefix' => $node->column_prefix,
            '#suffix' => $node->column_suffix,
          );
        }
        $index = 0;
        $out[$section][$col][] = mm_node_subpglist_render_branch($branch, $index, $link, $node);
        $cur_in_col += count($branch);
        $cur_branch++;
      }
    }
    $section = 'subpglist-hidden';
  }

  if (count($out['subpglist'])) {
    $out['subpglist']['#prefix'] = $node->outer_prefix;
    $out['subpglist']['#suffix'] = $node->outer_suffix;
    $out['subpglist']['#weight'] = 1;
  }

  if (count($out['subpglist-hidden'])) {
    $out['subpglist-hidden']['#weight'] = 2;
    $out['subpglist-hidden']['#prefix'] = (isset($node->subpglist_hidden_title) ? $node->subpglist_hidden_title : t('<p>The following item(s) are normally hidden:</p>')) . $node->outer_prefix;
    $out['subpglist-hidden']['#suffix'] = $node->outer_suffix;
  }

  $node->content['subpglist'] = array(
    '#weight' => 1,
    $out,
  );

  if (!empty($node->postamble)) {
    if (substr($node->postamble, 0, 3) != '<p>') {
      $node->postamble = "<p>$node->postamble</p>";
    }
    $node->content['postamble'] = array(
      '#markup' => $node->postamble,
      '#weight' => 2,
    );
  }
  return $node;
}

function mm_node_subpglist_render_branch($branch, &$index, $link, $node) {
  $out = array();
  $start_level = $branch[$index]->level;
  $max = count($branch);

  while ($index < $max && $branch[$index]->level == $start_level) {
    $leaf = $branch[$index++];

    $sub = array();
    while ($index < $max && $branch[$index]->level > $start_level) {
      $sub[] = array(
        '#prefix' => $node->section_prefix,
        'list' => mm_node_subpglist_render_branch($branch, $index, $link, $node),
        '#suffix' => $node->section_suffix,
      );
    }

    $out[] = array(
      '#prefix' => $node->row_prefix,
      array('#markup' => l(mm_content_get_name($leaf), $link . $leaf->mmtid)),
      '#suffix' => $node->row_suffix,
      $sub,
    );
  }

  return $out;
}

/******************** Alert functions start here *********************/

function monster_menus_form_alert_node_form_alter(&$form) {
  $form['publishing']['#weight'] = 19;
  $form['publishing']['#collapsed'] = FALSE;
  $form['publishing']['publish_on']['#required'] = TRUE;
  $form['publishing']['publish_on']['#description'] = '';
  $form['publishing']['publish_on']['#default_value'] = _mm_ui_date_to_string(REQUEST_TIME);
}

/**
 * Implements hook_form().
 */
function mm_node_alert_form(&$node) {
  drupal_set_title(!empty($node->nid) ? t('Edit a system alert') : t('Create a system alert'));
  $form = array();

  $type = node_type_get_type($node);
  $form['title'] = array('#type' => 'textfield', '#title' => check_plain($type->title_label), '#required' => TRUE, '#default_value' => $node->title, '#weight' => -5);
  // Use default body node form
  return $form;
}

function mm_node_alert_add_to_footer() {
  global $user;
  if (!$user->uid) return '';

  $output = '';

  $frequency = variable_get('mm_node_alert_frequency', 'once');
  if ($frequency == 'once') {
    $last = !empty($user->timestamp) ? $user->timestamp : $user->access;
  }
  elseif ($frequency == 'login') {
    if (empty($_SESSION['mm_node_displayed_alert'])) {
      $last = 0;
      $_SESSION['mm_node_displayed_alert'] = TRUE;
    }
    else {
      return;
    }
  }
  elseif ($frequency == 'constantly') {
    $last = 0;
  }

  // Note: If this method proves not to work well, go to using the history table
  // instead. Just remember to call node_tag_new($node->nid) here and clear-out
  // any entries when the node is edited.
  $select = db_select('node', 'n');
  $select->join('mm_node_schedule', 's', 's.nid = n.nid');
  $select->addField('n', 'nid');
  $select->addExpression('IF(s.set_change_date = 1 AND s.publish_on > 0, s.publish_on, n.changed)', 'sort_column');
  $select->condition('n.status', 1)
    ->condition('n.type', 'alert')
    ->condition('s.publish_on', REQUEST_TIME, '<=')
    ->condition(db_or()
      ->condition('s.unpublish_on', 0)
      ->condition('s.unpublish_on', REQUEST_TIME, '>')
    )
    ->condition(db_or()
      ->where(':last = 0', array(':last' => $last))
      ->where(':last BETWEEN s.publish_on AND n.changed - 1', array(':last' => $last))
      ->condition('s.publish_on', $last, '>')
    );
  $select->orderBy('n.sticky', 'DESC');
  $select->orderBy('sort_column', 'DESC');
  if (!empty($_SESSION['mm_node_alert_avoid_nid']))
    $select->condition('n.nid', $_SESSION['mm_node_alert_avoid_nid'], 'NOT IN');

  $result = $select->execute();
  $nids = array();
  while ($nid = $result->fetchField())
    $nids[] = $nid;

  foreach (node_load_multiple($nids) as $nid => $node) {
    if (mm_content_user_can_node($nid, MM_PERMS_READ) && !mm_content_node_is_recycled($node, MM_NODE_RECYCLED_MMTID_CURR)) {
      $rendered = node_view($node);
      $display = drupal_render($rendered);
      if (strpos($display, '!!!Do not display this page!!!') === FALSE) {
        $output .= '<div style="display:none" id="mm-alert-' . $node->nid . '" class="mm-alert">' . $display . '<div class="mm-alert-close"><input type="button" value="' . t('Close') . '" /></div></div>';
        mm_static('node_alert', TRUE);
      }
      else {
        $_SESSION['mm_node_alert_avoid_nid'][] = $node->nid;
      }
    }
  }
  return $output;
}

/**
 * Implements hook_user_logout().
 */
function mm_alert_user_logout($account) {
  // The access field is used by mm_node_alert_add_to_footer(), but only
  // gets updated at most every 180 seconds. Update it now, so that it's
  // accurate when the user next logs-in.
  db_update('users')
    ->fields(array('access' => REQUEST_TIME))
    ->condition('uid', $account->uid)
    ->execute();
}

/******************* Block Contents functions start here ********************/

/**
 * Implements hook_node_type_insert(), actually called by
 * monster_menus_node_type_insert().
 */
function mm_node_mm_block_all_nodes_node_type_insert($content_type) {
  // Set the body label.
  $body_instance = node_add_body_field($content_type, t('Prefix'));

  // Save our changes to the body field instance.
  field_update_instance($body_instance);

  // Create all the fields we are adding to our content type.
  field_create_field(array(
    'field_name'  => 'mm_block_block',
    'cardinality' => 1,
    'module'      => 'list',
    'type'        => 'list_text',
    'settings' => array(
      'allowed_values'          => array(),
      'allowed_values_function' => '_mm_node_mm_block_list_blocks',
    ),
  ));

  // Create all the instances for our fields.
  field_create_instance(array(
    'field_name'  => 'mm_block_block',
    'bundle'      => 'mm_block',
    'entity_type' => 'node',
    'label'       => t('Block to display'),
    'type'        => 'text',
    'required'    => 1,
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type'  => 'hidden',
      ),
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'type'   => 'options_select',
    ),
    'widget_type' => 'optionwidgets_select',
  ));

  variable_set('node_submitted_mm_block', 0);
  variable_set('signup_node_default_state_mm_block', 'disabled');
  variable_set('comment_mm_block', '0');
}

/**
 * Implements hook_form().
 */
function mm_node_mm_block_form(&$node) {
  drupal_set_title(!empty($node->nid) ? t('Edit a block contents item') : t('Create a block contents item'));
  return array();
}

/**
 * Implements hook_view().
 */
function mm_node_mm_block_view($node, $view_mode) {
  static $recursive;
  global $theme_key;

  if ($view_mode != 'teaser') {
    if (++$recursive > 5) {
      throw new Exception(t('The Block Contents node #!link caused an infinite loop because it has children which refer to other Block Contents nodes.', array('!link' => l($node->nid, "node/$node->nid/edit"))));
    }
    $node->content['teaser'] = array('#markup' => '');
    $lang = empty($node->language) ? LANGUAGE_NONE : $node->language;
    list($module, $delta) = explode(':', $node->mm_block_block[$lang][0]['value'], 2);
    $blocks = array("{$module}_{$delta}" => (object)array(
      'module' => $module,
      'delta' => $delta,
      'theme' => $theme_key,
      'region' => '',
      'title' => '',
    ));
    // This currently only handles one block per node. To allow for more, we'd
    // have to decide what to do about the $node->title.
    $rendered = _block_render_blocks($blocks);
    $block = reset($rendered);
    $node->title = $block->subject;
    $node->content['mm_block'] = array(
      '#weight' => 1000,
      $block->content,
    );
    $recursive--;
  }

  return $node;
}

function _mm_node_mm_block_list_blocks() {
  foreach (module_implements('block_info') as $module) {
    $module_blocks[$module] = module_invoke($module, 'block_info');
  }

  $query = db_select('block', 'b')
    ->fields('b')
    ->addTag('block_load')
    ->addTag('translatable')
    ->execute();

  $blocks = array();
  foreach ($query as $block) {
    if (isset($module_blocks[$block->module][$block->delta])) {
      $blocks[$block->module . ':' . $block->delta] = $module_blocks[$block->module][$block->delta]['info'];
    }
  }

  natcasesort($blocks);
  return $blocks;
}