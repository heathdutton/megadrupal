<?php

/**
 * @file
 * Miscellaneous MM functions
 */

/**
 * Get a standard "Access Denied" message
 *
 * @param $whole_page
 *   (optional) If TRUE, this is the only content on the page, so show top-
 *   level navigation and send the 403 header.
 * @return string|int
 *   The message
 */
function mm_access_denied($whole_page = TRUE) {
  static $return;

  if ($whole_page) {
    drupal_add_http_header('Status', '403 Forbidden');
  }
  else if (!empty($return)) {
    return $return;
  }

  $path403 = '';
  // First, try the original 403 location, saved by the httpauth module
  if (module_exists('httpauth')) {
    $path403 = drupal_get_normal_path(variable_get('httpauth_site_403', ''));
  }

  // Failing that, read the Drupal one
  if (empty($path403)) {
    $path403 = variable_get('site_403', '');
    if (empty($path403)) return t('<h2>Password Required</h2>');
  }

  if (!function_exists('menu_set_active_item')) {
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  }

  $old_q = $_GET['q'];
  menu_set_active_item($path403);
  $page = menu_execute_active_handler($path403, FALSE);
  $return = drupal_render($page);
  if (!$whole_page) {
    menu_set_active_item($old_q);
  }
  return $return;
}

/**
 * Generate a node containing the standard "Access Denied" message
 *
 * @param $node
 *   The node object, whose teaser or body is modified to contain the message
 * @param $teaser
 *   TRUE if the node's teaser should contain the message, otherwise the body
 *   is set
 */
function mm_node_load_fail(&$node, $teaser = FALSE) {
  $node->content = array();
  if ($teaser) {
    $node->content['teaser']['#markup'] = mm_access_denied(FALSE);
    $node->content['body']['#markup'] = '';
  }
  else {
    $node->content['teaser']['#markup'] = '';
    $node->content['body']['#markup'] = mm_access_denied(FALSE);
  }
  $node->no_attribution = TRUE;
  $node->title = '';
}

/**
 * Retrieve a string of autocomplete suggestions for existing users
 */
function mm_autocomplete($want_username, $string = '', $misc = NULL) {
  $limit = 15;
  $min_string = 2;

  $matches = array();
  $too_short = array('' => t('Please type some more characters'));

  $string = trim($string);
  if (!empty($string)) {
    $result = NULL;
    $hook = mm_module_implements('mm_autocomplete_alter');
    if ($hook) {
      $function = $hook[0] . '_mm_autocomplete_alter';
      $result = $function($string, $limit, $min_string, $misc);
      if (empty($result)) {
        $matches = $too_short;
      }
    }
    elseif (drupal_strlen($string) >= $min_string) {
      // Consider Anonymous and Administrator first
      $startswith = $contains = '';
      for ($i = 0; $i <= 1; $i++) {
        $name = mm_content_uid2name($i);
        if (($pos = stristr($name, $string)) !== FALSE) {
          $stmt = "(SELECT $i AS uid, '' AS name, '' AS pref_fml, '' AS pref_lfm, '$name' AS lastname, '' AS firstname, '' AS middlename) UNION ";
          if (!$pos) $startswith .= $stmt;
          else $contains .= $stmt;
        }
      }
      $status_limit = user_access('administer all users') ? '' : ' status = 1 AND';
      $result = db_query(
        'SELECT * FROM (' .
          $startswith .
          $contains .
          "(SELECT uid, name, '' AS pref_fml, '' AS pref_lfm, '' AS lastname, '' AS firstname, '' AS middlename " .
            "FROM {users} WHERE$status_limit uid > 1 AND name = :name_exact " .
            'ORDER BY name) UNION ' .
          "(SELECT uid, name, '', '', '', '', '' " .
            "FROM {users} WHERE$status_limit uid > 1 AND name LIKE :name_start " .
            'ORDER BY name) UNION ' .
          "(SELECT uid, name, '', '', '', '', '' " .
            "FROM {users} WHERE$status_limit uid > 1 AND name LIKE :name_any " .
            'ORDER BY name)) x ' .
          'LIMIT ' . intval($limit + 1),
          array(
            ':name_exact' => $string,
            ':name_start' => $string . '%',
            ':name_any' => '%_' . $string . '%',
          )
        );
    }
    else {
      $matches = $too_short;
    }

    if (!empty($result)) {
      foreach ($result as $usr) {
        if (count($matches) == $limit) {
          $matches[''] = '...';
          break;
        }
        else {
          $name = check_plain(mm_content_uid2name($usr->uid, 'lfmu', $usr));
          if (!$want_username) $matches[$usr->uid . '-' . $name] = $name;
          elseif ($usr->name) $matches[$usr->name] = $name;
        }
      }
    }
  }

  drupal_add_http_header('Content-Type', 'text/plain');
  print drupal_json_encode($matches);
  $GLOBALS['devel_shutdown'] = FALSE;
  exit();
}

/**
 * Redirect the user to a URL, while checking for possible recursion
 *
 * @param $url
 *   URL to redirect to
 * @param $query
 *   Optional query fragment
 * @param $hash
 *   Optional anchor to appear in the URL after '#'
 */
function mm_goto($url, $query = '', $hash = NULL) {
  $u = $url;
  $dummy = array();
  mm_module_invoke_all_array('url_outbound_alter', array(&$u, &$dummy, $url));
  if ($url == $_GET['q'] || $u == $_GET['q'] || mm_home_path() . "/$u" == $_GET['q']) {
    watchdog('mm', 'Recursive redirect: page=%page',
        array('%page' => $_GET['q']), WATCHDOG_ERROR, l(t('view'), $_GET['q']));

    menu_set_active_item('');
    drupal_set_title(t('Error'));
    drupal_set_page_content(t('This page tried to send you into an endless loop. Please contact the administrator, and let him or her know how you got here. Press your browser\'s Back button to return to the page you came from.'));
    return;
  }
  drupal_goto($url, array('query' => $query, 'fragment' => $hash));
}

/**
 * Return an <img> tag, or URL to an image, containing a user's email address in
 * a form not easily grabbed by spambots
 *
 * @param $string
 *   The text to encode
 * @param $alt
 *   The alternate text portion of the <img> tag
 * @param $url_only
 *   If TRUE, return the URL, rather than the <img> tag
 * @return string
 *   Either the <img> tag or the URL
 */
function mm_text2image($string, $alt, $url_only = FALSE) {
  $md5 = md5($string . REQUEST_TIME);
  $lifetime = max(variable_get('cache_lifetime', 0), 60);
  // Intentionally use time() here instead of REQUEST_TIME, in case page
  // generation takes a while.
  cache_set("mm_text2image:$md5", $string, 'cache', time() + $lifetime);
  $url = "mm-txtimg/$md5/x.jpg";
  drupal_page_is_cacheable(FALSE);
  if ($url_only) return $url;
  return '<img ' . drupal_attributes(array('src' => url($url), 'alt' => $alt)) . '>';
}

/**
 * Print the actual image data containing a user's email address in a form not
 * easily grabbed by spambots. A given image may only be accessed once per
 * session.
 *
 * @param $md5
 *   If set, and there is a corresponding string set by mm_text2image(), output
 *   the image data
 * @return int
 *   MENU_NOT_FOUND, if an error occurs
 */
function mm_text2image_img($md5 = NULL) {
  if (isset($md5)) {
    $cache = cache_get("mm_text2image:$md5");
    if (empty($cache->data) || REQUEST_TIME >= $cache->expire) {
      return MENU_NOT_FOUND;
    }
    $string = $cache->data;
    $GLOBALS['devel_shutdown'] = TRUE;
    $captcha = drupal_get_path('module', 'monster_menus') . '/libraries/php-captcha';
    require_once("$captcha/php-captcha.inc.php");
    $fonts = array("$captcha/fonts/VeraBd.ttf", "$captcha/fonts/VeraMono.ttf", "$captcha/fonts/VeraIt.ttf");
    $images = array("$captcha/images/1.jpg", "$captcha/images/2.jpg", "$captcha/images/3.jpg");
    list($fonts, $images) = array($fonts, $images);
    $cap = new MMCaptcha($fonts);
    $cap->SetBackgroundImages($images);
    $cap->SetMinFontSize(10);
    $cap->SetMaxFontSize(10);
    $cap->Create($string);
    drupal_page_is_cacheable(FALSE);
  }
  else {
    return MENU_NOT_FOUND;
  }
}

/**
 * Parse GET parameters in URL
 *
 * @param &$mmtids
 *   Array to receive the list of tree IDs
 * @param &$oarg_list
 *   Optional array to receive the parameters following the tree IDs, un-parsed
 * @param &$this_mmtid
 *   Optional variable to receive the last tree ID (that of the current page)
 * @param $url
 *   URL to parse; defaults to the current page URL
 * @return string
 *   The first GET parameter, usually 'mm'
 */
