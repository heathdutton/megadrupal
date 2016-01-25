<?php
/**
 * @file
 * Inclusion file of functions for dominion_menu.
 */

/**
 * Create subsite menu's if not already exists.
 * 
 * @param int $domain_id
 * @param array $functions
 * @param array $overrides
 *   array(
 *     cid => array(
 *       'disable' => FALSE,
 *       'copy_source' => NULL,
 *       'default_links' => '',
 *     ),
 *     ...
 *   )
 */
function _dominion_menu_create($domain_id, $functions, $overrides = array()) {
  $menus = array();
  $domain = db_select('domain', 'd')->fields('d')->condition('d.domain_id', $domain_id)->execute()->fetchAssoc();
  if (!($domain) || !($subsite = _dominion_load_info($domain_id))) {
    drupal_set_message(t('Error creating menu\'s'));
  }
  $query = db_select('dominion_menu_config', 'c');
  $mcb = $query->join('dominion_menu_config_blueprint', 'mcb', 'c.cid = mcb.cid');
  $res = $query->condition('mcb.blueprint', $subsite->blueprint)
    ->fields('c')
    ->execute();
  while ($config = $res->fetchAssoc()) {
    $config += isset($overrides[$config['cid']]) ? $overrides[$config['cid']] : array();
    $config += array('disable' => FALSE);
    if (!$config['disable']) {
      $create = TRUE;
      if ($config['inherit'] && $subsite->parent) {
        // Check if the parent has a menu built using the same config.
        $name = db_select('dominion_menu', 'm')
          ->fields('m', array('menu_name'))
          ->condition('m.domain_id', $subsite->parent)
          ->condition('m.cid', $config['cid'])
          ->execute()
          ->fetchField();
        if ($name) {
          // Do not create a menu. Use the parent subsite menu instead.
          $create = FALSE;
          // If not exists in dominion_menu table.
          if (!db_select('dominion_menu', 'm')->fields('m')->condition('m.domain_id', $domain_id)->condition('cid', $config['cid'])->execute()->fetchObject()) {
            db_insert('dominion_menu')->fields(array(
              'domain_id' => $domain_id,
              'menu_name' => $name,
              'cid' => $config['cid'],
            ))->execute();
          }
        }
      }
      if ($create) {
        $name = 'menu-' . token_replace($config['name_template'], array('domain' => $domain), array('clear' => TRUE));
        $description = token_replace($config['description_template'], array('domain' => $domain), array('clear' => TRUE));
        $description = drupal_substr($description, 0, 255);
        if (!_dominion_menu_exists($name)) {
          _dominion_menu_create_menu($domain_id, $config['cid'], $name, $description);
          if ($config['copy_source']) {
            _dominion_menu_copy_links($name, $config['copy_source']);
          }
          else {
            $links = token_replace($config['default_links'], array('domain' => $domain), array('clear' => TRUE));
            _dominion_menu_create_links($name, $links, $functions);
          }
        }
        else {
          // Menu already exists. Update name.
          $name = preg_replace('/[^a-z0-9]/', '-', strtolower($name));
          db_update('menu_custom')->fields(array(
            'title' => $description,
          ))->condition('menu_name', $name)->execute();
        }
      }
      $menus[] = $name;
    }
  }
  _dominion_menu_set_parent_options($domain_id, $menus);
}

/**
 * Check if a menu exists.
 *
 * @return string $name
 */
function _dominion_menu_exists($name) {
  return (bool) db_select('menu_custom', 'mc')
    ->fields('mc', array('menu_name'))
    ->condition('mc.menu_name', $name)
    ->execute()
    ->fetchField();
}

/**
 * Create menu.
 * 
 * @param string $name
 * @param string $description
 */
function _dominion_menu_create_menu($domain_id, $cid, $name, $description) {
  $name = preg_replace('/[^a-z0-9]/', '-', strtolower($name));
  
  $menu_edit_menu_form_state = array(
    'values' => array(
      'menu_name' => drupal_substr($name, 5, MENU_MAX_MENU_NAME_LENGTH_UI),
      'title' => $description,
      'description' => '',
    )
  );
  module_load_include('inc', 'menu', 'menu.admin');
  drupal_form_submit('menu_edit_menu', $menu_edit_menu_form_state, 'add');
  
  db_insert('dominion_menu')->fields(array(
    'domain_id' => $domain_id,
    'menu_name' => $name,
    'cid' => $cid,
  ))->execute();
}

/**
 * Copy menu links from other menu.
 * 
 * @param string $menu
 * @param string $source
 */
function _dominion_menu_copy_links($menu, $source, $source_plid = 0, $target_plid = 0) {
  $links = db_select('menu_links', 'l')
    ->fields('l', array('mlid', 'link_path', 'link_title', 'hidden', 'has_children', 'weight'))
    ->condition('l.menu_name', $source)
    ->condition('l.plid', $source_plid)
    ->execute()
    ->fetchAll();
  foreach ($links as $link) {
    $item = array(
      'menu_name' => $menu,
      'link_path' => $link->link_path,
      'link_title' => $link->link_title,
      'hidden' => $link->hidden,
      'weight' => $link->weight,
    );
    if ($target_plid) {
      $item['plid'] = $target_plid;
    }
    $parent_candidates = array();
    menu_link_save($item, array(), $parent_candidates);
    if ($link->has_children) {
      _dominion_menu_copy_links($menu, $source, $link->mlid, $item['mlid']);
    }
  }
}

