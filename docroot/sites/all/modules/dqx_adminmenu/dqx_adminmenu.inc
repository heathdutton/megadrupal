<?php


/**
 * Page callback for admin/dqx-adminmenu-data-json
 * Output administration menu for HTTP caching via AJAX request.
 */
function dqx_adminmenu_xml() {

  if (!user_access('use dqx_adminmenu')) {
    $xml = <<<EOT
      <div class="dqx_adminmenu-access_denied">dqx_adminmenu access denied</div>
EOT;
  }
  else if ($GLOBALS['user']->uid != $_GET['uid']) {
    $uid_session = $GLOBALS['user']->uid;
    $uid_request = $_GET['uid'];
    $xml = <<<EOT
      <div class="dqx_adminmenu-access_denied">dqx_adminmenu access denied due to user id mismatch. Session says uid = $uid_session, request says uid = $uid_request.</div>
EOT;
  }
  else {

    // Load from cache
    $uid = $GLOBALS['user']->uid;
    $lang = $GLOBALS['language']->language;
    $cid = "dqx_adminmenu:xml:$uid:$lang";
    $cache_get = cache_get($cid);

    // Timestamp of the latest menu rebuild.
    $timestamp = variable_get('dqx_adminmenu_timestamp', 0);
    $time = date('Y-m-d H:i:s', $timestamp);

    $messages[] = "uid:$uid, lang: $lang, timestamp: $time";

    // Check if cache is up to date, and has useful data.
    if (!is_object($cache_get) || !isset($cache_get->data['xml']) || $cache_get->data['timestamp'] != $timestamp) {
      $t0 = microtime(TRUE);
      $xml = _dqx_adminmenu_html();
      $duration = number_format((microtime(TRUE) - $t0) * 1000, 2) . ' ms';
      $cache = array('timestamp' => $timestamp, 'xml' => $xml);
      cache_set($cid, $cache, 'cache', CACHE_TEMPORARY);
      $messages[] = 'Fresh';
      $messages[] = "$duration to build the menu";
    }
    else {
      $xml = $cache_get->data['xml'];
      $messages[] = 'From cache';
    }
  }

  // header("Content-type: text/xml");
  header("Cache-Control: ''");  // HTTP/1.1
  $expires = date('D, d M Y H:i:s GMT', time() + 3600 * 24 * 2);  // two days ahead
  header("Expires: $expires");

  $t0 = microtime(TRUE);
  // Such a print statement can be disproportionaly slow for long strings.
  // This is reported on https://bugs.php.net/bug.php?id=18029.
  // On https://bugs.php.net/bug.php?id=18367&edit=3, it is suggested to print
  // the string in chunks. Unfortunately, this does not help.
  print $xml;
  $duration = number_format((microtime(TRUE) - $t0) * 1000, 2) . ' ms';
  $messages[] = "$duration to print the xml";

  // Provide benchmark information to site maintainers.
  // Chromium developer tools do a good job in displaying these comments.
  foreach ($messages as $msg) {
    print "\n<!-- $msg -->";
  }

  exit();
}


function _dqx_adminmenu_html() {
  $tree_html = _dqx_adminmenu_tree_html();

  $html = !$tree_html ? '' : <<<EOT
    <div id="dqx_adminmenu">$tree_html</div>
EOT;

  return trim($html);
}


function _dqx_adminmenu_tree_html() {

  _dqx_adminmenu_init_plugin_dir();

  $cache = new dqx_adminmenu_MenuRouterCache();

  // build items and check access
  $items = array();
  $api = new dqx_adminmenu_InjectedAPI_items($items, $cache);
  foreach (module_implements('dqx_adminmenu_items') as $module) {
    $f = $module . '_dqx_adminmenu_items';
    $result = $f($api);
    if (is_array($result)) {
      foreach ($result as $key => $item) {
        $items[$key] = $item;
      }
    }
  }

  // alter items and parent map
  $parent_map = array();
  $inline_children = array();

  $api = new dqx_adminmenu_InjectedAPI_itemsAlter($items, $parent_map, $inline_children);
  module_invoke_all('dqx_adminmenu_items_alter', $api);

  // build and alter submenus
  $submenus = dqx_adminmenu_build_structure($items, $parent_map);

  $api = new dqx_adminmenu_InjectedAPI_structureAlter($items, $submenus, $inline_children);
  module_invoke_all('dqx_adminmenu_structure_alter', $api);

  $treeObject = new dqx_adminmenu_MenuTree($submenus, $items, $inline_children);

  $ul_tree = $treeObject->renderMenu('admin');
  $ul_user = $treeObject->renderMenu('dqx-adminmenu-user');

  if ($ul_tree) {
    return $ul_user . $ul_tree;
  }
}


function _dqx_adminmenu_init_plugin_dir() {
  $plugin_dir = drupal_get_path('module', 'dqx_adminmenu') . '/plugins';
  foreach (scandir($plugin_dir) as $candidate) {
    if ($candidate != '.' && $candidate != '..' && is_file($plugin_dir . '/' . $candidate)) {
      if (preg_match('#^dqx_adminmenu\.([^\.]+)\.inc$#', $candidate, $m)) {
        if (module_exists($m[1])) {
          require_once $plugin_dir . '/' . $candidate;
        }
      }
    }
  }
}