function mm_parse_args(&$mmtids, &$oarg_list = NULL, &$this_mmtid = NULL, $url = NULL) {
  // Note: Don't try to cache this function, because doing so breaks redirects
  // after moving a page.
  if (is_null($url)) $url = $_GET['q'];

  $this_mmtid = NULL;
  $mmtids = explode('/', $url);
  $oarg_list = array();
  if (($out = array_shift($mmtids)) == 'mm') {       // skip 'mm'
    for ($i = 0; $i < count($mmtids); $i++) {
      $option = array();
      if ($i == count($mmtids) - 1) {
        // Include MM_GET_PARENTS, to speed up the call to
        // mm_content_get_parents() below.
        $option = MM_GET_PARENTS;
        // Handle the case of "mm/NNN?foo=bar"
        if (preg_match('{(^[\d\-]+)(\?.*)$}', $mmtids[$i], $matches)) {
          $mmtids[$i] = $matches[1];
          $mmtids[] = $matches[2];
        }
      }
      if (!is_numeric($mmtids[$i]) || !mm_content_get($mmtids[$i], $option)) {
        $oarg_list = array_splice($mmtids, $i);
        break;
      }
    }

    if (count($mmtids)) {
      $this_mmtid = $mmtids[count($mmtids) - 1];
      $parents = mm_content_get_parents($this_mmtid);
      array_shift($parents);    // skip root node
      array_splice($mmtids, 0, -1, $parents);  // insert parents
    }
  }
  else {
    $oarg_list = $mmtids;
    $mmtids = array();
  }
  return $out;
}

/**
 * Get the internal URI of the homepage
 *
 * @return string
 *   The URI
 */
function mm_home_path() {
  return 'mm/' . mm_home_mmtid();
}

/**
 * Get the MM Tree ID of the homepage
 *
 * @return int
 *   The MM Tree ID
 */
function mm_home_mmtid() {
  $list = variable_get('mm_home_mmtid', MM_HOME_MMTID_DEFAULT);
  if (!is_array($list)) {
    return $list;
  }
  $conf_path = conf_path();
  if (isset($list[$conf_path])) {
    return $list[$conf_path];
  }
  return MM_HOME_MMTID_DEFAULT;
}

/**
 * Determine if a given page is a homepage for this site or a multisite.
 *
 * @return bool
 *   TRUE if the page is a homepage.
 */
function mm_is_any_home_mmtid($mmtid) {
  $list = variable_get('mm_home_mmtid', array(MM_HOME_MMTID_DEFAULT));
  return is_array($list) ? in_array($mmtid, $list) : $mmtid == $list;
}

/**
 * Show a page, based on the GET parameters in the URL
 *
 * @return string
 *   HTML code for the page
 */
function mm_show_page() {
  mm_parse_args($mmtids, $oarg_list, $this_mmtid);
  if (!count($mmtids)) $mmtids = array($this_mmtid = mm_home_mmtid());
  $perms = mm_content_user_can($this_mmtid);

  $output = _mm_render_pages($mmtids, $oarg_list, $err);
  $links = array();

  if ($perms[MM_PERMS_IS_RECYCLED]) {
    if ($perms[MM_PERMS_IS_RECYCLE_BIN]) {
      $recyc_msg = t('The contents below are in the recycle bin.');
    }
    else {
      $recyc_msg = t('This page is in the recycle bin.');

      if (mm_content_recycle_enabled()) {
        $when = db_select('mm_recycle', 'r')
          ->fields('r', array('recycle_date'))
          ->condition('r.type', 'cat')
          ->condition('r.id', $this_mmtid)
          ->execute()->fetchField();
        $recyc_msg .= mm_content_get_recycle_autodel_time($when, NULL, $this_mmtid, ' ' . t('It'));
      }

      if ($perms[MM_PERMS_WRITE]) {
        if (count($mmtids) >= 2 && mm_content_user_can($mmtids[count($mmtids) - 2], MM_PERMS_IS_RECYCLE_BIN)) {
          $msg = user_access('delete permanently') ? 'You can restore or permanently delete it using the %settings tab.' : 'You can restore it using the %settings tab.';
          $recyc_msg .= ' ' . t($msg, array('%settings' => t('Settings')));
        }
        else {
          foreach (array_reverse($mmtids) as $t) {
            if (mm_content_user_can($t, MM_PERMS_IS_RECYCLE_BIN)) {
              if (!empty($last_t) && ($tree = mm_content_get($last_t)))
                $pg = l(mm_content_get_name($tree), mm_content_get_mmtid_url($last_t));
              break;
            }
            else $last_t = $t;
          }

          if (!empty($pg)) {
            $recyc_msg .= t('<p>This page cannot be restored by itself. You must restore the topmost parent page in the recycle bin, !page.</p>', array('!page' => $pg));
          }
        }
      }
    }
  }

  if (!$err) {
    if ($perms[MM_PERMS_IS_RECYCLED]) {
      drupal_set_message($recyc_msg);
      if ($perms[MM_PERMS_IS_RECYCLE_BIN]) {
        $output['bin'] = _mm_show_bin_contents($this_mmtid);
      }
    }
  }
  elseif ($err == 'no read') {
    $output = array(
      '#type' => 'item',
      '#markup' => $perms[MM_PERMS_IS_GROUP] ? t('You do not have permission to see the members of this group.') : mm_access_denied(),
    );
  }
  else {    // $err=='no content'
    if (!$perms[MM_PERMS_IS_GROUP] && !variable_get('mm_hide_empty_pages', FALSE)) {
      global $user;
      $list = array();
      $entry = mm_content_get($this_mmtid, array(MM_GET_FLAGS, MM_GET_PARENTS));
      if ($perms[MM_PERMS_IS_USER] && isset($entry->flags['user_home'])) {
        if ($entry->flags['user_home'] == $user->uid) {
          $list[0] = variable_get('mm_default_homepage', '');
        }
        if (empty($list[0])) {
          $list[0] = t('<h2>Welcome</h2><p>This is a personal homepage that has not been modified yet.</p>');
        }
      }
      elseif ($perms[MM_PERMS_IS_RECYCLE_BIN])
        $list[0] = ' ';
      else
        $list[0] = t('<p>This page does not yet have any content.</p>');

      $entry->perms = $perms;
      mm_module_invoke_all_array('mm_empty_page_alter', array($entry, &$list));
      $nada = join('', $list);
    }
    else $nada = ' ';

    if ($perms[MM_PERMS_APPLY] && !$perms[MM_PERMS_IS_RECYCLED] && !$perms[MM_PERMS_IS_GROUP]) {
      $link = "mm/$this_mmtid/contents/add";
      if (!$perms[MM_PERMS_IS_GROUP] && !variable_get('mm_hide_empty_pages', FALSE)) {
        $links[] = array(
          'title' => t('Add content'),
          'href' => $link,
        );
      }
      else {
        mm_goto($link);
        return;
      }
    }
    elseif ($perms[MM_PERMS_IS_GROUP]) {
      $users = mm_content_get_users_in_group($this_mmtid, NULL, FALSE, 100, TRUE);
      $output = array(array(
        '#theme' => 'html_tag',
        '#tag' => 'h2',
        '#attributes' => array(),
      ));

      if (!count($users)) $msg = t('There are no users in this group.');
      elseif (isset($users['']) && $users[''] == '...') {
        $msg = t('A partial list of users in this group:');
      }
      else {
        $msg = t('All users in this group:');
      }
      $output[0]['#value'] = $msg;

      $output[] = array(
        '#type' => 'item',
        '#markup' => join('<br />', $users),
      );
      return $output;
    }
    elseif ($perms[MM_PERMS_WRITE] || $perms[MM_PERMS_SUB])
      if (!$perms[MM_PERMS_IS_GROUP] && !variable_get('mm_hide_empty_pages', FALSE)) {
        if (!$perms[MM_PERMS_IS_RECYCLED])
          $nada .= t('<p>You do not have permission to add content, however you can use the %settings tab to make changes to the page itself.</p>', array('%settings' => t('Settings')));
      }
      else {
        mm_goto("mm/$this_mmtid/settings");
        return;
      }

    if ($perms[MM_PERMS_IS_RECYCLED]) {
      drupal_set_message($recyc_msg);
      if ($perms[MM_PERMS_IS_RECYCLE_BIN]) {
        $output['bin'] = _mm_show_bin_contents($this_mmtid);
      }
    }

    if (empty($output)) {
      $output['empty'] = array(
        '#type' => 'item',
        '#markup' => $nada,
      );
    }
  }   // $err=='no content'

  // If the no_index flag is set, include a noindex meta tag, asking nice
  // crawlers not to index the page.
  if (!isset($entry)) {
    $entry = mm_content_get($this_mmtid, MM_GET_FLAGS);
  }
  if (isset($entry->flags['no_index'])) {
    drupal_add_html_head(array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'robots',
        'content' => 'noindex',
      ),
    ), 'no_index');
  }

  if (isset($links)) {
    $output['links'] = array(
      '#theme' => 'links',
      '#links' => $links,
    );
  }

  return $output;
}

function _mm_show_bin_contents($mmtid) {
  $kid = mm_content_get(array('parent' => $mmtid), array(), 1);
  if (empty($kid)) {
    return '';
  }
  $node = _mm_node_subpglist_settings();
  $node->type = 'subpglist';
  node_object_prepare($node);
  $node->subpglist_hidden_title = '';
  $node->nid = NULL;
  $node->title = t('Pages in this recycle bin:');
  $node->no_attribution = TRUE;
  $node->comment = FALSE;
  return node_view($node);
}

function mm_show_group($this_mmtid) {
  drupal_add_library('monster_menus', 'dataTables');
  drupal_add_css(drupal_get_path('module', 'monster_menus') . '/css/mm.css');

  $body = '<table cellpadding="0" cellspacing="0" border="0" class="tablesorter" id="mm-user-datatable-members-display" width="100%"><thead><tr>';
  $headers = _mm_ui_userlist_get_headers();
  array_pop($headers);
  foreach ($headers as $value) {
    $class = empty($value) ? ' class="no-sort"' : '';
    $body .= '<th' . $class . '>' . $value . '</th>';
  }
  $col_def = '';
  for ($i = 0; $i < count($headers); $i++) {
    $col_def .= 'null, ';
  }
  $body .= '</tr></thead>';
  $body .= '<tbody><tr><td colspan="' . count($headers) . '" class="dataTables_empty">' . t('Loading data from server') . '</td></tr></tbody></table>';

  mm_static('show_group', TRUE, $this_mmtid, $col_def);
  print theme('mm_page_wrapper', array('title' => t('Group Members'), 'body' => $body, 'wrap_body_with_div' => FALSE));
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit();
}

