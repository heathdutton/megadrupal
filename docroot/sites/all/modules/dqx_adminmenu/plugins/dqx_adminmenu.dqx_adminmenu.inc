<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function dqx_adminmenu_dqx_adminmenu_items($api) {

  foreach (_dqx_adminmenu_cache_types() as $slug => $title) {
    $api->addTree('admin/dqx-adminmenu/flush-cache/' . $slug);
  }

  $api->forceShowItem('admin/structure/block/list');

  $items = array();
  $items += _dqx_adminmenu_sibling_items();
  $items += _dqx_adminmenu_icon_items();
  $items['dqx-adminmenu-user'] = $items['admin/dqx-adminmenu'];

  return $items;
}


/**
 * Implements hook_dqx_adminmenu_items_alter()
 */
function dqx_adminmenu_dqx_adminmenu_items_alter($api) {

  // change weights and titles
  $api->itemSetWeight('admin/build', -8);
  $api->itemSetTitle('admin/build', t('Build'));
  $api->itemSetTitle('admin/content', t('Content'));
  $api->itemSetTitle('admin/settings', t('Configure'));
  $api->itemSetTitle('admin/user', t('People'));

  // Views. We don't want yet another separate plugin, so we do it here.
  $api->parentMap('admin/structure/views/view', 'admin/structure/views/list');
  $api->submenuSetInlineChild('admin/structure/views', 'admin/structure/views/list');

  $api->submenuSetInlineChild('admin/structure/block', 'admin/structure/block/list');
}


/**
 * Implements hook_dqx_adminmenu_structure_alter()
 */
function dqx_adminmenu_dqx_adminmenu_structure_alter($api) {

  $api->submenuSetAttribute('admin', 'id', 'dqx_adminmenu-admin');
  $api->submenuSetAttribute('dqx-adminmenu-user', 'id', 'dqx_adminmenu-user');
  $api->submenuSetInlineChild('admin/dqx-adminmenu', 'admin/dqx-adminmenu/flush-cache');

  // Some more "inline child" stuff, that we don't want to make separate plugins for.
  foreach (array(
    'admin/structure/views/view',
  ) as $parent => $child) {
    if (is_numeric($parent)) {
      $fragments = explode('/', $child);
      $parent = implode('/', array_slice($fragments, 0, count($fragments) - 1));
    }
  }
}


function _dqx_adminmenu_sibling_items() {

  $items = array();

  // Items in siblings dropdown.
  $siblings = variable_get('dqx_adminmenu_sibling_sites', array());
  $siblings[url('admin')] = url('<front>', array('absolute' => TRUE));
  foreach ($siblings as $siblingAdminLink => $sibling) {
    $item = array();
    $item['link_path'] = $siblingAdminLink;
    $item['title'] = $sibling;
    $item['external'] = TRUE;
    $item['localized_options']['external'] = TRUE;
    $item['absolute'] = TRUE;
    $item['localized_options']['absolute'] = TRUE;
    $item['access'] = TRUE;
    $item['localized_options']['attributes']['class'] = 'dqx_adminmenu-link-variation';
    $items['dqx-adminmenu-user/siblings/x' . md5($sibling)] = $item;
  }

  // Parent item for siblings dropdown.
  $item = array();
  $item['link_path'] = 'admin/config/administration/dqx-adminmenu/settings';
  $item['title'] = t('Sibling sites');
  $item['item_attributes']['class'] = 'dqx-adminmenu-siblings';
  $items['dqx-adminmenu-user/siblings'] = $item;

  return $items;
}


/**
 * Build icon menu links; mostly containing maintenance helpers.
 *
 * @see theme_admin_menu_links()
 */
function _dqx_adminmenu_icon_items() {

  $items = array();
  $items['admin/dqx-adminmenu'] = _dqx_adminmenu_icon_item();

  // Add link to manually run cron.
  $items['admin/dqx-adminmenu/cron'] = array(
    'title' => t('Run cron'),
    'weight' => 50,
    'access' => user_access('administer site configuration'),
    'link_path' => 'admin/reports/status/run-cron',
  );

  // Add link to run update.php.
  $items['admin/dqx-adminmenu/update'] = array(
    'title' => t('Run updates'),
    'weight' => 50,
    'access' => ($GLOBALS['user']->uid == 1 || !empty($GLOBALS['update_free_access'])),
    'link_path' => base_path() . 'update.php',
    'localized_options' => array(
      'external' => TRUE,
    ),
  );

  // Add Devel module links.
  if (module_exists('devel')) {
    // Add variable editor.
    $items['admin/dqx-adminmenu/devel-variables'] = array(
      'title' => t('Variable editor'),
      'weight' => 20,
      'access' => user_access('access devel information'),
      'link_path' => 'devel/variable',
    );
  }

  return $items;
}


function _dqx_adminmenu_icon_item() {

  $icon_src = theme_get_setting('toggle_favicon') ? theme_get_setting('favicon') : base_path() . 'misc/favicon.ico';
  $icon_img = '<img id="dqx_adminmenu-icon-img" src="' . $icon_src . '" width="16" height="16" alt="' . t('Home') . '" />';

  return array(
    'title' => $icon_img,
    'access' => TRUE,  // everyone should see the front page
    'weight' => -100,
    'link_path' => '<front>',
    'localized_options' => array(
      'attributes' => array('id' => 'dqx_adminmenu-icon-link'),
      'html' => TRUE,
    ),
  );
}


