<?php

/**
 * @file
 * Hansel breadcrumb actions
 *
 * Breadcrumb actions from Hansel core are located in this file.
 * The definitions resides in hansel.module
 *
 * @see hansel_hansel_action_types()
 */

/**
 * Callback for the "add single link" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_single_link_get_crumbs($arguments) {
  global $_hansel_flags;
  if (!empty($arguments['skipalias']) && in_array('alias', $_hansel_flags)) {
    return array();
  }
  if (isset($arguments['translate']) && $arguments['translate']) {
    $title = t($arguments['title']);
  }
  else {
    $title = $arguments['title'];
  }
  if (!empty($arguments['remove_tokens'])) {
    $title = preg_replace('/\\[[a-z0-9_\\-]+\\]/si', '', $title);
  }
  return array(
    array(
      'title' => $title,
      'href' => $arguments['path'],
    ),
  );
}

/**
 * Callback for the "add single link" breadcrumb action to generate the info line.
 *
 * @param array $arguments
 * @return string
 */
function hansel_action_add_single_link_info($arguments) {
  return t('add link %title', array('%title' => $arguments['title']));
}

/**
 * Callback for the "add link to node" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_link_to_node_get_crumbs($arguments) {
  global $_hansel_flags;
  global $language;

  if (drupal_strtolower(hansel_arg(0)) == 'node' && is_numeric(hansel_arg(1))) {
    $href = 'node/' . hansel_arg(1);

    /**
     * Use the menu item title if available.
     * The menu item title is used to active the menu items, and is used by the
     * add parents breadcrumb action. So if available, we should use this one.
     * This is to fix the activation of menu items when the node title differs
     * from the menu item title.
     */
    $query = db_select('menu_links', 'c');
    if (module_exists('i18n_menu')) {
      $query->fields('c', array('menu_name', 'mlid', 'link_title', 'language'));
      $query->condition('c.language', array($language->language, LANGUAGE_NONE));
    }
    else {
      $query->fields('c', array('menu_name', 'link_title'));
    }
    $query->condition('c.link_path', $href)
      ->condition('hidden', 0)
      ->orderBy('depth')
      ->range(0, 1);

    $link = $query->execute()->fetchObject();
    if ($link) {
      if (module_exists('i18n_menu')) {
        $i18n_mode = i18n_menu_mode($link->menu_name);
      }
      else {
        $i18n_mode = FALSE;
      }
      if ($i18n_mode == '5' && $link->language == LANGUAGE_NONE) {
        // Menu is in "Translate and localise" mode and the menu item is "Language neutral" (translatable).
        $link_title = i18n_string_translate(array('menu', 'item', $link->mlid, 'title'), $link->link_title, array('langcode' => $language->language, 'sanitize' => FALSE));
      }
      else {
        // Menu is in "Fixed language" mode, or item is localized (not in "Language neutral").
        $link_title = $link->link_title;
      }
      return array(
        array(
          'title' => $link_title,
          'href' => $href,
        ),
      );
    } elseif ($node = node_load(hansel_arg(1))) {
      return array(
        array(
          'title' => $node->title,
          'href' => $href,
        ),
      );
    }
  }
  return array();
}