/**
 * Return a list of node types for which a flag is TRUE in the
 * hook_mm_node_info()
 *
 * @param $option
 *   The flag to test. Should be one of the MM_NODE_INFO_* constants.
 * @param $item
 *   Tree object for which node info is being requested. May be NULL if no
 *   specific page is being requested.
 * @return array
 *   An array of node types
 */
function mm_get_node_info($option = NULL, $item = NULL) {
  $list = &drupal_static(__FUNCTION__);
  if (!is_array($list)) {
    $list = array();
    foreach (mm_module_invoke_all('mm_node_info', $item) as $node_type => $data) {
      foreach ($data as $field => $value) {
        if (is_array($value) ? array_sum($value) : intval($value)) {
          $list[$field][] = $node_type;
        }
      }
    }
    drupal_alter('mm_node_info', $list, $item);
  }
  if (empty($option)) return $list;
  return isset($list[$option]) ? $list[$option] : array();
}

/**
 * When the requested URL is not found, evaluate it and try to come up with some
 * possible guesses as to what the user mis-typed, or to where the intended page
 * has moved.
 *
 * This code is generally called from within a node containing this line:
 *    <?php print mm_get_detailed_404(); ?>
 * In order to use this line in a node, you need enable the core module "PHP
 * Filter" and choose the "PHP code" text format when editing the node. See
 * http://drupal.org/node/1365274 for more details.
 *
 * @param $fuzzy
 *   If TRUE, show a list of pages having aliases that are fuzzy matches for
 *   the URL provided. If your site has too many pages, this can prove very
 *   slow, so it can be disabled by passing FALSE.
 * @return string
 *   HTML code containing the possible guesses. If an empty string is returned,
 *   the calling code should generate a default "page not found" message.
 */
function mm_get_detailed_404($fuzzy = TRUE) {
  global $user;

  $max_results = 10;
  $skipped = FALSE;

  $bad_path = explode('/', request_path());
  if ($site_404 = variable_get('site_404', '')) {
    mm_module_invoke_all_array('url_inbound_alter', array(&$site_404, $site_404, NULL));
  }

  $new_path = '';
  mm_module_invoke_all_array('url_inbound_alter', array(&$new_path, implode('/', $bad_path), NULL));
  if ($new_path == $site_404) return '';

  // Set the custom_theme to that of the 404 page, instead of the original page
  menu_set_custom_theme();

  $home_mmtid = mm_home_mmtid();
  $new_path = explode('/', $new_path);
  if ($new_path[0] == 'mm') {
    $found_mmtid = $new_path[1];
    $new_path = array_slice($new_path, 2);
  }
  else {
    $found_mmtid = $home_mmtid;
  }

  $mtime = 0;
  $output_list = array();
  $prefix_single = t('<p>The page you are looking for appears to have moved to:') . ' ';
  while ($child = array_shift($new_path)) {
    $select = db_select('mm_tree_revisions', 'r');
    $select->leftJoin('mm_tree', 't', 't.mmtid = r.mmtid');
    $num_revs = $select->condition('r.parent', $found_mmtid)
      ->condition('r.alias', $child)
      ->isNull('t.mmtid')
      ->countQuery()->execute()->fetchField();

    if ($num_revs) {
      $prefix_single = t('<p>The page you requested has been permanently deleted.');
      if (!count($output_list) && $found_mmtid != $home_mmtid) {
        $output_list[0] = $found_mmtid;
      }
      if (count($output_list)) {
        $prefix_single .= ' ' . t('You may be able to find what you were looking for here:') . ' ';
      }
      else {
        return $prefix_single;
      }
    }
    else {
      $mtime_query = db_select('mm_tree_revisions', 'r')
        ->fields('r', array('mmtid', 'mtime'))
        ->condition('r.parent', $found_mmtid)
        ->condition('r.alias', $child)
        ->where('LEFT(name, 1) <> :prefix', array(':prefix' => '.'))
        ->orderBy('r.vid', 'DESC');
      if ($mtime > 0)
        $mtime_query->condition('r.mtime', $mtime, '<=');

      if ($row = $mtime_query->execute()->fetchObject()) {
        if (mm_content_user_can($row->mmtid, MM_PERMS_READ)) {
          $output_list[0] = $found_mmtid = $row->mmtid;
          $mtime = $row->mtime;
          continue;
        }
        else {
          $skipped = TRUE;
        }
      }
      $output_list = array();
      // Use the SQL SOUNDEX() function instead of the PHP soundex() version,
      // because they produce different values
      $soundex = $fuzzy ? _mm_soundex($child) : '';
      $soundex_short = substr($soundex, 1, 3);
      $queries = array(
        // hook_menu() paths
        'hook' => array("SELECT * FROM {menu_router} WHERE SUBSTR(path, 1, 4) <> 'mm/%' AND (SUBSTRING_INDEX(path, '/', 1) = :child OR SOUNDEX(SUBSTRING_INDEX(path, '/', 1)) = :soundex) ORDER BY fit DESC", array(':child' => $child, ':soundex' => $soundex)),
        // Direct alias match at the correct level of the tree
        'p+a+' => array("t.parent = :found_mmtid AND t.alias = :child", array(':found_mmtid' => $found_mmtid, ':child' => $child)),
        // Alias starts with the string, at the correct level
        'p+a*' => array("t.parent = :found_mmtid AND t.alias LIKE :child", array(':found_mmtid' => $found_mmtid, ':child' => $child . '%')),
        // Alias sounds like the string, at the correct level
        'p+as' => array("t.parent = :found_mmtid AND SOUNDEX(t.alias) = :soundex", array(':found_mmtid' => $found_mmtid, ':soundex' => $soundex)),
        // Alias matches at any level
        'p-a+' => array("t.alias = :child", array(':child' => $child)),
        // Alias sounds like the string, at any level
        'p-as' => array("t.parent <> :found_mmtid AND SOUNDEX(t.alias) = :soundex", array(':found_mmtid' => $found_mmtid, ':soundex' => $soundex)),
        // Alias sounds like the string, at any level, using minimal match
        'p<as' => array("t.parent <> :found_mmtid AND SUBSTR(SOUNDEX(t.alias), 2, 3) = :soundex_short", array(':found_mmtid' => $found_mmtid, ':soundex_short' => $soundex_short)),
      );
      if (!$fuzzy) {
        $queries['hook'] = array("SELECT * FROM {menu_router} WHERE SUBSTR(path, 1, 4) <> 'mm/%' AND SUBSTRING_INDEX(path, '/', 1) = :child ORDER BY fit DESC", array(':child' => $child));
        unset($queries['p+as']);
        unset($queries['p-as']);
        unset($queries['p<as']);
      }
      $soundex_bad = FALSE;
      foreach ($queries as $index => $params) {
        if ($index == 'p+as') {
          // See if there is enough variance in the soundex value, by checking
          // the number of consonants and the frequency of resulting digits.
          $len_test = preg_replace('/[^bcdfghjklmnpqrstvwxyz]/i', '', $child);
          if (strlen($len_test) < 3 || strlen(count_chars($soundex, 3)) / strlen($soundex) <= 2/3) {
            $soundex_bad = TRUE;
            continue;
          }
        }
        elseif ($index == 'p-a+') {
          if (count($output_list) == 1) {
            $found_mmtid = $output_list[0];
            break;
          }
        }
        elseif ($index == 'p-as' || $index == 'p<as') {
          // This is the last-ditch effort, only if everything before has
          // failed, and the soundex value is sufficiently unique
          if (count($output_list) || $soundex_bad) break;
          $max_results = 5;
        }
        elseif ($index == 'hook') {
          // Search in the hook_menu() list for a close match, using just the
          // first element of the path. Only do this when MM found no match at
          // all.
          if ($found_mmtid != $home_mmtid) continue;

          $test_path = $new_path;
          array_unshift($test_path, $child);
          $results = db_query($params[0], $params[1]);
          foreach ($results as $menu) {
            $matched = TRUE;
            $match = array();
            foreach (explode('/', $menu->path) as $path_index => $elem) {
              if ($path_index >= count($test_path)) {
                $matched = FALSE;
                break;
              }

              if ($elem == '%' || strcasecmp($test_path[$path_index], $elem) == 0) {
                $match[] = $test_path[$path_index];
              }
              elseif ($fuzzy && _mm_soundex($test_path[$path_index]) == _mm_soundex($elem)) {
                $match[] = $elem;
              }
              else {
                $matched = FALSE;
                break;
              }
            }

            if ($matched) {
              $item = (array)$menu;
              _menu_translate($item, $match, TRUE);
              if ($item['access']) {
                $output_list[] = implode('/', $match);
              }
            }
          }

          if (count($output_list)) {
            // Stop looking.
            $new_path = array();
          }
          continue;   // Go to next query.
        }

        $where = $params[0];
        $params = $params[1];
        // There's no need to do matches against mm_tree, since its data is
        // duplicated in mm_tree_revisions in the most recent revision.
        // Don't match items or parents with a name starting with '.'.
        $query = "SELECT DISTINCT t.mmtid FROM {mm_tree_revisions} t WHERE $where AND LEFT(t.name, 1) <> '.' AND (SELECT COUNT(*) FROM {mm_tree} t2 INNER JOIN {mm_tree_parents} p ON p.parent = t2.mmtid WHERE p.mmtid = t.mmtid AND LEFT(t2.name, 1) = '.') = 0";

        // Skip mmtids already found
        if (count($output_list)) {
          $query .= ' AND t.mmtid NOT IN (:output_list)';
          $params[':output_list'] = $output_list;
        }

        $results = db_query($query . ' ORDER BY t.vid DESC', $params);
        while (count($output_list) < $max_results && ($item = $results->fetchObject()))
          if (mm_content_user_can($item->mmtid, MM_PERMS_READ)) {
            $output_list[] = $item->mmtid;
          }
          else {
            $skipped = TRUE;
          }

        if (count($output_list) >= $max_results) break;
      }

      if (!$output_list) {
        if ($found_mmtid == $home_mmtid) return '';
        $output_list[0] = $found_mmtid;
        $prefix_single = t('<p>The page you are looking for was not found, but you might be able to find it here:') . ' ';
        break;
      }

      $prefix_single = t('<p>This page might be what you are looking for:') . ' ';

      if (count($output_list) != 1) break;
    }
  }

  if (count($output_list) == 1 && is_numeric($output_list[0]) && mm_content_is_recycled($output_list[0])) {
    // We'll only get here if the page is readable by the user
    return t('<p>The page you requested has been marked for future deletion. Therefore, it is no longer accessible.</p>');
  }

  $options = array();
  foreach ($output_list as $index => $elem) {
    if (!is_numeric($elem)) {
      $path = $elem;
    }
    else {
      $path = $path0 = 'mm/' . $elem;
      mm_module_invoke_all_array('url_outbound_alter', array(&$path, &$options, NULL));
      if ($path == $path0) {
        unset($output_list[$index]);
        continue;
      }
    }
    $output_list[$index] = l(base_path() . $path, $path, array('attributes' => array('rel' => 'nofollow')));
  }

  $post = !$user->uid && $skipped ? t('<p>You might get more results if you log-in.</p>') : '';
  if (count($output_list) == 1) {
    return $prefix_single . $output_list[0] . '</p>' . $post;
  }

  if (count($output_list) > 0) {
    $out = array(
      '#theme' => 'item_list',
      '#title' => t('<p>One of these pages might be what you are looking for:'),
      '#items' => $output_list,
      '#suffix' => $post,
    );
    return drupal_render($out);
  }

  return !$user->uid && $skipped ? t('This page may be able to provide you with suggestions as to where to find what you are looking for, but you need to log-in first.') : '';
}