function dqx_adminmenu_build_structure(array $items, array $parent_map) {
  // Normally, we determine the parent of an item by chopping off the last path fragment.
  // Some items should have a parent different from that.

  $paths = array_keys($items);
  $prefix_map = $parent_map;
  if (!empty($paths)) {
    $prefix_map += array_combine($paths, $paths);
  }

  $submenus = array();
  foreach ($items as $path => $item) {
    if (!empty($item['needs_approval'])) {
      if (!_dqx_adminmenu_find_path_prefix($path, $parent_map)) {
        continue;
      }
    }
    $prefix = _dqx_adminmenu_find_path_prefix($path, $prefix_map);
    if (strlen($prefix)) {
      $submenus[$prefix][$path] = $path;
    }
  }

  return $submenus;
}


function _dqx_adminmenu_find_path_prefix($path, array $haystack) {
  $fragments = explode('/', $path);
  while (count($fragments) > 1) {
    array_pop($fragments);
    $prefix = implode('/', $fragments);
    if (isset($haystack[$prefix])) {
      return $haystack[$prefix];
    }
  }
}


function _dqx_adminmenu_build_menu_item($item) {

  if ($item['type'] == MENU_CALLBACK) {
    $item['hidden'] = -1;
  }
  elseif ($item['type'] == MENU_SUGGESTED_ITEM) {
    $item['hidden'] = 1;
  }

  // TODO: Why do we need this?
  $item['module'] = 'dqx_adminmenu';

  $item += array(
    'menu_name' => 'dqx_adminmenu',
    'link_title' => $item['title'],
    'link_path' => $item['path'],
    'hidden' => 0,
    'options' => empty($item['description']) ? array() : array(
      'attributes' => array('title' => $item['description'])
    ),

    'router_path' => $item['path'],

    // this is taken from menu_item_save() in D6.
    'weight' => 0,
    'has_children' => 0,
    'expanded' => 1,
    'customized' => 0,
    'updated' => 0,

    // stuff added for D7
    'external' => FALSE,
  );

  // _menu_link_translate() wants serialized options
  $item['options'] = serialize($item['options']);

  // Check access
  // This does expect serialized options, but it does unserialize them again after.
  _menu_link_translate($item);

  return $item;
}


function _dqx_adminmenu_router_items($sql_filter, array $sql_args = array(), array $allow_wildcard = NULL) {

  $q = db_query("
    SELECT *
    FROM {menu_router}
    WHERE title != '' AND ($sql_filter)
    ORDER BY weight ASC, title ASC
  ", $sql_args);

  $allow_wildcard_regex = array();
  if (is_array($allow_wildcard)) {
    foreach ($allow_wildcard as $wildcard => $replace) {
      $regex = '#^' . preg_quote($wildcard) . '([^\%]*)$#';
      $allow_wildcard_regex[$regex] = $replace;
    }
  }

  $items = array();
  $callback_items = array();
  while ($item = $q->fetchAssoc()) {
    if (strpos($item['path'], '%')) {
      if (is_array($allow_wildcard)) {
        $discard = TRUE;
        foreach ($allow_wildcard_regex as $regex => $replace) {
          if (preg_match($regex, $item['path'], $m)) {
            $item['path'] = $replace . $m[1];
            $discard = FALSE;
          }
        }
        if ($discard) {
          continue;
        }
      }
      else {
        continue;
      }
    }
    if ($item['type'] & (MENU_VISIBLE_IN_TREE | MENU_VISIBLE_IN_BREADCRUMB | MENU_IS_LOCAL_TASK)) {
      $item = _dqx_adminmenu_build_menu_item($item);
      $items[$item['path']] = $item;
    }
    elseif ($item['type'] & MENU_VISIBLE_IN_BREADCRUMB && $item['number_parts'] > 2) {
      // $item['item_attributes']['style'] = 'background:green;';
      $item['needs_approval'] = TRUE;
      $item = _dqx_adminmenu_build_menu_item($item);
      $items[$item['path']] = $item;
    }
  }

  return $items;
}


function dqx_adminmenu_link_attributes($path, array $options) {
  $obj = dqx_adminmenu_extract_attributes($options, 'attributes', FALSE);
  $obj->setAttribute('href', url($path, $options));
  return $obj;
}


function dqx_adminmenu_extract_attributes(array $info_array, $array_key, $raw = TRUE) {
  $attr = array();
  if (is_array(@$info_array[$array_key])) {
    $attr = $info_array[$array_key];
  }
  return dqx_adminmenu_tag_attributes($attr, $raw);
}


function dqx_adminmenu_tag_attributes($attr = NULL, $raw = TRUE) {
  if (is_object($attr)) {
    return $attr;
  }
  else {
    $obj = new dqx_adminmenu_HtmlTagAttributes();
    if (is_array($attr)) {
      $obj->setAttributes($attr, $raw);
      if (isset($attr['class'])) {
        $obj->addClasses($attr['class']);
      }
    }
    return $obj;
  }
}


function dqx_adminmenu_class_attribute($classes) {
  if (!is_object($classes)) {
    $attr = new dqx_adminmenu_HtmlTagAttributes();
    $attr->addClasses($classes);
    return $attr;
  }
  else {
    // assume that it is already a TagAttributes object
    return $classes;
  }
}
