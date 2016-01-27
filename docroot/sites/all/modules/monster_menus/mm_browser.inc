<?php

/**
 * @file
 * Tree browser for Monster Menus
 */

function mm_browser_menu() {
  // Tree Browser Specific Items
  $items = array();
  $items['mm-browser'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_json',
    'page arguments' => array(1),
    'access arguments' => array('use tree browser'),
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-browser-load'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_load',
    'access arguments' => array('use tree browser'),
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-browser-getright'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_getright',
    'page arguments' => array(FALSE),
    'access arguments' => array('access content'),
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-browser-bookmark-exists'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_bookmark_exists',
    'page arguments' => array(FALSE),
    'access arguments' => array('use tree browser'),
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-browser-get-bookmarks'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_get_bookmarks',
    'page arguments' => array(FALSE),
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-browser-get-lastviewed'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_get_lastviewed',
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-browser-set-lastviewed'] = array(
    'title' => 'JSON Callback',
    'page callback' => 'mm_browser_set_lastviewed',
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-bookmarks/add'] = array(
    'title' => 'Add Bookmark',
    'page callback' => 'mm_browser_bookmark_add',
    'page arguments' => array(2),
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-bookmarks/manage'] = array(
    'title' => 'Add Bookmark',
    'page callback' => 'mm_browser_bookmark_manage',
    'page arguments' => array(FALSE),
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-bookmarks/delete'] = array(
    'title' => 'Delete Bookmark',
    'page callback' => 'mm_browser_bookmark_delete',
    'page arguments' => array(FALSE),
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-bookmarks/edit'] = array(
    'title' => 'Edit Bookmark',
    'page callback' => 'mm_browser_bookmark_edit',
    'page arguments' => array(FALSE),
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  $items['mm-bookmarks/sort'] = array(
    'title' => 'Sort Bookmarks',
    'page callback' => 'mm_browser_bookmark_sort',
    'page arguments' => array(FALSE),
    'access callback' => 'user_is_logged_in',
    'file' => 'mm_browser.inc',
    'type' => MENU_CALLBACK);
  return $items;
}

/**
 * Generate a tree browser and print the resulting HTML.
 *
 * @param $start
 *   Additional parameters to be passed to the underlying rendering code,
 *   concatenated with '-' as the separator. The current pattern is:
 *       startID-mode-instance-enabled-selectable
 *   -startID:    Tree ID to start the tree with
 *   -mode:       One of the MM_BROWSER_MODE_* constants
 *   -instance:   Numeric instance of the browser on the page
 *   -enabled:    If specified, only allow the user to click on elements for which
 *                they have this type of access. Must be one of MM_PERMS_READ,
 *                MM_PERMS_WRITE, MM_PERMS_SUB, MM_PERMS_APPLY.
 *   -selectable: If specified, only show a 'select' option for elements for which
 *                they have this type of access. Must be one of MM_PERMS_READ,
 *                MM_PERMS_WRITE, MM_PERMS_SUB, MM_PERMS_APPLY.
 * @param $path...
 *   Any remaining parameters are a path describing the pre-selected tree ID
 */
function mm_browser_load() {
  $path = func_get_args();
  $start = array_shift($path);
  $params = explode('-', $start);
  if (isset($path[0]) && $path[0] != 1 && $path[0] != $params[0]) {
    array_unshift($path, $params[0]);
  }
  elseif (!count($path)) {
    $path[] = 1;
  }

  for ($i = 0; $i < 7; $i++)
    if (!isset($params[$i])) $params[$i] = '';

  print theme('mm_page_wrapper', array(
    'title' => t('Tree Browser'),
    'body' => mm_browser($path[count($path) - 1], $path[0], $params[1], $params[3], $params[4], $params[5], $params[6]),
    'wrap_body_with_div' => FALSE,
    'attributes' => array(
      'id' => 'mm-media-assist-load',
      'class' => array('mm-media-assist'),
    ),
  ));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit;
}

function mm_browser($selected, $top_mmtid = 1, $mode = MM_BROWSER_MODE_PAGE, $enabled = MM_PERMS_READ, $selectable = '', $title = '', $field_id = NULL) {
  if (empty($title)) {
    $titles = array(
      MM_BROWSER_MODE_GALLERY => t('Select a file or page of files'),
      MM_BROWSER_MODE_GROUP =>   t('Select a group'),
      MM_BROWSER_MODE_WYSIWYG => t('Select a file upload'),
      MM_BROWSER_MODE_RSS =>     t('Select a page to display'),
      MM_BROWSER_MODE_USER =>    t('Select a user'),
      MM_BROWSER_MODE_NODE =>    t('Select a piece of content'),
    );
    if (isset($titles[$mode])) $title = $titles[$mode];
    else $title = t('Select a page');
  }

  drupal_add_js(array(
    'browserInstanceId' => empty($_GET['instanceId']) ? 0 : $_GET['instanceId'],
    'libraryPath' => drupal_get_path('module', 'monster_menus') . '/libraries',
  ), 'setting');
  if (!$top_mmtid) $top_mmtid = 1;
  drupal_add_js(array(
    'browserDots'       => $mode == MM_BROWSER_MODE_GROUP || $mode == MM_BROWSER_MODE_ADMIN_GROUP || user_access('administer all menus') || user_access('view all menus'),
    'browserEnabled'    => $enabled,
    'browserMode'       => $mode,
    'browserSelectable' => $selectable,
    'browserTop'        => $top_mmtid,
    'lastBrowserPath'   => _mm_browser_get_relative_path(_mm_browser_get_lastviewed($mode), $top_mmtid),
    'startBrowserPath'  => _mm_browser_get_relative_path($selected, $top_mmtid),
  ), 'setting');
  if (!empty($field_id)) {
    drupal_add_js(array('browserFieldID' => $field_id), 'setting');
  }
/****  Use this code to jump to the last viewed location
  if ($last_mmtid = _mm_browser_get_lastviewed()) {
    drupal_add_js(array('lastBrowserPath' => mm_content_get_full_path($last_mmtid)), 'setting');
  }
****/
  mm_static('back_in_history', TRUE);

  drupal_add_library('system', 'ui');
  drupal_add_library('monster_menus', 'fg.menu');
  drupal_add_library('monster_menus', 'splitter');
  drupal_add_library('monster_menus', 'jsTree');
  drupal_add_library('monster_menus', 'mm_browser');

  drupal_add_css(drupal_get_path('module', 'monster_menus') . '/css/mm_browser.css');

  // Suppress the admin module
  module_invoke_all('suppress');

  $out = '<div id="mmtree-browse"><div id="mmtree-browse-nav">';
  $out .= mm_browser_getURLs($mode, $top_mmtid);
  $out .= '<h2 class="mmtree-assist-title">' . $title . '</h2></div>';
  $out .= '<div id="mmtree-browse-browser">';
  $out .= '<div id="mmtree-browse-tree" class="resizable-frame"></div>';
  $out .= '<div id="mmtree-browse-items"><div id="mmtree-browse-header"><h4 id="mmtree-assist-title"></h4><div id="mmtree-assist-links"></div></div><div id="mmtree-assist-content"></div></div>';
  $out .= '</div></div>';
  return $out;
}

/*
 *  Create the list of URLs at the top of the tree browser.
 */
function mm_browser_getURLs($mode, $top_mmtid) {
  global $user, $base_url;

  $urls = '<ul class="dropdown">';
  $allowed_top = $mode == MM_BROWSER_MODE_GROUP || $mode == MM_BROWSER_MODE_ADMIN_GROUP ? mm_content_groups_mmtid() : 1;

  if ($top_mmtid != $allowed_top) {
    $urls .= '<li><a href="#" onclick="Drupal.mm_browser_goto_top(\'' . $allowed_top . '\');" class="fg-button fg-button-icon-right ui-widget ui-state-default ui-corner-all">' . t('View entire tree') . '</a></li>';
  }

  if ($mode != MM_BROWSER_MODE_GROUP && $mode != MM_BROWSER_MODE_ADMIN_GROUP && isset($user->user_file_mmtid) && is_numeric($user->user_file_mmtid) && $user->user_file_mmtid > 0) {
    $urls .= '<li><a href="#" onclick="Drupal.mm_browser_reload_data(\'' . mm_content_get_full_path($user->user_file_mmtid) . '\');" class="fg-button fg-button-icon-right ui-widget ui-state-default ui-corner-all" id="my-uploaded-files-link">' . t('My uploaded files') . '</a></li>';
  }
  $urls .= '<li><span id="last-viewed"><a href="#" onclick="Drupal.mm_browser_last_viewed();" id="last-viewed-link" class="fg-button fg-button-icon-right ui-widget ui-state-default ui-corner-all">' . t('Last location') . '</a></span></li>';
  $urls .= join('', mm_module_invoke_all('mm_browser_navigation', $mode, $top_mmtid));

  // Display bookmarks.
  $urls .= '<li id="bookmarks-link"><a href="#" class="fg-button fg-button-icon-right ui-widget ui-state-default ui-corner-all mm-browser-button">' . t('Bookmarks') . ' <img src="' . $base_url . '/misc/menu-expanded.png" alt="" /></a><div class="mm-browser-button-list" class="hidden"><ul>';
  $urls .= '<li><a href="#" onclick="Drupal.mm_browser_manage_bookmarks();">' . t('Organize Bookmarks...') . '</a></li><li><hr /></li><span class="bm-list">';

  $result = _mm_browser_get_bookmarks($mode);
  while ($row = $result->fetchAssoc()) {
    $ds_data = unserialize($row['data']);
    $urls .= '<li class="' . $ds_data['mmtid'] . '"><a href="#" onclick="Drupal.mm_browser_reload_data(\'' . _mm_browser_get_relative_path($ds_data['mmtid'], $top_mmtid) . "');\">" . check_plain($ds_data['title']) . '</a></li>';
  }
  $urls .= '</span>';
  $urls .= '</ul></div></li>';
  $urls .= '</ul>';
  return $urls;
}

function mm_browser_json() {
  $term_ids = func_get_args();
  $num_ids = func_num_args();
  $mmtid = isset($_GET['id']) ? intval(substr($_GET['id'], 5)) : 0;

  if ($mmtid) {
    $mm_children = mm_browser_get_tree($num_ids, $mmtid, 0, 'single');
  }
  else {
    $num_ids--;
    $mm_children = mm_browser_get_tree($num_ids, $term_ids, 1, 'multi');
  }
  drupal_add_http_header('Pragma', 'no-cache');
  drupal_json_output($mm_children);
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

function mm_browser_get_tree(&$num_ids, &$term_ids, $index, $level) {
  $mm_children = array();
  $mmtid = is_array($term_ids) ? $term_ids[$index] : $term_ids;
  if (!$mmtid) return $mm_children;

  $params = array(
    MM_GET_TREE_ADD_TO_CACHE =>     TRUE,
    MM_GET_TREE_FILTER_BINS =>      FALSE,
    MM_GET_TREE_FILTER_DOTS =>      $_GET['browserDots'] == 'true',
    MM_GET_TREE_FILTER_HIDDEN =>    TRUE,
    MM_GET_TREE_DEPTH =>            1,
    MM_GET_TREE_RETURN_KID_COUNT => TRUE,
    MM_GET_TREE_RETURN_PERMS =>     TRUE,
  );
  switch ($_GET['browserMode']) {
    case MM_BROWSER_MODE_NODE:
      $allowed_node_types = array();
      if (isset($_GET['browserFieldID']) && function_exists('mm_fields_get_widget_settings')) {
        list($field_name, $bundle) = explode(',', $_GET['browserFieldID']);
        $instance = field_info_instance('node', $field_name, $bundle);
        if (!empty($instance)) {
          $settings = mm_fields_get_widget_settings($instance);
          foreach ($settings['mm_list_nodetypes'] as $node_type) {
            if ($node_type !== 0) {
              $allowed_node_types[] = "'" . $node_type . "'";
            }
          }
        }
      }
      $wheres = $allowed_node_types ? 'AND nd.type IN (' . implode(', ', $allowed_node_types) . ') ' : '';
      $wheres .= 'AND nd.status = 1';
      $params[MM_GET_TREE_ADD_SELECT] = "(SELECT COUNT(DISTINCT n.nid) FROM {mm_node2tree} n INNER JOIN {node} nd ON nd.nid = n.nid INNER JOIN {node_type} t ON t.type = nd.type WHERE n.mmtid = t.mmtid {$wheres}) AS nodecount ";
      $params[MM_GET_TREE_FILTER_NORMAL] = $params[MM_GET_TREE_FILTER_USERS] = TRUE;
      break;

    case MM_BROWSER_MODE_ADMIN_GROUP:
    case MM_BROWSER_MODE_GROUP:
      if ($mmtid == 1) {
        $mmtid = mm_content_groups_mmtid();
      }
      $params[MM_GET_TREE_FILTER_GROUPS] = TRUE;
      break;

    case MM_BROWSER_MODE_GALLERY:
    case MM_BROWSER_MODE_WYSIWYG:
      $segments = array();
      foreach (array_keys(_mm_browser_get_fields()) as $fid_table) {
        $segments[] = "SELECT COUNT(*) FROM $fid_table f INNER JOIN {node} n ON n.nid = f.entity_id INNER JOIN {mm_node2tree} n2 ON n2.nid = n.nid WHERE f.deleted = 0 AND n.status = 1 AND n2.mmtid = t.mmtid";
      }
      $params[MM_GET_TREE_ADD_SELECT] = '(' . join(') + (', $segments) . ') AS nodecount';
      $params[MM_GET_TREE_FILTER_NORMAL] = $params[MM_GET_TREE_FILTER_USERS] = TRUE;
      break;

    case MM_BROWSER_MODE_MENU:
      $params[MM_GET_TREE_RETURN_BLOCK] = TRUE;
      $params[MM_GET_TREE_BLOCK] = 1;
      break;

    default:
    case MM_BROWSER_MODE_ADMIN_PAGE:
    case MM_BROWSER_MODE_PAGE:
    case MM_BROWSER_MODE_RSS:
      $params[MM_GET_TREE_FILTER_NORMAL] = $params[MM_GET_TREE_FILTER_USERS] = TRUE;
      break;

    case MM_BROWSER_MODE_ADMIN_USER:
    case MM_BROWSER_MODE_USER:
      $params[MM_GET_TREE_FILTER_USERS] = TRUE;
      break;
  }

  $list = mm_content_get_tree($mmtid, $params);

  array_shift($list);
  foreach ($list as $item) {
    $class = $children = array();
    $state = $item->state & MM_GET_TREE_STATE_LEAF ? 'leaf' : 'closed';
    $attributes = array('id' => 'mmbr-' . $item->mmtid);

    $name = mm_content_get_name($item);
    $hidden = $item->state & MM_GET_TREE_STATE_HIDDEN;
    if ($hidden) {
      $name .= ' ' . t('(hidden)');
      $class['mmtree-hidden'] = 1;
    }
    $data = array('title' => $name);
    if (isset($item->nodecount) && $item->nodecount > 0) {
      $data['title'] = t('!name <span class="mmtree-browse-filecount">!count</span>', array('!name' => $name, '!count' => format_plural($item->nodecount, '(1 item)', '(@count items)')));
    }

    if ($_GET['browserMode'] == MM_BROWSER_MODE_MENU) {
      $path = 'mm/' . $item->mmtid;
      $options = NULL;
      mm_module_invoke_all_array('url_outbound_alter', array(&$path, &$options, NULL));
      $data['path'] = $path;
      $data['mmtid'] = $item->mmtid;
      $parent = db_select('mm_tree', 't')
        ->addTag(__FUNCTION__)
        ->fields('t', array('parent'))
        ->condition('mmtid', $mmtid)
        ->execute()->fetchField();
      $data['parent'] = $parent;
    }

    if ($state == 'leaf') {
      $class['leaf'] = 1;
    }

    $denied = $item->state & MM_GET_TREE_STATE_DENIED || !empty($_GET['browserEnabled']) && !$item->perms[$_GET['browserEnabled']];
    if ($denied) {
      $class['disabled'] = $class['leaf'] = 1;
      $state = 'leaf';
    }
    elseif ($level != 'single' && isset($term_ids[$index + 1]) && $term_ids[$index + 1] == $item->mmtid) {
      $state = 'open';
      $children = mm_browser_get_tree($num_ids, $term_ids, $index + 1, $level);
    }

    $attributes['class'] = $data['attr']['class'] = join(' ', array_keys($class));

    $mm_children[] = array(
      'attr' =>     $attributes,
      'children' => $children,
      'data' =>     $data,
      'state' =>    $state,
    );
  }
  return $mm_children;
}

function mm_browser_getright() {
  function _mm_browser_choice($item, $cat, $usr, $group) {
    if (!empty($item->is_user)) return $usr;
    if (!empty($item->is_group)) return $group;
    return $cat;
  }

  $mmtid = intval(substr($_GET['id'], 5));
  if (!$mmtid) {
    print theme('mm_page_wrapper', array());
    $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
    exit();
  }

  // Suppress the admin module
  module_invoke_all('suppress');

  $mode = $_GET['browserMode'];

  $bookmark = '';
  if (!mm_browser_is_bookmarked($mmtid, $_GET['browserMode'])) {
    $bookmark = '<a href="#" id="add-bookmark-link" title="' . t('Bookmark this location') . '" onclick="Drupal.mm_browser_add_bookmark_tb(' . $mmtid . "); this.blur();\">" . t('Bookmark') . '</a>';
  }

  $links = array();
  $item = mm_content_get($mmtid, MM_GET_FLAGS);
  $perms = $item->perms = mm_content_user_can($mmtid);
  $item->is_group = $perms[MM_PERMS_IS_GROUP];
  $item->is_user = $perms[MM_PERMS_IS_USER];
  $name = mm_content_get_name($item);

  if ($bookmark) $links['bkmark'] = $bookmark;

  if ($mode == MM_BROWSER_MODE_GALLERY) {
    $links['all'] = "<a href=\"javascript:Drupal.mm_browser_gallery_add($mmtid,'" . mm_ui_js_escape($name) . ' ' . t('(all)') . "');\" title=\"" . t('Select all current and future files on this page') . '">' . t('Select all') . '</a>';
  }
  $users = '';
  if ($mode == MM_BROWSER_MODE_ADMIN_PAGE || $mode == MM_BROWSER_MODE_ADMIN_GROUP) {
    $x = mm_ui_strings($item->is_group);

    if ($perms[MM_PERMS_READ]) {
      $links['contents'] = l(t('View Contents'), "mm/$item->mmtid", array('attributes' => array('title' => t('View this !thingpos contents', $x))));
    }

    if ($perms[MM_PERMS_WRITE] || $perms[MM_PERMS_SUB]) {
      $links['settings'] = l(t('Change Settings'), "mm/$item->mmtid/settings", array('attributes' => array('title' => t('Edit this !thing', $x))));
    }
  }
  elseif (($mode == MM_BROWSER_MODE_PAGE || $mode == MM_BROWSER_MODE_RSS || $mode == MM_BROWSER_MODE_GROUP) && empty($item->is_virtual) && _mm_browser_user_can($perms)) {
    $js_parms = 0;
    if ($mode == MM_BROWSER_MODE_GROUP) {
      $users = mm_content_get_users_in_group($mmtid, '<br />', FALSE, 20, TRUE);
      if ($users == '') $users = t('(none)');
      $js_parms = "'" . mm_ui_js_escape($users) . "'";
    }
    $links['select'] = "<a href=\"javascript:Drupal.mm_browser_page_add($mmtid, $js_parms);\">" . t('Select') . '</a>';
  }
  $links['close'] = _mm_browser_close_link($mode);
  mm_module_invoke_all_array('mm_browser_links_alter', array($mode, &$links, $item));

  switch ($mode) {
    case MM_BROWSER_MODE_NODE:
      $content = _mm_browser_node_view($mode, $mmtid, $perms);
      break;

    case MM_BROWSER_MODE_GALLERY:
    case MM_BROWSER_MODE_WYSIWYG:
      $content = _mm_browser_gallery_view($mode, $mmtid, $perms);
      break;

    default:
      $content = _mm_browser_default_view($mode, $mmtid, $perms, $item, $users);
      break;
  }

  // Get the last viewed item
  $lastviewed = '';
  if ($last_mmtid = _mm_browser_get_lastviewed($_GET['browserMode'])) {
    $lastviewed = _mm_browser_get_relative_path($last_mmtid, $_GET['browserTop']);
  }
  if (isset($_GET['id'])) mm_browser_set_lastviewed(intval(substr($_GET['id'], 5)));

  $json = array(
    'title' => $name,
    'links' => implode($links, ' | '),
    'body' => $content,
    'lastviewed' => $lastviewed,
  );
  drupal_json_output($json);

  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

// Bookmark Specific Functions
function mm_browser_bookmark_add() {
  global $base_url, $user;

  $args = func_get_args();
  $mmtid = $args[0];
  $name = mm_content_get_name($mmtid);
  $output = '';

  if (mm_browser_is_bookmarked($mmtid, $_GET['browserMode'])) {
    $output = '<link rel="stylesheet" href="' . base_path() . drupal_get_path('module', 'monster_menus') . '/css/mm_browser_tb.css" />';
    $output .= '<script type="text/javascript" src="' . $base_url . '/misc/jquery.js"></script>';
    $output .= '<script type="text/javascript" src="' . $base_url . '/sites/all/modules/thickbox/thickbox.js"></script>';
    $output .= '<body id="tb-body"><p>' . t('This location is already bookmarked.') . '</p>';
    $output .= '<a href="#" onclick="parent.tb_remove();">' . t('Close window') . '</a></body></html>';
  }
  elseif (!empty($_POST)) {
    $mm_bookmark_serialized = serialize(array('title' => $_POST['linktitle'], 'mmtid' => $_POST['linkmmtid']));
    $type = _mm_browser_get_bookmarks_type($_GET['browserMode']);
    $transaction = db_transaction();
    $select = db_select('mm_tree_bookmarks', 'b')
      ->condition('b.uid', $user->uid)
      ->condition('b.type', $type);
    $select->addExpression('IFNULL(MAX(b.weight), -1) + 1', 'max_weight');
    $max_weight = $select->execute()->fetchField();
    db_insert('mm_tree_bookmarks')
      ->fields(array(
        'uid' => $user->uid,
        'weight' => $max_weight,
        'type' => $type,
        'data' => $mm_bookmark_serialized,
      ))
      ->execute();
    unset($transaction);
  }
  else {
    $output = mm_static('mm_browser_bookmark_add', FALSE, $name, $mmtid);
  }

  print($output);
  module_invoke_all('suppress');
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

// Bookmark Specific Functions
function mm_browser_bookmark_manage() {
  global $user;

  $num_rows = db_select('mm_tree_bookmarks', 'b')
    ->condition('b.uid', $user->uid)
    ->condition('b.type', _mm_browser_get_bookmarks_type($_GET['browserMode']))
    ->countQuery()->execute()->fetchField();

  if ($num_rows < 1) {
    $output = '<div id="message"><p>' . t('No bookmarks found.') . '</p><p><a href="#" onclick="window.parent.tb_remove(); return false;">' . t('Continue') . '</a></p></div>';
  }
  else {
    drupal_add_css(drupal_get_path('module', 'monster_menus') . '/css/mm_browser_tb.css');
    mm_static('mm_browser_bookmark_manage', TRUE);
    drupal_add_library('system', 'ui.sortable');
    $output = '<div><form id="manage-bookmarks-form"><div id="manage-bookmarks-div"><ul id="sortable">';

    $result = _mm_browser_get_bookmarks($_GET['browserMode']);
    while ($row = $result->fetchAssoc()) {
      $ds_data = unserialize($row['data']);
      $escaped_title = mm_ui_js_escape($ds_data['title']);
      $output .= '<li class="ui-state-default" id="li_' . $ds_data['mmtid'] . '" name="' . $ds_data['mmtid'] . '"><table class="manage-bookmarks-table"><tr id=' . $ds_data['mmtid'] . '><td class="tb-manage-name"><span class="ui-icon ui-icon-arrowthick-2-n-s"></span>' . check_plain($ds_data['title']) . '</td><td><a href="#" onclick="return window.parent.Drupal.mm_browser_delete_bookmark_confirm(' . $ds_data['mmtid'] . ', \'' . $escaped_title . '\', document)">' . t('Delete') . '</a></td><td><a href="#" onclick="return window.parent.Drupal.mm_browser_edit_bookmark_edit(' . $ds_data['mmtid'] . ', \'' . $escaped_title . '\', document)">' . t('Edit') . '</a></td></tr></table></li>';
    }
    $output .= '</ul></div></form>';
  }

  print(theme('mm_page_wrapper', array(
    'title' => '',
    'body' => $output,
    'attributes' => array('id' => 'tb-manage-body'),
  )));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

// Check to ensure a bookmark does not already exist prior to adding it.
function mm_browser_is_bookmarked($mmtid, $mode) {
  $already_exists = FALSE;
  $result = _mm_browser_get_bookmarks($mode);
  foreach ($result as $row) {
    $ds_data = unserialize($row->data);
    if ($ds_data['mmtid'] == $mmtid) {
      $already_exists = TRUE;
      break;
    }
  }
  return $already_exists;
}

// Quick check to see if bookmark exists with JSON return.
function mm_browser_bookmark_exists() {
  drupal_json_output(array('exists' => mm_browser_is_bookmarked($_GET['id'], $_GET['browserMode'])));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

// Delete the Posted bookmark
function mm_browser_bookmark_delete() {
  global $user;

  $mmtid = $_POST['mmtid'];

  $result = _mm_browser_get_bookmarks($_GET['browserMode']);
  while ($row = $result->fetchAssoc()) {
    $ds_data = unserialize($row['data']);
    if ($ds_data['mmtid'] == $mmtid) {
      db_delete('mm_tree_bookmarks')
        ->condition('bmid', $row['bmid'])
        ->condition('uid', $user->uid)
        ->execute();
    }
  }
  drupal_json_output(array('mmtid' => $mmtid));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

// Edit the posted bookmark.
function mm_browser_bookmark_edit() {
  global $user;

  $mmtid = $_POST['mmtid'];
  $title = $_POST['title'];

  $result = _mm_browser_get_bookmarks($_GET['browserMode']);
  while ($row = $result->fetchAssoc()) {
    $ds_data = unserialize($row['data']);
    if ($ds_data["mmtid"] == $mmtid) {
      $ds_data['title'] = $title;
      $ds_done = serialize($ds_data);
      db_update('mm_tree_bookmarks')
        ->fields(array('data' => $ds_done))
        ->condition('bmid', $row['bmid'])
        ->condition('uid', $user->uid)
        ->execute();
    }
  }
  drupal_json_output(array('title' => $title, 'mmtid' => $mmtid));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

// Edit the posted bookmark.
function mm_browser_bookmark_sort() {
  global $user;

  $neworder = explode('|', $_POST['neworder']);

  $result = _mm_browser_get_bookmarks($_GET['browserMode']);
  while ($row = $result->fetchAssoc()) {
    $ds_data = unserialize($row['data']);

    for ($i = 0; $i < count($neworder); $i++) {
      if ($ds_data['mmtid'] == $neworder[$i]) {
        db_update('mm_tree_bookmarks')
          ->fields(array('weight' => $i + 1))
          ->condition('bmid', $row['bmid'])
          ->condition('uid', $user->uid)
          ->execute();
      }
    }
  }
  drupal_json_output(array('neworder' => $neworder));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

function mm_browser_get_lastviewed() {
  if ($last_mmtid = _mm_browser_get_lastviewed($_GET['browserMode'])) {
    drupal_json_output(array('path' => _mm_browser_get_relative_path($last_mmtid, $_GET['browserTop'])));
  }
  if (isset($_GET['id'])) mm_browser_set_lastviewed(intval(substr($_GET['id'], 5)));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

function _mm_browser_get_relative_path($mmtid, $top) {
  $path = mm_content_get_full_path($mmtid);
  if (preg_match('{\b' . $top . '(/|$)(.*)}', $path, $matches)) return $matches[0];
  return $path;
}

function _mm_browser_get_lastviewed($mode) {
  global $user;

  return db_select('mm_tree_bookmarks', 'b')
    ->fields('b', array('data'))
    ->condition('b.uid', $user->uid)
    ->condition('b.type', _mm_browser_get_bookmarks_type($mode) . '_last')
    ->execute()->fetchField();
}

function mm_browser_set_lastviewed($mmtid) {
  global $user;

  if ($mmtid) {
    $type = _mm_browser_get_bookmarks_type($_GET['browserMode']) . '_last';
    $num_updated = db_update('mm_tree_bookmarks')
      ->fields(array('data' => $mmtid))
      ->condition('uid', $user->uid)
      ->condition('type', $type)
      ->execute();
    if (!$num_updated) {
      db_insert('mm_tree_bookmarks')
        ->fields(array(
          'uid' => $user->uid,
          'weight' => 0,
          'type' => $type,
          'data' => $mmtid,
        ))
        ->execute();
    }
  }
}

function _mm_browser_get_bookmarks($mode) {
  global $user;

  return db_select('mm_tree_bookmarks', 'b')
    ->fields('b')
    ->condition('b.uid', $user->uid)
    ->condition('b.type', _mm_browser_get_bookmarks_type($mode))
    ->orderBy('b.weight')
    ->execute();
}

function _mm_browser_get_bookmarks_type($mode) {
  return $mode == MM_BROWSER_MODE_GROUP || $mode == MM_BROWSER_MODE_ADMIN_GROUP ? 'browser_grp' : 'browser';
}

function mm_browser_get_bookmarks() {
  // Display bookmarks.
  $bookmarks = array();
  $result = _mm_browser_get_bookmarks($_GET['browserMode']);
  while ($row = $result->fetchAssoc()) {
    $ds_data = unserialize($row['data']);
    $bookmarks[] = array($ds_data['mmtid'], $ds_data['title'], _mm_browser_get_relative_path($ds_data['mmtid'], $_GET['browserTop']));
  }
  drupal_json_output($bookmarks);
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

function _mm_browser_header($mode, $mmtid) {
  $header = '<div id="mmtree-browse-header"><h4 class="mmtree-assist-title">' . mm_content_get_name($mmtid) . '</h4>' . _mm_browser_close_link($mode) . '</div><br /><hr />';
  return $header;
}

function _mm_browser_close_link($mode) {
  return '<a href="#" onclick="' . ($mode == MM_BROWSER_MODE_WYSIWYG ? 'parent.window.close()' : 'parent.parent.tb_remove();') . '">' . t('Close window') . '</a>';
}

/**
 * Get the appropriate link for the current mode.
 * @param $mode
 *   MM browser mode constant
 * @param $item
 *   May be a node, may be a file, etc. depends on browser mode
 * @param $mmtid
 *   mmtid of current page
 * @return
 *   The HTML code of link
 */
function _mm_browser_get_link($mode, &$item, $mmtid, $file = NULL) {
  switch ($mode) {
    case MM_BROWSER_MODE_NODE:
      $onclick = "Drupal.mm_browser_nodepicker_add({$mmtid}, '" . mm_ui_js_escape($item->title) . "', {$item->nid});";
      if ($file) {
        return theme('mm_browser_thumbnail', array('file' => $file, 'mode' => $mode, 'mmtid' => $mmtid, 'onclick' => $onclick));
      }
      return l($item->title, '#', array('attributes' => array('onclick' => $onclick), 'external' => TRUE));
  }
}

function _mm_browser_node_view($mode, $mmtid, $perms) {
  // This retrieves the attributes of the content type.
  // Allow all node types by default.
  $settings = array('mm_list_nodetypes' => array());
  if (!empty($_GET['browserFieldID']) && function_exists('mm_fields_get_widget_settings')) {
    list($field_name, $bundle) = explode(',', $_GET['browserFieldID']);
    $instance = field_info_instance('node', $field_name, $bundle);
    if (!empty($instance)) {
      $settings = mm_fields_get_widget_settings($instance);
    }
  }

  if (empty($settings['mm_list_selectable'])) {
    $settings['mm_list_selectable'] = MM_PERMS_READ;
  }

  if (empty($perms[$settings['mm_list_selectable']])) {
    if ($mmtid > 0) {
      $out = '<div id="mmtree-browse-thumbnails"><br /><p>' . t('You do not have permission to use the content on this page.') . '</p>';
      $options = array(
        MM_PERMS_WRITE => t('delete it or change its settings'),
        MM_PERMS_SUB   => t('append subpages to it'),
        MM_PERMS_APPLY => t('add content to it'),
        MM_PERMS_READ  => t('read it'));
      if (isset($options[$settings['mm_list_selectable']])) {
        $out .= t('<p>To use content from this page, you must be able to @do.</p>', array('@do' => $options[$settings['mm_list_selectable']]));
      }
      $out .= '</div>';
    }
    $json = array(
      'title' => mm_content_get_name($mmtid),
      'body' => $out,
    );
    drupal_json_output($json);

    $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
    exit();
  }

  $allowed_node_types = array();
  foreach ($settings['mm_list_nodetypes'] as $node_type) {
    if ($node_type !== 0) {
      $allowed_node_types[] = $node_type;
    }
  }

  $table_header = array(
    array('data' => t('Type'), 'field' => 'n.type'),
    array('data' => t('Title'), 'field' => 'n.title'),
    array('data' => t('Last Modified'), 'field' => 'n.changed', 'sort' => 'desc'),
  );

  $select = db_select('node', 'n');
  $select->addTag(__FUNCTION__);
  $select->join('mm_node2tree', 'm', 'm.nid = n.nid');
  $select->join('node_type', 't', 'n.type = t.type');
  $select->fields('n', array('nid', 'title', 'type', 'changed'))
    ->fields('t', array('name'));
  $select->condition('m.mmtid', $mmtid);
  if ($allowed_node_types)
    $select->condition('n.type', $allowed_node_types);
  $select->condition('n.status', 1);
  $result = $select->extend('TableSort')
    ->orderByHeader($table_header)
    ->extend('PagerDefault')
    ->limit(variable_get('mm_nodelist_pager_limit', 20))
    ->execute();  // execute() must be called on the extended query, not $select.

  $nids = array();
  foreach ($result as $r) {
    $nids[$r->nid] = $r->name;
  }
  $nodes = node_load_multiple(array_keys($nids));

  $rows = array();
  foreach ($nids as $nid => $name) {
    $row = array();
    $node = $nodes[$nid];
    $file = NULL;
    if ($node->type == 'mm_media' && isset($node->field_multimedia[$node->language][0]['fid'])) {
      $file = file_load($node->field_multimedia[$node->language][0]['fid']);
      if ($file) {
        $name .= ' - ' . file_get_type($file);
        if (!empty($node->title)) {
          $file->title = check_plain($node->title);
        }
      }
    }
    $link = _mm_browser_get_link($mode, $node, $mmtid, $file);
    $row[] = check_plain($name);
    $row[] = $link;
    $row[] = format_date($node->changed, 'custom', 'M j, Y g:i A');
    $rows[] = $row;
  }

  if (!$rows) {
    $content = '<br />' . t('There is no selectable content on this page.');
  }
  else {
    $content = theme('table', array(
      'header' => $table_header,
      'rows' => $rows,
    ));
    $content .= theme('pager', array(
      'tags' => NULL,
      'element' => 0,
    ));

    // Select all link currently disabled. May be nice to enable at a later date,
    //  but I couldn't think of a way for it to not be confusing. For example, if
    //  the max allowed nodes set in the cck config mm_list node is 3, then we could theoretically
    //  allow the user to "Select all" on a page with less than 3 nodes. However, the way it is
    //  currently architected, there is no way to prevent the user from selecting a single
    //  node for each of the first 3 picks, then a select all nodes on a page with 3 nodes,
    //  thereby selecting 5 total nodes. Anyway, I couldn't think of a way to make this
    //  not confusing for the user so I disabled it, but am leaving it here in case anyone
    //  can think of a way to make it not confusing in the future.
    // It also has the problem that a user could select a page which, at the time of selection
    //  had less than 3 nodes, but was later populated to have > 3 thereby overloading.
    //  Not any graceful way to prevent this, so it sits disabled. -VJQ 2/25/2011
    // $links['all'] = "<a href=\"javascript:parent.Drupal.mm_browser_gallery_add($mmtid,'" . mm_ui_js_escape($name) . ' ' . t('(all)') . "');\" title=\"" . t('Select all current and future files on this page') . '">' . t('Select all') . '</a>';
    drupal_add_library('system', 'ui');
  }
  return $content;
}

function _mm_browser_gallery_view($mode, $mmtid, $perms) {
  if (!$perms[MM_PERMS_APPLY]) {
    $out = '';
    if ($mmtid > 0) {
      $out = '<div id="mmtree-browse-thumbnails"><br />' . t('You do not have permission to use the file uploads on this page.') . '</div>';
    }
    $json = array(
      'title' => mm_content_get_name($mmtid),
      'body' => $out,
    );
    drupal_json_output($json);
    $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
    exit();
  }

  $seg_num = 0;
  foreach (_mm_browser_get_fields() as $fid_table => $fid_field) {
    $segment = db_select($fid_table, 'f');
    $segment->addTag(__FUNCTION__ . '__' . $fid_table . '.' . $fid_field);
    $segment->fields('n', array('title', 'nid', 'changed'));
    $segment->addField('f', $fid_field, 'fid');
    $segment->join('node', 'n', 'n.nid = f.entity_id');
    $segment->join('mm_node2tree', 'n2', 'n2.nid = n.nid');
    $segment->condition('f.deleted', 0, '=')
      ->condition('n.status', 1, '=')
      ->condition('n2.mmtid', $mmtid, '=');

    if (!$seg_num++) {
      $query = $segment;
    }
    else {
      $query->union($segment);
    }
  }

  $query = db_select($query, 'subquery');
  $query->addTag(__FUNCTION__);
  $query->join('file_managed', 'm', 'm.fid = subquery.fid');
  $query->fields('m');
  $query->fields('subquery', array('title', 'nid'));
  $result = $query->extend('PagerDefault')
    ->orderBy('changed')
    ->limit(variable_get('mm_nodelist_pager_limit', 20))
    ->execute();  // execute() must be called on the extended query, not $query.

  $content = theme('mm_browser_thumbnails', array('files' => $result, 'mode' => $mode, 'mmtid' => $mmtid));

  if (empty($content)) {
    $content = '<p><br />' . t('There are no file uploads on this page.') . '</p>';
  }
  else {
    $content .= theme('pager', array('tags' => NULL, 'element' => 0));
  }
  $content = '<div id="mmtree-browse-thumbnails">' . $content . '</div>';
  drupal_add_library('system', 'ui');

  return $content;
}

function _mm_browser_default_view($mode, $mmtid, $perms, $item, $users) {
  $content = '';
  if (in_array($mode, array(MM_BROWSER_MODE_PAGE, MM_BROWSER_MODE_ADMIN_PAGE, MM_BROWSER_MODE_RSS, MM_BROWSER_MODE_GROUP, MM_BROWSER_MODE_ADMIN_GROUP)) && empty($item->is_virtual)) {
    $content = '<table>';
    if (empty($item->is_group)) {
      $content .= '<tr><td align="right"><b>' . t('URL alias:') . '</b>&nbsp;</td><td>' . check_plain($item->alias) . '</td></tr>';
    }
    if (isset($item->nodecount)) {
      $content .= '<tr><td align="right"><b>' . t('Items on page:') . '</b>&nbsp;</td><td>' . $item->nodecount . '</td></tr>';
    }
    $content .= '<tr><td align="right"><b>' . t('Owner:') . '</b>&nbsp;</td><td>' . mm_content_uid2name($item->uid) . '</td></tr>';
    $can = array();
    $avail_perms = array(
      MM_PERMS_WRITE => _mm_browser_choice($item,
        t('delete/edit page'),
        t('delete/edit user'),
        t('delete/edit group')),
      MM_PERMS_SUB => _mm_browser_choice($item,
        t('add sub-pages'),
        $item->name == MM_ENTRY_NAME_USERS ? t('add users') : t('add sub-pages'),
        t('add sub-groups')),
      MM_PERMS_APPLY => !empty($item->is_group) ? t('apply this group') : t('assign content to page'),
      MM_PERMS_READ => !empty($item->is_group) ? t('see group\'s members') : t('read contents of page'),
    );
    foreach ($avail_perms as $type => $desc) {
      if ($perms[$type]) {
        $can[] = $desc;
      }
    }
    $content .= '<tr valign="top"><td align="right"><b>' . t('You can:') . '</b>&nbsp;</td><td>' . implode('<br />', $can) . '</td></tr>';
    if ($users) {
      $content .= '<tr valign="top"><td align="right"><b>' . t('Users in group:') . '</b>&nbsp;</td><td>' . $users . '</td></tr>';
    }
    $content .= '</table>';
  }

  return $content;
}

function _mm_browser_user_can($perms) {
  $user_can = TRUE;
  if (drupal_strlen($_GET['browserSelectable'])) {
    $user_can = FALSE;
    foreach (str_split($_GET['browserSelectable']) as $check) {
      if ($perms[$check]) {
        $user_can = TRUE;
        break;
      }
    }
  }
  return $user_can;
}

function _mm_browser_get_fields() {
  $fields = array();
  if (function_exists('field_info_field_map')) {    // The faster version requires core >= 7.22
    foreach (field_info_field_map() as $field_name => $field) {
      if (in_array($field['type'], array('image', 'file', 'media')) && isset($field['bundles']['node'])) {
        $field_def = field_info_field($field_name);
        $temp = array_keys($field_def['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
        $fid_table = $temp[0];
        $fields[$fid_table] = $field_def['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$fid_table]['fid'];
      }
    }
  }
  else {
    foreach (field_info_fields() as $field_def) {
      if (in_array($field_def['type'], array('image', 'file', 'media')) && isset($field_def['bundles']['node'])) {
        $temp = array_keys($field_def['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
        $fid_table = $temp[0];
        $fields[$fid_table] = $field_def['storage']['details']['sql']['FIELD_LOAD_CURRENT'][$fid_table]['fid'];
      }
    }
  }
  return $fields;
}