/**
 * Redirect the user to an MM URL based on a Drupal node
 *
 * @param $nid
 *   Drupal node ID to redirect to
 * @param $add
 *   Path elements to add after the last MM tree ID. If used, must start with '/'.
 * @param $hash
 *   Optional anchor to appear in the URL after '#'
 */
function mm_redirect_to_node($nid, $add = NULL, $hash = NULL) {
  $mmtids = mm_content_get_by_nid($nid);
  if (!empty($mmtids)) {
    mm_redirect_to_mmtid($mmtids[0], $add, $hash);
  }
}

/**
 * Redirect the user to an MM URL based on an MM tree ID
 *
 * @param $mmtid
 *   Tree ID to redirect to
 * @param $add
 *   Path elements to add after the last MM tree ID. If used, must start with '/'.
 * @param $hash
 *   Optional anchor to appear in the URL after '#'
 */
function mm_redirect_to_mmtid($mmtid, $add = NULL, $hash = NULL) {
  mm_goto("mm/$mmtid$add", NULL, $hash);
}

/**
 * Add MM group-based roles to user objects being loaded
 */
function mm_set_user_roles(&$account) {
  global $user;

  // The query below assumes that uid=0 will never be checked
  if (!$account->uid) return;

  $added_role = FALSE;

  $inner = db_select('mm_role2group', 'r2');
  $inner->join('role', 'r', 'r.rid = r2.rid');
  $inner->join('mm_group', 'g', 'g.gid = r2.gid');
  $inner->leftJoin('mm_virtual_group', 'v', 'g.vgid = v.vgid');
  $inner->addField('r2', 'rid');
  $inner->addField('r', 'name');
  $inner->addExpression('(COALESCE(SUM(v.uid = :uid1 OR g.vgid = 0 AND g.uid = :uid2), 0) > 0) <> r2.negative', 'ok', array(':uid1' => $account->uid, ':uid2' => $account->uid));
  $inner->groupBy('r2.rid');
  $select = db_select($inner, 's')
    ->fields('s', array('rid', 'name'))
    ->condition('s.ok', 1);
  $result = $select->execute();
  foreach ($result as $r) {
    if (!isset($account->roles[$r->rid])) {
      $account->roles[$r->rid] = $r->name;
      $added_role = TRUE;
    }
  }

  if ($account->uid == $user->uid && $added_role) {
    $user->roles = $account->roles;
    if (function_exists('user_access')) {
      // Clear out the user_access cache
      user_access('', NULL, TRUE);
    }
  }
}

/**
 * Retry a query when it deadlocks.
 *
 * It turns out to be fairly common for MySQL to deadlock when more than one
 * process is updating the same table at the same time. According to the
 * documentation, the proper way to handle this is to retry the query.
 *
 * @param $query
 *   A query object, from db_select(), db_delete(), etc. or a query string.
 * @param $args
 *   An array of values to substitute into the query, when $query is a string.
 * @param $options
 *   An array of options to control how the query operates, when $query is a
 *   string.
 * @return DatabaseStatementInterface
 *   Any results from the query.
 * @throws PDOException
 *   Any exception other than the deadlock
 */
function mm_retry_query($query, array $args = array(), array $options = array()) {
  $tries = 0;
  for (;;) {
    try {
      if (is_object($query)) {
        return $query->execute();
      }
      return db_query($query, $args, $options);
    }
    catch (PDOException $e) {
      if (++$tries < 70 && strpos($e->getMessage(), 'Deadlock found when trying to get lock') !== FALSE) {
        // Sleep for .1 sec. at first, then .5 sec after the first 10 tries
        // (31 sec. total).
        usleep($tries <= 10 ? 1000000 : 5000000);
      }
      else {
        throw $e;
      }
    }
  }
}

/**
 * Update the table containing all results of virtual group queries.
 */
