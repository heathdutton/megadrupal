<?php

/**
 * @file
 * TODO.
 */

/**
 * Menu callback which shows an overview page of all the custom menus.
 */
function gcc_menu_overview_page($entity_type, $entity, $base_path) {

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $menus = isset($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE]) ? $entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE] : array();
  $header = array(

    t('Title'),
    array(
      'data' => t('Operations'),
      'colspan' => '3',
    ),
  );

  $rows = array();
  foreach ($menus as $menu) {

    $row = array(theme('menu_admin_overview', array(

      'title' => $menu['title'],
      'name' => $menu['menu_name'],
      'description' => $menu['description'],
    )));
    $row[] = array('data' => l(t('list links'), $base_path . '/gcc/menu/' . $menu['menu_name']));
    $row[] = array('data' => l(t('edit menu'), $base_path . '/gcc/menu/' . $menu['menu_name'] . '/edit'));
    $row[] = array('data' => l(t('add link'), $base_path . '/gcc/menu/' . $menu['menu_name'] . '/add'));
    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * TODO.
 */
function gcc_menu_overview_form($entity_type, $entity, $base_path, $menu) {

  module_load_include('inc', 'menu', 'menu.admin');

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form = drupal_get_form('menu_overview_form', $menu);
  foreach (element_children($form) as $key) {

    if (!isset($form[$key]['#item'])) {
      continue;
    }

    $mlid = $form[$key]['#item']['mlid'];

    $form[$key]['operations']['edit']['#href'] = $base_path . '/gcc/menu/item/' . $mlid . '/edit';
    $form[$key]['operations']['delete']['#href'] = $base_path . '/gcc/menu/item/' . $mlid . '/delete';
  }

  $form['#empty_text'] = t('There are no menu links yet. <a href="@link">Add link</a>.', array('@link' => url($base_path . '/gcc/menu/' . $menu['menu_name'] . '/add')));

  return $form;
}

/**
 * Menu callback; Build the form that handles the adding/editing of a menu.
 */
function gcc_menu_edit_menu($form, &$form_state, $op, $entity_type, $entity, $base_path, $menu = array()) {

  $menu += array(

    'menu_name' => '',
    'title' => '',
    'description' => '',
    'default_menu' => 0,
  );

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;
  $form['#insert'] = $op == 'add';
  $form['#menu'] = $menu;

  $form['title'] = array(

    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $menu['title'],
    '#required' => TRUE,
  );

  $form['menu_name'] = array(

    '#type' => 'machine_name',
    '#title' => t('Menu name'),
    '#default_value' => $menu['menu_name'],
    '#maxlength' => 128,
    '#description' => t('A unique name to construct the URL for the menu. It must only contain lowercase letters, numbers and hyphens.'),
    '#machine_name' => array(
      'exists' => 'gcc_menu_edit_menu_name_exists',
      'source' => array('title'),
      'replace_pattern' => '[^a-z0-9-]+',
      'replace' => '-',
    ),
    // A menu's machine name cannot be changed.
    '#disabled' => !empty($menu['menu_name']),
    '#entity_type' => $entity_type,
    '#entity_id' => $entity_id,
  );

  $form['description'] = array(

    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $menu['description'],
  );

  $form['default_menu'] = array(

    '#type' => 'checkbox',
    '#title' => t('Default menu of the group'),
    '#description' => t('Make this menu the default menu of the group'),
    '#default_value' => $menu['default_menu'],
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Save'),
  );

  // Only existing menu can be deleted.
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#access' => $op == 'edit',
    '#submit' => array('gcc_menu_delete_submit'),
  );

  return $form;
}

/**
 * Returns whether a menu name already exists.
 */
function gcc_menu_edit_menu_name_exists($value, $element, $form_state) {

  $value = 'gcc-menu-' . $element['#entity_type'] . '-' . $element['#entity_id'] . '-' . $value;

  $query = new EntityFieldQuery();
  $query->fieldCondition(GCC_MENU_FIELD_MENU, 'menu_name', $value);
  $query->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');
  $query->range(0, 1);
  $query->count();

  $menu_exists = $query->execute();
  $link_exists = db_query_range("SELECT 1 FROM {menu_links} WHERE menu_name = :menu", 0, 1, array(':menu' => $value))->fetchField();

  return $menu_exists || $link_exists;
}