/**
 * Callback for the "add link to nodetype" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_link_to_nodetype_get_crumbs($arguments) {
  if (drupal_strtolower(hansel_arg(0)) == 'node' && is_numeric(hansel_arg(1))) {
    if ($node = node_load(hansel_arg(1))) {
      $type = $node->type;
    }
  }
  elseif (drupal_strtolower(hansel_arg(0)) == 'node' && drupal_strtolower(hansel_arg(1)) == 'add') {
    // Nodetypes may not contain hyphens. So it's same to assume that all hyphens are underscores.
    $type = str_replace('-', '_', hansel_arg(2));
  }
  if (!empty($type)) {
    $path = isset($arguments['path']) ? $arguments['path'] : 'node/add/[type]';
    $path = str_replace('[type]', str_replace('_', '-', $type), $path);
    $path = str_replace('[type-raw]', $type, $path);
    $node = new stdClass();
    $node->type = $type;
    if ($title = node_type_get_name($node)) {
      return array(
        array(
          'title' => t($title),
          'href' => $path,
        ),
      );
    }
  }
  return array();
}

/**
 * Callback for the "add link to user" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_link_to_user_get_crumbs($arguments) {
  global $user;
  $links = array();

  // Check if we have to use the realname integration.
  if (module_exists('realname')) {
    $use_realname = (isset($arguments['realname']) && $arguments['realname']);
  }
  else {
    $use_realname = FALSE;
  }

  if (drupal_strtolower(hansel_arg(0)) == 'user' && is_numeric(hansel_arg(1))) {
    if (!empty($arguments['you']) && $user->uid == hansel_arg(1) && $user->uid) {
      $links[] = array(
        'title' => (isset($arguments['translate']) && $arguments['translate']) ? t($arguments['you']) : $arguments['you'],
        'href' => 'user/' . $user->uid,
      );
    }
    else {
      $cid = 'user:' . hansel_arg(1) . ':' . ($use_realname ? 'real' : 'std');
      if ($data = hansel_cache_get($cid)) {
        return $data;
      }
      if ($account = user_load(hansel_arg(1))) {
        $links[] = array(
          'title' => $use_realname ? $account->realname : $account->name,
          'href' => 'user/' . $account->uid,
        );
      }
      hansel_cache_set($cid, $links);
    }
  }
  elseif (drupal_strtolower(hansel_arg(0)) == 'node' && is_numeric(hansel_arg(1))) {
    if ($node = node_load(hansel_arg(1))) {
      if (!empty($arguments['you']) && $node->uid == $user->uid && $node->uid) {
        $links[] = array(
          'title' => (isset($arguments['translate']) && $arguments['translate']) ? t($arguments['you']) : $arguments['you'],
          'href' => 'user/' . $user->uid
        );
      }
      else {
        $cid = 'user:' . hansel_arg(1) . ':' . ($use_realname ? 'real' : 'std');
        if ($data = hansel_cache_get($cid)) {
          return $data;
        }
        if ($account = user_load($node->uid)) {
          $links[] = array(
            'title' => $use_realname ? $account->realname : $account->name,
            'href' => 'user/' . $account->uid,
          );
        }
        hansel_cache_set($cid, $links);
      }
    }
  }
  return $links;
}

/**
 * Callback for the "add link to current page" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_link_to_current_page_get_crumbs($arguments) {
  $args = array();
  $i = 0;
  while ($arg = hansel_arg($i++)) {
    $args[] = $arg;
  }
  $title = drupal_get_title();

  // Convert to plaintext.
  $title = decode_entities(strip_tags($title));

  if (!empty($title)) {
    return array(
      array(
        'title' => $title,
        'href' => implode('/', $args),
      )
    );
  }
}

/**
 * Callback for the "add single link" breadcrumb action to generate the crumbs.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_parents_get_crumbs($arguments) {
  $args = array();
  $i = 0;
  while ($arg = hansel_arg($i++)) {
    $args[] = $arg;
  }
  $path = implode('/', $args);

  if ($path == variable_get('site_frontpage', 'node')) {
    // Do not output parents for frontpage.
    return array();
  }

  if ($crumbs = hansel_cache_get("parents:$path")) {
    return $crumbs;
  }

  $crumbs = array();
  $parent_hashes = array();

  $modules = module_implements('hansel_get_parent');

  while (TRUE) {
    foreach ($modules as $module) {
      $hook = "{$module}_hansel_get_parent";
      if ($parent = $hook($path)) {
        // Create a hash to check recursion. The reason that we do not simply
        // check the paths is that the site structure may not be hierarchical.
        // There could be a parent item with the same path. This is often used
        // in cases where a top level item item has the same link as its first
        // child item.
        $hash = crc32($parent['path'] . $parent['title']);
        if (in_array($hash, $parent_hashes)) {
          // We are in recursion.
          break(2);
        }
        $parent_hashes[] = $hash;
        if ($parent['path'] == variable_get('site_frontpage', 'node') || $parent['path'] == '<front>') {
          // The frontpage is not added as a parent, to avoid issues having
          // duplicated items in the breadcrumb when the frontpage is created
          // as a menu item.
          break;
        }
        array_unshift($crumbs, array(
          'title' => $parent['title'],
          'href' => $parent['path'],
        ));
        $path = $parent['path'];
        continue(2);
      }
    }
    // We tried all hooks but did not find a parent.
    break;
  }

  hansel_cache_set('parents:' . implode('/', $args), $crumbs);

  return $crumbs;
}

/**
 * Implements hook_hansel_get_parent().
 *
 * @param string $path
 * @return array
 */
