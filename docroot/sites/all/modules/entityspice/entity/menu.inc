<?php

/**
 * Wrapper for devel_load_object which is in an include file.
 */
function _entityspice_devel_load_object($etype, $object, $name = NULL) {
  $object = _entityspice_get_entity($etype, $object);
  if (module_exists('devel')) {
    module_load_include('inc', 'devel', 'devel.pages');
    return devel_load_object($etype, $object, $name);
  }
  else {
    return drupal_not_found();
  }
}

/**
 * Wrapper function for devel_render_object which is in an include file.
 */
function _entityspice_devel_render_object($etype, $object, $name = NULL) {
  $object = _entityspice_get_entity($etype, $object);
  if (module_exists('devel')) {
    module_load_include('inc', 'devel', 'devel.pages');
    return devel_render_object($etype, $object, $name);
  }
  else {
    return drupal_not_found();
  }
}

// ___________________________________________________________________________

/**
 * Implements hook_menu().
 *
 * Build menu items for module defining an entity.
 */
function _entityspice_entity_menu_items($etype) {
  $info = _entityspice_getinfo($etype);

  $label = $info['label'];
  $parent_path_u = $info['parent userland path'];

  $edit_form   = $info['edit form'];
  $delete_form = $info['delete form'];

  $items[$parent_path_u] = array(
    'title' => $label,
    'description' => 'View ' . $label . 's',
    'page callback' => '_entityspice_entity_page',
    'page arguments' => array($etype),
    'access arguments' => array("administer $etype"),
  );

  $items[$parent_path_u . '/add'] = array(
    'title' => 'Add a ' . $label,
    'description' => 'Add a new ' . $label,
    'page callback' => '_entityspice_entity_bundles_list_page',
    'page arguments' => array($etype),
    'type' => MENU_NORMAL_ITEM,
    'access arguments' => array('add ' . $etype),
  );

  $entity_info = entity_get_info($etype);
  foreach ($entity_info['bundles'] as $bun_name => $bundle) {
    $items[$parent_path_u . '/add/' . $bun_name] = array(
      'title' => $bundle['label'],
      'title callback' => 'check_plain',
      'description' => isset($bundle['description']) ? $bundle['description'] : '',
      'page callback' => '_entityspice_entity_add_page',
      'page arguments' => array($etype, $bun_name),
      'access arguments' => array('add ' . $etype . ' : ' . $bun_name),
    );
  }

  $argno = $info['parent user path arg no'];
  $access_cbk = $etype . '_entity_access';
  $items[$parent_path_u . '/%'] = array(
    'title callback' => '_entityspice_entity_title',
    'title arguments' => array($etype, $argno),
    'page callback' => '_entityspice_entity_view',
    'page arguments' => array($etype, $argno),
    'weight' => 10,
    'access callback' => $access_cbk,
    'access arguments' => array('view', $argno),
  );

  $items[$parent_path_u . '/%/view'] = array(
    'title' => 'Summary',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
    'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    'access callback' => $access_cbk,
    'access arguments' => array('view', $argno),
  );

  $items[$parent_path_u . '/%/view/edit'] = array(
    'title' => 'Edit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array($edit_form, $argno),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
    'access callback' => $access_cbk,
    'access arguments' => array('edit', $argno),
  );

  $items[$parent_path_u . '/%/view/delete'] = array(
    'title' => 'Delete',
    'page callback' => 'drupal_get_form',
    'page arguments' => array($delete_form, $argno),
    'type' => MENU_LOCAL_TASK,
    'weight' => 5,
    'access callback' => $access_cbk,
    'access arguments' => array('delete', $argno),
  );

  if (module_exists('devel')) {
    $items[$parent_path_u . '/%/devel'] = array(
      'title' => 'Devel',
      'page callback' => '_entityspice_devel_load_object',
      'page arguments' => array($etype, $argno),
      'type' => MENU_LOCAL_TASK,
      'weight' => 100,
      'access arguments' => array('access devel information'),
    );

    $items[$parent_path_u . '/%/devel/load'] = array(
      'title' => 'Load',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'access arguments' => array('access devel information'),
    );

    $items[$parent_path_u . '/%/devel/render'] = array(
      'title' => 'Render',
      'page callback' => '_entityspice_devel_render_object',
      'page arguments' => array($etype, $argno),
      'type' => MENU_LOCAL_TASK,
      'weight' => 100,
      'access arguments' => array('access devel information'),
    );
  }

  return $items;
}

/**
 * Why did I put this here? what does these comments mean? :|
 *
 * Implements hook_menu_local_tasks_alter().
 *
 * Changing the BASEPATH/ENTITY/add menu item to type MENU_LOCAL_ACTION would
 * break display of child items. Instead, dynamically add the local action.
 *
 * Adds action link to 'xxxx/yyyy/add' on 'xxxx/yyyyy' page.
 */
function _entityspice_menu_local_task_alter($etype, &$data, $router_item, $root_path) {
  if ($path = _entityspice_getinfo($etype, 'parent admin path') == $root_path) {
    $item = menu_get_item($path . '/add');
    if ($item['access']) {
      $data['actions']['output'][] = array(
        '#theme' => 'menu_local_action',
        '#link' => $item,
      );
    }
  }
}

/**
 * Implements hook_admin_menu_map().
 */
function _entityspice_admin_menu_map($etype) {
  $i = _entityspice_getinfo($etype, '_entityspice_entity_info_alter');
  $path = $i['path'];
  $name = '%' . $i['name'];

  $map[$path]['parent'] = $i['parent'];
  $map[$path]['arguments'][][$name] =
    _entityspice_entity_get_bundles_names($etype, FALSE);

  return $map;
}

/**
 * Makes sure entity exists.
 *
 * Is called in menu callbacks, like entity delete form or entity view page, to
 * make sure asked entity actually exists.
 */
function _entityspice_entity_in_path_exists($etype, $entity) {
  $id = is_object($entity) ? $entity->getID() : $entity;
  return ctype_digit($id)  && entity_load($etype, array($id));
}