/**
 * Submit handler.
 */
function gcc_menu_edit_menu_submit($form, &$form_state) {

  drupal_set_message(t('Your configuration has been saved.'));

  if ($form['#insert']) {
    $menu_name = 'gcc-menu-' . $form['#entity_type'] . '-' . $form['#entity_id'] . '-' . $form_state['values']['menu_name'];
  }
  else {
    $menu_name = $form_state['values']['menu_name'];
  }

  $form_state['redirect'] = $form['#base_path'] . '/gcc/menu/' . $menu_name;

  $entity = $form['#entity'];

  if ($form['#insert']) {
    $entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE][] = array(

      'menu_name' => $menu_name,
      'title' => $form_state['values']['title'],
      'description' => $form_state['values']['description'],
      'default_menu' => $form_state['values']['default_menu'],
    );
  }
  else {
    foreach ($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE] as $delta => $item) {
      if ($item['menu_name'] == $menu_name) {

        $entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE][$delta]['title'] = $form_state['values']['title'];
        $entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE][$delta]['description'] = $form_state['values']['description'];
        $entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE][$delta]['default_menu'] = $form_state['values']['default_menu'];
      }
    }
  }

  if ($form_state['values']['default_menu']) {
    foreach ($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE] as $delta => $item) {
      if ($item['menu_name'] != $menu_name) {
        $entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE][$delta]['default_menu'] = 0;
      }
    }
  }

  field_attach_presave($form['#entity_type'], $entity);
  field_attach_update($form['#entity_type'], $entity);
}

/**
 * Submit handler.
 */
function gcc_menu_delete_submit($form, &$form_state) {

  $form_state['redirect'] = $form['#base_path'] . '/gcc/menu/' . $form['#menu']['menu_name'] . '/delete';
}

/**
 * TODO.
 */
function gcc_menu_delete_menu($form, &$form_state, $entity_type, $entity, $base_path, $menu) {

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;
  $form['#menu'] = $menu;

  $caption = '';
  $num_links = db_query("SELECT COUNT(*) FROM {menu_links} WHERE menu_name = :menu", array(':menu' => $menu['menu_name']))->fetchField();

  if ($num_links) {
    $caption .= '<p>' . format_plural($num_links, '<strong>Warning:</strong> There is currently 1 menu link in %title. It will be deleted (system-defined items will be reset).', '<strong>Warning:</strong> There are currently @count menu links in %title. They will be deleted (system-defined links will be reset).', array('%title' => $menu['title'])) . '</p>';
  }
  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, t('Are you sure you want to delete the custom menu %title?', array('%title' => $menu['title'])), $base_path . '/gcc/menu', $caption, t('Delete'));
}

/**
 * TODO.
 */
function gcc_menu_delete_menu_submit($form, &$form_state) {

  $entity = $form['#entity'];
  foreach ($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE] as $delta => $item) {
    if ($item['menu_name'] == $form['#menu']['menu_name']) {
      unset($entity->{GCC_MENU_FIELD_MENU}[LANGUAGE_NONE][$delta]);
    }
  }

  field_attach_presave($form['#entity_type'], $entity);
  field_attach_update($form['#entity_type'], $entity);

  menu_delete($form['#menu']);

  $t_args = array('%title' => $form['#menu']['title']);
  drupal_set_message(t('The custom menu %title has been deleted.', $t_args));

  $form_state['redirect'] = $form['#base_path'] . '/gcc/menu';
}

/**
 * TODO.
 */