function menu_hansel_get_parent($path) {
  global $language;

  // Build a list of menu names. We will only look for items in one of these menu's.
  $menus = menu_get_menus();
  if (isset($menus['devel'])) {
    // Do not use the development menu.
    unset($menus['devel']);
  }
  $menus = array_keys($menus);

  // Let other modules alter the menu list.
  drupal_alter('hansel_menus', $menus);

  if ($menus) {
    // Try to get parent by menu.
    $query = db_select('menu_links', 'c');
    $parent = $query->join('menu_links', 'p', 'c.plid = p.mlid');
    if (module_exists('i18n_menu')) {
      $query->fields($parent, array('menu_name', 'mlid', 'link_path', 'link_title', 'language'));
      $query->condition('c.language', array($language->language, LANGUAGE_NONE));
    }
    else {
      $query->fields($parent, array('menu_name', 'link_path', 'link_title'));
    }
    $query->condition('c.link_path', $path)
      ->condition('c.menu_name', $menus)
      ->orderBy("$parent.depth", 'asc')
      ->orderBy("$parent.weight", 'asc')
      ->range(0, 1);
    $link = $query->execute()->fetchObject();
    if ($link) {
      if (module_exists('i18n_menu')) {
        $i18n_mode = i18n_menu_mode($link->menu_name);
      }
      else {
        $i18n_mode = FALSE;
      }
      if ($i18n_mode == '5' && $link->language == LANGUAGE_NONE) {
        // Menu is in "Translate and localise" mode and the menu item is "Language neutral" (translatable).
        $link_title = i18n_string_translate(array('menu', 'item', $link->mlid, 'title'), $link->link_title, array('langcode' => $language->language, 'sanitize' => FALSE));
      }
      else {
        // Menu is in "Fixed language" mode, or item is localized (not in "Language neutral").
        $link_title = $link->link_title;
      }
      return array(
        'path' => $link->link_path,
        'title' => $link_title,
      );
    }
  }

  if (preg_match('/^node\\/([0-9]+)$/si', $path, $match)) {
    // Try to get parent by nodetype settings.
    $nodetypes = variable_get('hansel_nodetypes', array());
    $node = node_load($match[1]);
    if (($node = node_load($match[1])) && isset($nodetypes[$node->type])) {
      return $nodetypes[$node->type];
    }
  }

  return FALSE;
}

/**
 * Implements hook_hansel_get_parent().
 *
 * @param string $path
 * @return array
 */
function path_hansel_get_parent($path) {
  if (variable_get('hansel_parent_lookup_paths', TRUE)) {
    $args = array();
    $i = 0;
    while ($arg = hansel_arg($i++)) {
      $args[] = $arg;
    }
    $path = implode('/', $args);

    $alias = drupal_get_path_alias($path);
    $alias = substr($alias, 0, strrpos($alias, '/'));
    if (!($path = drupal_lookup_path($alias))) {
      // The alias is probably a valid system path.
      $path = $alias;
    }
    if (!$parent_path = drupal_lookup_path($path)) {
      $parent_path = $path;
    }
    if ($item = menu_get_item($parent_path)) {
      return array(
        'path' => $parent_path,
        'title' => $item['title'],
      );
    }
  }
  return FALSE;
}

/**
 * Callback for the "add single link" breadcrumb action to generate the config form.
 *
 * @param array $arguments
 * @return array
 */
function hansel_action_add_parents_config_form($arguments) {
  return array();
}