/**
 * Create menu links.
 * 
 * @param string $menu
 * @param string $configuration
 * @param array $functions
 */
function _dominion_menu_create_links($menu, $configuration, $functions) {
  $weight = 0;
  $items = array_filter(array_map('trim', explode("\n", $configuration)));
  foreach ($items as $item) {
    $item = array_filter(array_map('trim', explode('|', $item)));
    if (isset($item[2]) && empty($functions[$item[2]])) {
      // This subsite does not have the required function enabled.
      continue;
    }
    ++$weight;
    $item = array(
      'link_path' => $item[0],
      'link_title' => $item[1],
      'weight' => $weight,
      'menu_name' => $menu,
    );
    menu_link_save($item);
  }
}

/**
 * Set parent options for menu tab on node edit pages.
 * 
 * @param int $domain_id
 * @param array $menus
 */
function _dominion_menu_set_parent_options($domain_id, $menus) {
  $menus = drupal_map_assoc($menus);
  $info = entity_get_info('node');
  $types = array_keys($info['bundles']);
  foreach ($types as $type) {
    $type_menus = variable_get('menu_options_' . $type, array('main-menu' => 'main-menu'));
    // When this is empty, menu links are not allowed for this nodetype.
    // Only overwrite setting if this is not empty.
    if ($type_menus) {
      domain_conf_variable_set($domain_id, 'menu_options_' . $type, $menus);
    }
  }
}

/**
 * Delete subsite menus.
 *
 * @param int $domain_id
 */
function _dominion_menu_delete($domain_id) {
  // Start a batch to delete the domain menu's.
  $batch = array(
    'operations' => array(),
    'title' => t('Deleting domain menu\'s'),
    'init_message' => t('Starting'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Error'),
  );
  $menus = db_select('dominion_menu', 'm')
    ->fields('m', array('menu_name'))
    ->condition('m.domain_id', $domain_id)
    ->execute()
    ->fetchCol();
  foreach ($menus as $name) {
    $batch['operations'][] = array('_dominion_menu_delete_menu', array($domain_id, $name));
  }
  if ($batch['operations']) {
    batch_set($batch);
  }
  db_delete('dominion_menu')->condition('domain_id', $domain_id)->execute();
}

/**
 * Get menu's which contains menu items copied from the same menu or from this menu.
 * 
 * @param string $menu_name
 * @param array
 *   array(name => label, ...)
 */
function _dominion_menu_get_copies($menu_name) {
  $menus = array();
  $menu_name = preg_replace('/^menu\\-/', '', $menu_name);
  // Add menu's copied from this.
  $query = db_select('dominion_menu_config', 'dmc');
  $dm = $query->join('dominion_menu', 'dm', 'dmc.cid = dm.cid');
  $mc = $query->join('menu_custom', 'mc', "$dm.menu_name = mc.menu_name");
  $query->fields($mc, array('menu_name', 'title'));
  $query->fields('dmc', array('copy_by_default'));
  $items = $query
    ->condition('dmc.copy_source', $menu_name)
    ->orderBy("$mc.title")
    ->execute()
    ->fetchAll();
  foreach ($items as $item) {
    $menus[$item->menu_name] = $item;
  }
  // Add other menu's copied from the same source.
  $query = db_select('dominion_menu', 'dm');
  $dmc = $query->join('dominion_menu_config', 'dmc', 'dm.cid = dmc.cid');
  $dm2 = $query->join('dominion_menu', 'dm2', 'dm.cid = dm2.cid');
  $mc = $query->join('menu_custom', 'mc', "$dm2.menu_name = mc.menu_name");
  $query->fields($mc, array('menu_name', 'title'));
  $query->fields('dmc', array('copy_by_default'));
  $items = $query->condition("dm.menu_name", $menu_name)
    ->condition("$dm2.menu_name", $menu_name, '!=')
    ->condition("$dmc.inherit", 1)
    ->orderBy("$mc.title")
    ->execute()
    ->fetchAll();
  foreach ($items as $item) {
    $menus[$item->menu_name] = $item;
  }
  return $menus;
}

/**
 * Get the mlid of the copied menu item. This is the menu item in the given menu
 * with the same path and title as the given item.
 * 
 * @param array $item
 * @param string $menu
 */
function _dominion_menu_get_copied_item($item, $menu) {
  $mlid = db_select('menu_links', 'ml')
    ->fields('ml', array('mlid'))
    ->condition('ml.menu_name', $menu)
    ->condition('ml.link_path', $item['link_path'])
    ->condition('ml.link_title', $item['link_title'])
    ->range(0, 1)
    ->execute()
    ->fetchField();
  return $mlid;
}