function mm_regenerate_vgroup() {
  // Intentionally don't use REQUEST_TIME here, because several minutes may have
  // elapsed between when it was set and the time this code is run.
  $now = time();
  if ($running = variable_get('mm_vgroup_regen_semaphore', 0)) {
    if ($now - $running < 24 * 60 * 60) {
      watchdog('mm', 'mm_regenerate_vgroup() is already running.', array(), WATCHDOG_ERROR);
      return t('mm_regenerate_vgroup() is already running.');
    }
    watchdog('mm', 'mm_regenerate_vgroup() has been running for more than a day, or terminated unexpectedly. Ignoring semaphore.', array(), WATCHDOG_ERROR);
  }
  variable_set('mm_vgroup_regen_semaphore', $now);

  // Delete entries for groups that no longer exist. Unfortunately, we have to
  // do this as two separate queries, because there's no way to do a multi-
  // table delete in Drupal, and MySQL can't do a sub-select during DELETE that
  // refers to the table being deleted from.
  $txn = db_transaction();
  try {
    // SELECT g.vgid FROM {mm_virtual_group} g
    //   LEFT JOIN {mm_vgroup_query} q ON g.vgid = q.vgid
    //   WHERE q.vgid IS NULL
    $inner = db_select('mm_virtual_group', 'g');
    $inner->addField('g', 'vgid');
    $inner->addJoin('LEFT OUTER', 'mm_vgroup_query', 'q', 'g.vgid = q.vgid');
    $inner->isNull('q.vgid');
    if ($vgids = $inner->execute()->fetchCol()) {
      // DELETE FROM mm_virtual_group WHERE vgid IN (:vgids)
      mm_retry_query(db_delete('mm_virtual_group')
        ->condition('vgid', $vgids, 'IN'));
    }
  }
  catch (Exception $e) {
    $txn->rollback();
    variable_del('mm_vgroup_regen_semaphore');
    // Repeat the exception, but with the location below.
    throw new Exception($e->getMessage());
  }
  // Commit.
  unset($txn);

  $vgids = $email_errors = array();
  // Split the queries up into chunks of mm_vgroup_regen_chunk, so that the SQL
  // buffer size isn't exceeded.
  $chunksize = variable_get('mm_vgroup_regen_chunk', 15);
  $chunks_per_run = variable_get('mm_vgroup_regen_chunks_per_run', 50);
  for ($chunk = 0; $chunk < $chunks_per_run; $chunk++) {
    $i = $chunksize * $chunk;
    $list = array();
    // dirty==MM_VGROUP_DIRTY_FAILED means there was a previous sanity error, so
    // ignore it now.
    // SELECT vg.*, g.gid FROM {mm_vgroup_query} q
    //   INNER JOIN {mm_group} g ON g.vgid = q.vgid
    //   WHERE q.dirty IN(MM_VGROUP_DIRTY_NEXT_CRON, MM_VGROUP_DIRTY_REDO)
    // LIMIT $i, $chunksize
    $query = db_select('mm_vgroup_query', 'q');
    $query->join('mm_group', 'g', 'g.vgid = q.vgid');
    $result = mm_retry_query($query->fields('q')
      ->fields('g', array('gid'))
      ->condition('q.dirty', array(MM_VGROUP_DIRTY_NEXT_CRON, MM_VGROUP_DIRTY_REDO))
      ->range($i, $chunksize));
    $nrows = 0;
    $chunk_vgids = array();
    foreach ($result as $r) {
      $qfrom = module_exists('token') ?
        token_replace($r->qfrom, array('mm_tree' => mm_content_get($r->gid, MM_GET_FLAGS)), array('clear' => TRUE, 'sanitize' => FALSE)) :
        $r->qfrom;
      $list[] = "(SELECT $r->vgid, $r->field $qfrom)";
      $vgids[] = $r->vgid;
      $chunk_vgids[] = $r->vgid;
      $nrows++;
    }

    if (!$nrows) break;

    if (!isset($created)) {
      // We can't use a real temporary table because MySQL has a limitation
      // which prevents temp tables from appearing twice within the same query.
      try {
        db_drop_table('mm_virtual_group_temp');
        db_create_table('mm_virtual_group_temp', drupal_get_schema('mm_virtual_group'));
      }
      catch (DatabaseSchemaObjectExistsException $e) {
        watchdog('mm', 'Could not create table mm_virtual_group_temp',
            array(), WATCHDOG_ERROR);
        variable_del('mm_vgroup_regen_semaphore');
        return;
      }

      $created = TRUE;
    }

    // Unfortunately, there's no way to do this using db_select() and db_insert().
    db_query('INSERT INTO {mm_virtual_group_temp} (vgid, uid) ' . join(' UNION ', $list));
    // Delete uids that no longer exist in users table
    // DELETE FROM {mm_virtual_group_temp} WHERE
    //   (SELECT COUNT(*) = 0 FROM {users} WHERE uid = mm_virtual_group_temp.uid)
    $count = db_select('users');
    $count->addExpression('COUNT(*) = 0');   // countQuery() won't work here
    $count->where('uid = {mm_virtual_group_temp}.uid');
    db_delete('mm_virtual_group_temp')
      ->condition($count)
      ->execute();

    $result = mm_retry_query('SELECT vgid1 AS vgid, gid, orig_count, IFNULL(temp_count, 0) AS temp_count ' .
      'FROM (' .
        'SELECT * FROM (' .
          'SELECT vgid AS vgid1, COUNT(*) AS orig_count ' .
            'FROM {mm_virtual_group} ' .
            'WHERE vgid IN(:vgids1) ' .
            'GROUP BY vgid) ' .
          'AS t1 ' .
        'LEFT JOIN (' .
          'SELECT vgid AS vgid2, COUNT(*) AS temp_count ' .
            'FROM {mm_virtual_group_temp} ' .
            'WHERE vgid IN(:vgids2) ' .
            'GROUP BY vgid) ' .
          'AS t2 ' .
        'ON vgid2 = vgid1 ' .
        'WHERE IFNULL(temp_count, 0) < orig_count AND (orig_count - IFNULL(temp_count, 0)) / orig_count > :sanity AND (temp_count IS NULL OR temp_count >= 10)) ' .
      'AS insane ' .
      'INNER JOIN {mm_vgroup_query} vg ON vg.vgid = insane.vgid1 ' .
      'LEFT JOIN {mm_group} g ON g.vgid = insane.vgid1 ' .
      'WHERE vg.dirty <> :dirty',
      array(
        ':vgids1' => $chunk_vgids,
        ':vgids2' => $chunk_vgids,
        ':sanity' => MM_VGROUP_COUNT_SANITY,
        ':dirty' => MM_VGROUP_DIRTY_REDO,
      )
    );
    foreach ($result as $r) {
      $tree = mm_content_get($r->gid);
      $msg = t('The size of the virtual group with vgid=!vgid, gid=!gid, name=!name went down by more than !pct%. It went from !orig to !new users. To ignore this condition and regenerate the virtual group anyway, set its "dirty" field to !redo.', array('!vgid' => $r->vgid, '!gid' => $r->gid, '!name' => $tree->name, '!pct' => MM_VGROUP_COUNT_SANITY*100, '!orig' => $r->orig_count, '!new' => $r->temp_count, '!redo' => MM_VGROUP_DIRTY_REDO));
      $email_errors[$r->vgid] = $msg;
      watchdog('mm', $msg, array(), WATCHDOG_ERROR);
      // Set dirty to MM_VGROUP_DIRTY_FAILED so that the same error is not
      // logged repeatedly
      mm_retry_query(db_update('mm_vgroup_query')
        ->fields(array('dirty' => MM_VGROUP_DIRTY_FAILED))
        ->condition('vgid', $r->vgid));
      // Don't copy data from temp to real table for this vgid
      $vgid_index = array_search($r->vgid, $vgids);
      if ($vgid_index !== FALSE) {
        array_splice($vgids, $vgid_index, 1);
      }
    }

    if ($nrows < $chunksize) break;
  }

  if ($vgids) {
    // The anonymous user can never be in any groups. Also delete any data for
    // vgroups that are insane.
    // DELETE FROM {mm_virtual_group_temp} WHERE uid = 0 OR vgid NOT IN (:vgids)
    db_delete('mm_virtual_group_temp')
      ->condition(db_or()->condition('uid', 0)->condition('vgid', $vgids, 'NOT IN'))
      ->execute();

    // Update the preview column for mm_content_get_users_in_group().
    $result = mm_retry_query(db_select('mm_virtual_group_temp', 'v')
      ->fields('v', array('vgid'))
      ->groupBy('v.vgid'));
    foreach ($result as $r) {
      db_query('SET @i=0');
      $query =
        'UPDATE {mm_virtual_group_temp} t ' .
        'INNER JOIN (' .
          'SELECT v.uid, (@i:=@i+1) AS ind FROM {mm_virtual_group_temp} v ' .
            'INNER JOIN {users} u ON v.vgid = :vgid1 AND u.uid = v.uid ' .
          'ORDER BY u.name' .
        ') AS j ' .
        'ON j.uid = t.uid AND t.vgid = :vgid2 SET preview = IF(j.ind <= 32767, j.ind, NULL)';
      mm_module_invoke_all_array('mm_regenerate_vgroup_preview_alter', array(&$query));
      db_query($query, array(':vgid1' => $r->vgid, ':vgid2' => $r->vgid));
    }

    // Start a transaction
    $txn = db_transaction();

    try {
      // DELETE FROM {mm_virtual_group} WHERE vgid IN (:vgids)
      mm_retry_query(db_delete('mm_virtual_group')
        ->condition('vgid', $vgids));

      // INSERT INTO {mm_virtual_group}
      //   (SELECT t.vgid, t.uid, t.preview FROM {mm_virtual_group_temp} t)
      $select = db_select('mm_virtual_group_temp', 't');
      $select->fields('t', array('vgid', 'uid', 'preview'));
      mm_retry_query(db_insert('mm_virtual_group')
        ->from($select));

      // UPDATE {mm_vgroup_query} SET dirty = <MM_VGROUP_DIRTY_NOT>
      //   WHERE vgid IN(:vgids)
      mm_retry_query(db_update('mm_vgroup_query')
        ->fields(array('dirty' => MM_VGROUP_DIRTY_NOT))
        ->condition('vgid', $vgids));
    }
    catch (Exception $e) {
      $txn->rollback();
      watchdog_exception('mm', $e);
      variable_del('mm_vgroup_regen_semaphore');
      // Repeat the exception, but with the location below.
      throw new Exception($e->getMessage());
    }

    // Commit.
    unset($txn);
  }

  if ($email_errors) {
    $to = variable_get('mm_vgroup_errors_email', '');
    if (empty($to)) {
      $to = variable_get('site_mail', ini_get('sendmail_from'));
    }

    $params = array(
      'errors' => $email_errors,
      'in' => join(',', array_keys($email_errors)),
    );
    drupal_mail('monster_menus', 'mm_regenerate_vgroup', $to, language_default(), $params);
  }

  if (isset($created)) db_drop_table('mm_virtual_group_temp');
  db_query('OPTIMIZE TABLE {mm_virtual_group}');

  variable_del('mm_vgroup_regen_semaphore');
  return t('Virtual groups have been regenerated.');
}

/**
 * Mark the pre-defined group "All logged-in users" dirty, so it will be rebuilt
 * during the next cron run.
 */
function mm_mark_all_logged_in_vgroup_dirty() {
  static $done;

  // Only do this once per page load, to avoid excessive updates if multiple
  // users are inserted/deleted.
  if (empty($done)) {
    db_update('mm_vgroup_query')
      ->fields(array('dirty' => 1))
      ->condition('qfrom', 'FROM {users} WHERE uid > 0')
      ->execute();
    $done = TRUE;
  }
}

function mm_render_nodes_on_page($mmtid, $per_page) {
  $item = mm_content_get($mmtid, MM_GET_ARCHIVE);
  $perms = mm_content_user_can($mmtid);
  $no_read = $ok = 0;
  $output = array();
  // set $_GET['page'] to control the page number
  if (_mm_render_nodes_on_page($item, $perms, (int)$per_page, array(), FALSE, $output, $ok, $no_read, $pager_elem, $archive_tree, $archive_date_int, $rss_link)) {
    print drupal_render($output);
  }
  $GLOBALS['devel_shutdown'] = FALSE; // prevent the devel module from outputting
  exit;
}

/**
 * Store some data to be added at a future point to the footer region of the page.
 *
 * @param $content
 *   The content to add; a render array is preferred
 * @return array
 *   An array containing all data added so far
 */
function mm_add_page_footer($content = NULL) {
  static $data = array();

  if (!empty($content)) {
    if (is_string($content)) $content = array('#type' => 'markup', '#markup' => $content);
    $data[] = $content;
  }
  return $data;
}