function gcc_menu_edit_item($form, &$form_state, $op, $entity_type, $entity, $base_path, $item, $menu) {

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;
  $form['#menu'] = $menu;

  if ($op == 'add' || empty($item)) {
    // This is an add form, initialize the menu link.
    $item = array(
      'link_title' => '',
      'mlid' => 0,
      'plid' => 0,
      'menu_name' => $menu['menu_name'],
      'weight' => 0,
      'link_path' => '',
      'options' => array(),
      'module' => 'menu',
      'expanded' => 0,
      'hidden' => 0,
      'has_children' => 0,
    );
  }
  else {
    // Get the human-readable menu title from the given menu name.
    $menu = gcc_menu_load($item['menu_name']);
    $current_title = $menu['title'];

    // Get the current breadcrumb and add a link to that menu's overview page.
    $breadcrumb = menu_get_active_breadcrumb();
    $breadcrumb[] = l($current_title, $form['#base_path'] . '/gcc/menu/' . $item['menu_name']);
    drupal_set_breadcrumb($breadcrumb);
  }

  $form['actions'] = array('#type' => 'actions');

  $form['link_title'] = array(

    '#type' => 'textfield',
    '#title' => t('Menu link title'),
    '#default_value' => $item['link_title'],
    '#description' => t('The text to be used for this link in the menu.'),
    '#required' => TRUE,
  );

  foreach (array(
    'link_path',
    'mlid',
    'module',
    'has_children',
    'options',
  ) as $key) {
    $form[$key] = array(
      '#type' => 'value',
      '#value' => $item[$key],
    );
  }
  // Any item created or edited via this interface is considered "customized".
  $form['customized'] = array(

    '#type' => 'value',
    '#value' => 1,
  );
  $form['original_item'] = array(

    '#type' => 'value',
    '#value' => $item,
  );

  $path = $item['link_path'];
  if (isset($item['options']['query'])) {
    $path .= '?' . drupal_http_build_query($item['options']['query']);
  }
  if (isset($item['options']['fragment'])) {
    $path .= '#' . $item['options']['fragment'];
  }
  if ($item['module'] == 'menu') {

    $form['link_path'] = array(

      '#type' => 'textfield',
      '#title' => t('Path'),
      '#maxlength' => 255,
      '#default_value' => $path,
      '#description' => t('The path for this menu link. This can be an internal Drupal path such as %add-node or an external URL such as %drupal. Enter %front to link to the front page.', array(
        '%front' => '<front>',
        '%add-node' => 'node/add', '%drupal' => 'http://drupal.org',
      )),
      '#required' => TRUE,
    );
    $form['actions']['delete'] = array(

      '#type' => 'submit',
      '#value' => t('Delete'),
      '#access' => $item['mlid'],
      '#submit' => array('gcc_menu_item_delete_submit'),
      '#weight' => 10,
    );
  }
  else {

    $form['_path'] = array(

      '#type' => 'item',
      '#title' => t('Path'),
      '#description' => l($item['link_title'], $item['href'], $item['options']),
    );
  }

  $form['description'] = array(

    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($item['options']['attributes']['title']) ? $item['options']['attributes']['title'] : '',
    '#rows' => 1,
    '#description' => t('Shown when hovering over the menu link.'),
  );

  $form['enabled'] = array(

    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => !$item['hidden'],
    '#description' => t('Menu links that are not enabled will not be listed in any menu.'),
  );

  $form['expanded'] = array(

    '#type' => 'checkbox',
    '#title' => t('Show as expanded'),
    '#default_value' => $item['expanded'],
    '#description' => t('If selected and this menu link has children, the menu will always appear expanded.'),
  );

  // Generate a list of possible parents.
  $options = menu_parent_options(array($item['menu_name'] => t($menu['title'])), $item);
  $default = $item['menu_name'] . ':' . $item['plid'];
  if (!isset($options[$default])) {
    $default = $item['menu_name'] . ':0';
  }

  $form['parent'] = array(

    '#type' => 'select',
    '#title' => t('Parent link'),
    '#default_value' => $default,
    '#options' => $options,
    '#description' => t('The maximum depth for a link and all its children is fixed at !maxdepth. Some menu links may not be available as parents if selecting them would exceed this limit.', array('!maxdepth' => MENU_MAX_DEPTH)),
    '#attributes' => array('class' => array('menu-title-select')),
  );

  $form['weight'] = array(

    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 50,
    '#default_value' => $item['weight'],
    '#description' => t('Optional. In the menu, the heavier links will sink and the lighter links will be positioned nearer the top.'),
  );

  $form['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * This function forward the form alter to the classic menu item edit form.
 */
function gcc_form_gcc_menu_edit_item_alter(&$form, &$form_state, $form_id) {

  $form_id = 'menu_edit_item';

  $hooks = array('form');
  $hooks[] = 'form_' . $form_id;

  drupal_alter($hooks, $form, $form_state, $form_id);
}

/**
 * TODO.
 */
function gcc_menu_edit_item_validate($form, &$form_state) {

  $item = &$form_state['values'];
  $normal_path = drupal_get_normal_path($item['link_path']);
  if ($item['link_path'] != $normal_path) {

    drupal_set_message(t('The menu system stores system paths only, but will use the URL alias for display. %link_path has been stored as %normal_path', array('%link_path' => $item['link_path'], '%normal_path' => $normal_path)));
    $item['link_path'] = $normal_path;
  }
  if (!url_is_external($item['link_path'])) {

    $parsed_link = parse_url($item['link_path']);
    if (isset($parsed_link['query'])) {
      $item['options']['query'] = drupal_get_query_array($parsed_link['query']);
    }
    else {
      // Use unset() rather than setting to empty string
      // to avoid redundant serialized data being stored.
      unset($item['options']['query']);
    }
    if (isset($parsed_link['fragment'])) {
      $item['options']['fragment'] = $parsed_link['fragment'];
    }
    else {
      unset($item['options']['fragment']);
    }
    if (isset($parsed_link['path']) && $item['link_path'] != $parsed_link['path']) {
      $item['link_path'] = $parsed_link['path'];
    }
  }
  if (!trim($item['link_path']) || !drupal_valid_path($item['link_path'], TRUE)) {
    form_set_error('link_path', t("The path '@link_path' is either invalid or you do not have access to it.", array('@link_path' => $item['link_path'])));
  }
}

/**
 * TODO.
 */
function gcc_menu_edit_item_submit($form, &$form_state) {

  $item = &$form_state['values'];

  // The value of "hidden" is the opposite of the value
  // supplied by the "enabled" checkbox.
  $item['hidden'] = (int) !$item['enabled'];
  unset($item['enabled']);

  $item['options']['attributes']['title'] = $item['description'];
  list($item['menu_name'], $item['plid']) = explode(':', $item['parent']);

  if (!menu_link_save($item)) {
    drupal_set_message(t('There was an error saving the menu link.'), 'error');
  }
  else {
    drupal_set_message(t('Your configuration has been saved.'));
  }

  $form_state['redirect'] = $form['#base_path'] . '/gcc/menu/' . $item['menu_name'];
}

/**
 * Submit handler.
 */
function gcc_menu_item_delete_submit($form, &$form_state) {

  $form_state['redirect'] = $form['#base_path'] . '/gcc/menu/item/' . $form_state['values']['mlid'] . '/delete';
}

/**
 * TODO.
 */
function gcc_menu_delete_item($form, &$form_state, $entity_type, $entity, $base_path, $item) {

  $entity_id = gcc_get_entity_id($entity_type, $entity);
  $base_path = gcc_translate_path($entity_type, $entity_id, $base_path);

  $form['#entity_type'] = $entity_type;
  $form['#entity_id'] = $entity_id;
  $form['#entity'] = $entity;
  $form['#base_path'] = $base_path;
  $form['#item'] = $item;

  return confirm_form($form, t('Are you sure you want to delete the custom menu link %item?', array('%item' => $item['link_title'])), $form['#base_path'] . '/gcc/menu/' . $item['menu_name']);
}

/**
 * TODO.
 */
function gcc_menu_delete_item_submit($form, &$form_state) {

  $item = $form['#item'];
  menu_link_delete($item['mlid']);

  $t_args = array('%title' => $item['link_title']);
  drupal_set_message(t('The menu link %title has been deleted.', $t_args));
  watchdog('menu', 'Deleted menu link %title.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = $form['#base_path'] . '/gcc/menu/' . $item['menu_name'];
}