/**
 * Return JSON response for user list request.
 */
function mm_large_group_get_users_json($mmtid, $element, $form_token = '') {
  $clean_element = str_replace('-', '_', $element);
  $table_name = ($clean_element == 'mm_user_datatable_members_display' ? 'mm_group' : 'mm_group_temp');
  $users_array = mm_module_invoke_all_array('mm_large_group_get_users', array('mmtid' => $mmtid, 'element' => $clean_element, 'form_token' => $form_token));
  if (empty($users_array)) {
    $query = db_select('users', 'u')
      ->fields('u', array('uid', 'name'));
    if ($table_name == 'mm_group_temp') {
      $query->join('mm_group_temp', 'm', 'u.uid = m.uid');
      $query->condition('m.gid', $mmtid)
        ->condition('m.sessionid', session_id())
        ->condition('m.token', $form_token);
    }
    else {
      $query_virtual_group = db_select('mm_group', 'm')
        ->fields('m', array('vgid'));
      $query_virtual_group->condition('m.gid', $mmtid)
        ->condition('m.vgid', 0, '<>');
      $query_virtual_group->groupBy('m.gid');
      $results = $query_virtual_group->execute();
      if ($results->rowCount() > 0) {
        $vgid = $results->fetchField();
        $query->join('mm_virtual_group', 'm', 'u.uid = m.uid');
        $query->condition('m.vgid', $vgid);
      }
      else {
        $query->join('mm_group', 'm', 'u.uid = m.uid');
        $query->condition('m.gid', $mmtid);
      }
    }
    $sort_array = array('name');
    $sort_list = array();
    $unfiltered_query = $query;
    if (!empty($_GET['sSearch'])) {
      $query->condition('u.name', '%' . $_GET['sSearch'] . '%', 'LIKE');
    }
    if (isset($_GET['iSortCol_0'])) {
      if (is_numeric($_GET['iSortingCols'])) {
        for ($i = 0; $i < $_GET['iSortingCols']; $i++) {
          $direction = isset($_GET['sSortDir_' . $i]) && $_GET['sSortDir_' . $i] == 'desc' ? 'DESC' : 'ASC';
          if (is_numeric($_GET['iSortCol_' . $i]) && isset($sort_list)) {
            $query->orderBy($sort_array[(int)$_GET['iSortCol_' . $i]], $direction);
          }
        }
      }
    }

    $total_unfiltered_rows = $unfiltered_query->countQuery()->execute()->fetchField();
    $total_rows = $query->countQuery()->execute()->fetchField();
    if (isset($_GET['iDisplayStart'])) {
      $query->range($_GET['iDisplayStart'], $_GET['iDisplayLength']);
    }
    $results = $query->execute();

    $users = array(
      'sEcho' => intval($_GET['sEcho']),
      'iTotalRecords' => $total_rows,
      'iTotalDisplayRecords' => $total_unfiltered_rows,
      'aaData' => array(),
    );
    foreach ($results as $item) {
      if ($clean_element == 'mm_user_datatable_members_display') {
        $users['aaData'][] = array($item->name);
      }
      elseif ($clean_element == 'members') {
        $users['aaData'][] = array($item->name, '<a href="Javascript:Drupal.mmGroupRemoveUser(' . $item->uid . ',\'' . $clean_element . '\')">' . t('Delete') . '</a>');
      }
    }
  }
  else {
    $users = $users_array;
  }

  drupal_add_http_header('Pragma', 'no-cache');
  drupal_json_output($users);
  exit();
}

/**
 * Delete a user from the editing form temporary table.
 *
 * @param $mmtid
 *   The mmtid of the group from which the user is being removed
 * @param $token
 *   The token associated with the form from which the request originates
 * @param $delete_user
 *   User object of the user to be deleted
 */
function mm_large_group_delete_user($mmtid, $token, $delete_user) {
  db_delete('mm_group_temp')
    ->condition('gid', $mmtid)
    ->condition('uid', $delete_user->uid)
    ->condition('sessionid', session_id())
    ->condition('token', $token)
    ->execute();
}

/**
 * Add one or more users to the editing form temporary table.
 *
 * @param $mmtid
 *   The mmtid of the group to which the user is being added
 * @param $token
 *   The token associated with the form from which the request originates
 * @param $uids
 *   A comma-separated list of uids to be added
 */
function mm_large_group_add_users($mmtid, $token, $uids) {
  foreach (explode(',', $uids) as $uid) {
    if (!empty($uid) && user_load($uid)) {
      db_merge('mm_group_temp')
        ->key(array(
          'gid' => $mmtid,
          'uid' => $uid,
          'sessionid' => session_id(),
          'token' => $token,
        ))
        ->execute();
    }
  }
}

/**
 * Redirects comment links to the correct page depending on comment settings.
 *
 * Since comments are paged there is no way to guarantee which page a comment
 * appears on. Comment paging and threading settings may be changed at any time.
 * With threaded comments, an individual comment may move between pages as
 * comments can be added either before or after it in the overall discussion.
 * Therefore we use a central routing function for comment links, which
 * calculates the page number based on current comment settings and returns
 * the full comment view with the pager set dynamically.
 *
 * @param $cid
 *   A comment identifier.
 * @param $mmtid
 *   The mmtid of the page containing the node with the given comment.
 * @return int|mixed
 *   The comment listing set to the page on which the comment appears.
 * @see comment_permalink
 */
function mm_comment_permalink($cid, $mmtid) {
  if (($comment = comment_load($cid)) && ($node = node_load($comment->nid))) {

    // Find the current display page for this comment.
    $page = comment_get_display_page($comment->cid, $node->type);

    // Set $_GET['q'] and $_GET['page'] ourselves so that the node callback
    // behaves as it would when visiting the page directly.
    $_GET['q'] = 'node/' . $node->nid;
    $_GET['page'] = $page;

    // Return the node view, this will show the correct comment in context.
    return menu_execute_active_handler("mm/$mmtid/node/" . $node->nid, FALSE);
  }
  drupal_not_found();
}

/**
 * @defgroup mm_hooks Monster Menus Hooks
 * @{
 * Allow modules to interact with Monster Menus.
 */

/**
 * Get a list of all enabled modules and MM sub-modules that implement a hook.
 *
 * @param $hook
 *   The name of the hook to query
 * @return array
 *   An array of module names
 */
function mm_module_implements($hook) {
  static $cache = array();

  if (isset($cache[$hook])) {
    return $cache[$hook];
  }

  $list = module_implements($hook);
  foreach (monster_menus_node_info() as $desc) {
    if (function_exists($desc['base'] . '_' . $hook)) {
      $list[] = $desc['base'];
    }
  }

  if (drupal_bootstrap(NULL, FALSE) == DRUPAL_BOOTSTRAP_FULL) {
    $cache[$hook] = $list;
  }
  return $list;
}

/**
 * Invoke a hook in all enabled modules and MM sub-modules that implement it.
 *
 * @param $hook
 *   The name of the hook to invoke.
 * @param ...
 *   Arguments to pass to the hook.
 * @return array
 *   An array of return values of the hook implementations. If modules return
 *   arrays from their implementations, those are merged into one array.
 */
function mm_module_invoke_all() {
  $args = func_get_args();
  $hook = array_shift($args);
  $return = array();
  foreach (mm_module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    $result = call_user_func_array($function, $args);
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
 * Invoke a hook in all enabled modules and MM sub-modules that implement it.
 * Unlike mm_module_invoke_all(), any references are preserved.
 *
 * @param $hook
 *   The name of the hook to invoke.
 * @param $args
 *   Arguments to pass to the hook. Any references in the array are preserved.
 * @return array
 *   An array of return values of the hook implementations. If modules return
 *   arrays from their implementations, those are merged into one array.
 */
function mm_module_invoke_all_array($hook, $args) {
  $return = array();
  foreach (mm_module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    $result = call_user_func_array($function, $args);
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
 * @} End of "defgroup mm_hooks".
 */

// ****************************************************************************
// * Private functions start here
// ****************************************************************************

function _mm_render_pages($mmtids, $oarg_list, &$err, $no_attribution = FALSE, $allow_rss = TRUE, $block_id = 0) {
  $_mm_page_subscribe_item = &drupal_static('_mm_page_subscribe_item');

  $err = '';
  $output = array();
  $no_read = $ok = 0;
  $this_mmtid = $mmtids[count($mmtids) - 1];

  // Check for mm_showpage callbacks, specified in hook_mm_showpage_routing()
  $showpage_no_nodes = FALSE;
  $router = _mm_showpage_router();
  if ($router) {
    $temp_path = "mm/$this_mmtid";
    $temp_options = array();
    mm_module_invoke_all_array('url_outbound_alter', array(&$temp_path, &$temp_options, $temp_path));
    $temp_path = join('/', array_merge(array($temp_path), $oarg_list));
    $temp_args = explode('/', $temp_path);
    $mm_region_contents = &drupal_static('mm_region_contents', array());

    foreach ($router as $key => $item) {
      if (preg_match($key, $temp_path) && (!isset($item['block id']) || $item['block id'] == $block_id) && _mm_showpage_callback($item, 'access', $temp_args, $oarg_list, $this_mmtid, $block_id)) {
        $showpage_output = _mm_showpage_callback($item, 'page', $temp_args, $oarg_list, $this_mmtid, $block_id);
        if (is_array($showpage_output)) {
          if (isset($showpage_output['by_region'])) {
            if (isset($showpage_output['by_region']['content'])) {
              // Save all content in regions other than 'content' for later.
              foreach ($showpage_output['by_region'] as $region => $content) {
                if ($region != 'content') {
                  $mm_region_contents[$region][] = $content;
                }
              }
              // Process just the 'content' region now.
              $showpage_output = $showpage_output['by_region']['content'];
            }
            else {
              $showpage_output = array();
            }
          }
          if (isset($showpage_output['output_pre'])) {
            if (!isset($output['output_pre'])) {
              $output['output_pre']['#weight'] = -1000;
            }
            $output['output_pre'][] = is_array($showpage_output['output_pre']) ? $showpage_output['output_pre'] : array('#type' => 'item', '#markup' => $showpage_output['output_pre']);
          }
          if (isset($showpage_output['output_post'])) {
            if (!isset($output['output_post'])) {
              $output['output_post']['#weight'] = 1000;
            }
            $output['output_post'][] = is_array($showpage_output['output_post']) ? $showpage_output['output_post'] : array('#type' => 'item', '#markup' => $showpage_output['output_post']);
          }
          if (isset($showpage_output['no_nodes'])) {
            $showpage_no_nodes |= $showpage_output['no_nodes'];
          }
        }
        elseif (!empty($showpage_output)) {
          if (!isset($output['output_post'])) {
            $output['output_post']['#weight'] = 1000;
          }
          $output['output_post'][] = array('#type' => 'item', '#markup' => $showpage_output);
          $showpage_no_nodes = FALSE;
        }
      }
    }
  }

  $showpage_show_nodes = !$showpage_no_nodes;
  $nodes_per_page = mm_content_resolve_cascaded_setting('nodes_per_page', $this_mmtid, $npp_at, $npp_parent);
  if (empty($nodes_per_page) && $nodes_per_page !== 0 || $nodes_per_page == -2 && $block_id) $nodes_per_page = variable_get('default_nodes_main', 10);

  $perms = mm_content_user_can($this_mmtid);
  if (!$perms[MM_PERMS_READ]) {
    $no_read++;
  }
  elseif ((count($mmtids) == 1 || count($mmtids) == 2 && variable_get('mm_use_virtual_user_dir', TRUE)) && $mmtids[0] == mm_content_users_mmtid()) {
    module_load_include('inc', 'monster_menus', 'mm_ui_user_list');
    $output[] = mm_ui_user_list_form($mmtids);
    $ok++;
  }
  elseif ($showpage_show_nodes) {
    $item = mm_content_get($this_mmtid, MM_GET_ARCHIVE);
    if (!_mm_render_nodes_on_page($item, $perms, $nodes_per_page, $oarg_list, $no_attribution, $output, $ok, $no_read, $pager_elem, $archive_tree, $archive_date_int, $rss_link)) {
      drupal_not_found();
      exit;
    }

    if ($ok && isset($oarg_list[0]) && $oarg_list[0] == 'feed') {
      $result = db_query_range(
          mm_content_get_accessible_nodes_by_mmtid_query($this_mmtid, $count_sql),
          0, variable_get('feed_default_items', 10));
      $nids = array();
      foreach ($result as $row) {
        if ($row->scheduled) {
          $nids[] = $row->nid;
        }
      }
      $channel = array(
        'link' => url("mm/$this_mmtid", array('absolute' => TRUE)),
        'title' => variable_get('site_name', 'drupal') . ': ' . $item->name,
        'description' => '',
      );
      node_feed($nids, $channel);
      exit;
    }

    // not a feed
    if ($block_id == 0) $_mm_page_subscribe_item = $item;

    if ($ok) {
      if ($allow_rss && ($item->rss || !variable_get('mm_enable_rss', FALSE))) {
        drupal_add_html_head_link(array('rel' => 'alternate',
          'type' => 'application/rss+xml',
          'title' => t('RSS'),
          'href' => !empty($rss_link) ? $rss_link : url("mm/$this_mmtid/feed", array('absolute' => TRUE))));
      }

      if (isset($item->main_mmtid) || isset($item->archive_mmtid)) {
        $archive = theme('mm_archive', array(
          'list' => $archive_tree,
          'frequency' => $item->frequency,
          'this_mmtid' => $this_mmtid,
          'main_mmtid' => $item->main_mmtid,
          'archive_mmtid' => $item->archive_mmtid,
          'date' => $archive_date_int));
        $output[] = array(
          $archive['header'],
          '#weight' => -1,
        );
        $output[] = array(
          $archive['links'],
          '#weight' => 100000,
        );
      }
      elseif (isset($pager_elem)) {
        $output[] = array(
          '#theme' => 'pager',
          '#tags' => NULL,
          '#element' => $pager_elem,
          '#weight' => (count($output) + 1) / 10000.0,
        );
      }
    }
  }
  elseif ($block_id == 0) {
    // Allow permalink to work even on pages with nodes suppressed by mm_showpage_routing
    $_mm_page_subscribe_item = mm_content_get($this_mmtid, MM_GET_ARCHIVE);
  }

  if ($output || $ok) {
    if ($nodes_per_page == -2) {
      mm_content_get_accessible_nodes_by_mmtid_query($this_mmtid, $count_sql);
      $total_nodes = db_query($count_sql)->fetchColumn();
      $total_pages = ceil($total_nodes / MM_LAZY_LOAD_NUMBER_OF_NODES);
      $output[] = array('#markup' => '<input type="hidden" value="0" class="mm-lazy-load-max-page">', '#weight' => 1);
      mm_static('lazy_load_node', TRUE, $this_mmtid, $total_pages);
    }
  }

  if ($output || $ok) {
    return array('mm_nodes' => $output);
  }

  $err = $no_read ? 'no read' : 'no content';
  return array();
}

// display a list of pages assigned to a tree entry
function _mm_render_nodes_on_page($item, $perms, $nodes_per_page, $oarg_list, $no_attribution, &$output, &$ok, &$no_read, &$pager_elem, &$archive_tree, &$archive_date_int, &$rss_link) {
  $_mm_mmtid_of_node = &drupal_static('_mm_mmtid_of_node');
  $rss_link = NULL;
  $archive_date_int = 0;
  $archive_tree = array();

  $result = NULL;
  if (isset($item->main_mmtid) || isset($item->archive_mmtid)) {
    // This is an archive page, or the main page for which there is an archive
    if ($item->mmtid > 0) {
      $mmtid = isset($item->main_mmtid) ? $item->main_mmtid : $item->mmtid;
      if (!mm_content_user_can($mmtid, MM_PERMS_READ)) {
        $no_read++;
        return TRUE;
      }
      $q = mm_content_get_accessible_nodes_by_mmtid_query($mmtid, $count_sql);
      $result = db_query($q);
    }
  }
  elseif (count($oarg_list) && !$output) {
    // if the remaining parameters in the URL can't be accounted for, it's a dead link
    $other_mmtids = array_diff($oarg_list, mm_content_reserved_aliases());
    if (count($other_mmtids)) {
      return FALSE;
    }
  }

  if (is_null($result)) {
    if (!mm_content_user_can($item->mmtid, MM_PERMS_READ)) {
      $no_read++;
      return TRUE;
    }

    $omit_nodes = '';
    if (!$perms[MM_PERMS_IS_RECYCLED]) {
      $omit_node_types = mm_get_node_info(MM_NODE_INFO_NO_RENDER, $item);
      if ($omit_node_types) {
        $omit_nodes = " AND n.type NOT IN('" . join("', '", $omit_node_types) . "')";
      }
    }

    $pager_elem = $nodes_per_page > 0 ? PagerDefault::$maxElement++ : NULL;
    $result = mm_content_get_accessible_nodes_by_mmtid($item->mmtid, $nodes_per_page, $pager_elem, '', '', $omit_nodes . ' AND r.region IS NULL');
  }

  if (isset($item->archive_mmtid)) {
    $archive_count = 0;
    if ($oarg_list && preg_match('/([12]\d\d\d)-(0[1-9]|1[0-2])-([0123]\d)/', $oarg_list[0], $matches)) {
      $archive_date = array(
        'year' => intval($matches[1]),
        'mon' =>  intval($matches[2]),
        'mday' => intval($matches[3]));
    }
  }

  $nids = $scheduled = array();
  foreach ($result as $n) {
    $ok++;
    $_mm_mmtid_of_node[$n->nid] = $item->mmtid;
    $scheduled[$n->nid] = !empty($n->scheduled);
    if (empty($oarg_list) || $oarg_list[0] != 'feed') {
      // This is an archive page, or the main page for which there is an archive,
      // and the node is not stuck on the main page, and it's always visible to everyone
      if (isset($item->archive_mmtid) && ($item->archive_mmtid == $item->mmtid || !$n->stuck && $n->scheduled && $n->status == 1)) {
        // skip this node if viewing the main page of an archive and we've seen main_nodes # of non-sticky, always-appearing nodes
        $archive_show = FALSE;
        if (++$archive_count > $item->main_nodes || $item->archive_mmtid == $item->mmtid && !$n->stuck && $n->scheduled && $n->status == 1) {
          $date = getdate($n->created);
          switch ($item->frequency) {
            case 'year':
              $rounded = mktime(0, 0, 0, 1, 1, $date['year']);
              $archive_tree[$date['year']] = $rounded;
              $archive_show = isset($archive_date) && $archive_date['year'] == $date['year'];
              break;

            case 'month':
              $rounded = mktime(0, 0, 0, $date['mon'], 1, $date['year']);
              $archive_tree[$date['year']][$date['mon']] = $rounded;
              $archive_show = isset($archive_date) && $archive_date['year'] == $date['year'] && $archive_date['mon'] == $date['mon'];
              break;

            case 'week':
              $rounded = mktime(0, 0, 0, $date['mon'], $date['mday']-$date['wday'], $date['year']);
              $date = getdate($rounded);
              $archive_tree[$date['year']][$date['mon']][$date['mday']] = $rounded;
              $archive_show = isset($archive_date) && $archive_date['year'] == $date['year'] && $archive_date['mon'] == $date['mon'] && $archive_date['mday'] == $date['mday'];
              break;

            case 'day':
              $rounded = mktime(0, 0, 0, $date['mon'], $date['mday'], $date['year']);
              $archive_tree[$date['year']][$date['mon']][$date['mday']] = $rounded;
              $archive_show = isset($archive_date) && $archive_date['year'] == $date['year'] && $archive_date['mon'] == $date['mon'] && $archive_date['mday'] == $date['mday'];
              break;
          }
          if (!isset($archive_date)) {
            // not in URL, so default to most recent node's date, rounded down
            $archive_date = getdate($rounded);
            $archive_show = TRUE;
          }
          if ($archive_show) $archive_date_int = $rounded;
        }

        // skip this node if viewing an archive page and it's sticky
        if ($item->archive_mmtid == $item->mmtid && ($n->stuck || !$n->scheduled || !$archive_show)) continue;
        // skip this node if viewing the main page and we've already seen main_nodes # of nodes
        if ($item->main_mmtid == $item->mmtid && $archive_count > $item->main_nodes) continue;
      }

      $nids[] = $n->nid;
    }
  }

  _mm_render_nodes($nids, $scheduled, $item->previews ? 'teaser' : 'full', $no_attribution, $output, $rss_link);
  return TRUE;
}

function _mm_render_nodes($nids, $scheduled, $view_mode, $no_attribution, &$output, &$rss_link) {
  foreach (node_load_multiple($nids) as $node) {
    $message = '';
    if (!$node->status) {
      $message = t('This piece of content is not yet published. It can only be seen by people who can edit it.');
    }
    elseif (empty($scheduled[$node->nid])) {
      $message = t('This piece of content can only be seen by people who can edit it, due to its publishing schedule.');
    }

    if ($no_attribution) {
      $node->no_attribution = TRUE;
    }
    drupal_alter('mm_node_show', $node, $view_mode);
    $body = node_view($node, $view_mode);
    // $node->no_display is a custom field that can be set by a hook_view module, such as rss_page_view
    if (empty($node->no_display)) {
      if ($message) {
        $body = array(
          '#prefix' => '<div class="preview">',
          'preview' => array(
            '#prefix' => '<div id="message">',
            'status' => array(
              '#prefix' => '<div class="messages status">',
              'message' => array(
                '#type' => 'item',
                '#markup' => $message,
              ),
              '#suffix' => '</div>',
            ),
            '#suffix' => '</div>',
          ),
          'body' => $body,
          '#suffix' => '</div>',
        );
      }
      $output[] = array(
        '#prefix' => "<a name=\"node-$node->nid\"></a>",
        'body' => $body,
        '#weight' => (count($output) + 1) / 10000.0,
      );
    }

    $rss_link = is_null($rss_link) && !empty($node->rss_link) ? $node->rss_link : FALSE;
  }
}

function _mm_resolve_archive(&$mmtid) {
  $mmtid = intval($mmtid);
  if ($mmtid) {
    $tree = mm_content_get($mmtid, MM_GET_ARCHIVE);
    if (isset($tree->main_mmtid) && $tree->archive_mmtid == $mmtid) {
      if (!mm_content_user_can($mmtid, MM_PERMS_READ) || !mm_content_user_can($tree->main_mmtid, MM_PERMS_READ)) {
        mm_access_denied();
        return FALSE;
      }
      return $tree->main_mmtid;
    }
    return $mmtid;
  }
  return FALSE;
}

function _mm_showpage_router($reset = FALSE) {
  $router = &drupal_static(__FUNCTION__);

  if (!isset($router) || $reset) {
    if (!$reset && ($cache = cache_get('mm_showpage')) && isset($cache->data)) {
      $router = $cache->data;
    }
    else {
      $callbacks = array();
      foreach (mm_module_implements('mm_showpage_routing') as $module) {
        $router_items = call_user_func($module . '_mm_showpage_routing');
        if (isset($router_items) && is_array($router_items)) {
          foreach (array_keys($router_items) as $path) {
            if (!isset($router_items[$path]['module'])) {
              $router_items[$path]['module'] = $module;
            }
          }
          $callbacks = array_merge($callbacks, $router_items);
        }
      }

      $router = $sort = array();
      foreach ($callbacks as $path => $item) {
        list($fit,) = _mm_showpage_router_fit($path, isset($item['partial path']) ? $item['partial path'] : '');
        $ending = !empty($item['partial path']) ? '(?:$|/)}' : '$}';
        $path = '{^' . str_replace(array('%', '\\*'), array('[^/]+', '[^/]*'), preg_quote($path)) . $ending;
        $sort[$path] = $fit;

        if (!isset($item['access callback']) && isset($item['access arguments'])) {
          // Default callback.
          $item['access callback'] = 'mm_content_user_can';
        }
        if (empty($item['page callback'])) {
          $item['access callback'] = FALSE;
        }
        elseif (!isset($item['access callback'])) {
          $item['access callback'] = 'mm_content_user_can';
          $item['access arguments'] = array('_mmtid_', MM_PERMS_READ);
        }
        $item += array(
          'access arguments' => array(),
          'access callback' => '',
          'page arguments' => array(),
          'page callback' => '',
          'file' => '',
        );
        $router[$path] = $item;
      }
      array_multisort($sort, SORT_NUMERIC, SORT_DESC, $router);

      cache_set('mm_showpage', $router);
    }
  }

  return $router;
}

function _mm_showpage_callback($item, $type, $args, $oargs, $this_mmtid, $block_id) {
  $callback = $item["$type callback"];
  if (is_bool($callback)) {
    return $callback;
  }

  if (!empty($item['file'])) {
    $file = DRUPAL_ROOT . '/' . drupal_get_path('module', $item['module']) . '/' . $item['file'];
    require_once $file;
  }

  $callback = trim($callback);
  if (empty($callback) || !function_exists($callback)) {
    return FALSE;
  }

  $arguments = $item["$type arguments"];
  $all = array();
  foreach ($arguments as $k => $v) {
    if (is_int($v)) {
      $arguments[$k] = isset($args[$v]) ? $args[$v] : '';
    }
    elseif ($v === '_mmtid_') {
      $arguments[$k] = $this_mmtid;
    }
    elseif ($v === '_block_id_') {
      $arguments[$k] = $block_id;
    }
    elseif ($v === '_all_') {
      array_unshift($all, $k);
    }
    elseif ($v === '_oargs_') {
      array_unshift($oargs, $k);
    }
  }

  foreach ($all as $k) {
    array_splice($arguments, $k, 1, $args);
  }

  return call_user_func_array($callback, $arguments);
}

function _mm_showpage_router_fit($path, $partial_path) {
  $fit = 0;
  $parts = explode('/', $path);
  $number_parts = count($parts);
  foreach ($parts as $k => $part) {
    if ($part != '%') {
      $fit |= 1 << ($number_parts - 1 - $k);
    }
  }

  if (!$fit) {
    // If there is no %, it fits maximally.
    $fit = (1 << $number_parts) - 1;
  }
  $fit = ($fit << 1) + ($partial_path ? 0 : 1);

  return array($fit, $number_parts);
}

function _mm_soundex($string) {
  $cache = &drupal_static(__FUNCTION__);

  // The PHP soundex() function is incorrect, so use the SQL version, but cache
  // the results.
  $string = preg_replace('/[\x80-\xFF]/', '', $string);
  if (!isset($cache[$string])) {
    $cache[$string] = db_query('SELECT SOUNDEX(:string)', array(':string' => $string))->fetchField();
  }
  return $cache[$string];
}

function _mm_report_error($message, $vars, &$stats, $watchdog_type = WATCHDOG_ERROR) {
  if (is_array($stats)) {
    $stats['errors'][] = array('message' => $message, 'vars' => $vars);
    if (isset($stats['suppress_errors'])) {
      return;
    }
  }
  drupal_set_message(t($message, $vars));
  watchdog('mm', $message, $vars, $watchdog_type);
}

function _mm_report_stat($is_group, $mmtid, $message, $vars, &$stats, $watchdog_notice = FALSE) {
  if (is_numeric($mmtid)) {
    $vars['!mmtid'] = l($mmtid, "mm/$mmtid");
  }
  if (is_array($stats)) {
    $vars['@thing'] = $is_group ? 'group' : 'page';
    $stats[$is_group ? 'groups' : 'pages'][$mmtid][] = array('message' => $message, 'vars' => $vars);
  }
  if ($watchdog_notice) {
    watchdog('mm', $message, $vars);
  }
}

function mm_var_export_html($var) {
  return '<pre>' . check_plain(var_export($var, TRUE)) . '</pre>';
}

/**
 * This is a version of drupal_get_title() without the additional check_plain().
 * It must only be used when certain that the title was escaped while being set
 * previously.
 *
 * @return null|string
 *   The page title, without any additional escaping.
 */
function mm_get_html_title() {
  $title = drupal_set_title(NULL, PASS_THROUGH);

  // During a bootstrap, menu.inc is not included and thus we cannot provide a title.
  if (!isset($title) && function_exists('menu_get_active_title')) {
    return menu_get_active_title();
  }

  return $title;
}
