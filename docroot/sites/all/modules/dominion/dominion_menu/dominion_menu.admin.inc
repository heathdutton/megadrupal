<?php
/**
 * @file
 * Admin functions for dominion_menu.
 */

/**
 * Page callback for the admin overview.
 */
function dominion_menu_admin_page() {
  $header = array(t('Name'), t('Blueprints'), t('Operations'));
  $rows = array();
  $items = db_select('dominion_menu_config', 'c')
    ->fields('c', array('cid', 'name'))
    ->execute()
    ->fetchAll();
  foreach ($items as $item) {
    $edit = l(t('edit'), 'admin/structure/dominion/settings/menu/' . $item->cid . '/edit');
    $delete = l(t('delete'), 'admin/structure/dominion/settings/menu/' . $item->cid . '/delete');
    $query = db_select('dominion_menu_config_blueprint', 'mcb');
    $b = $query->join('dominion_blueprint', 'b', 'mcb.blueprint = b.blueprint');
    $blueprints = $query->fields($b, array('name'))
      ->condition('mcb.cid', $item->cid)
      ->orderBy("$b.is_default", 'desc')
      ->orderBy("$b.name", 'asc')
      ->execute()
      ->fetchCol();
    $rows[] = array(
      check_plain($item->name),
      $blueprints ? theme('item_list', array('items' => $blueprints)) : 'none',
      "$edit $delete",
    );
  }
  $output = array();
  $output['info'] = array(
    '#markup' => '<p>' . t('Menu configurations determine which menu\'s will be created when creating new subsites.') . '</p>',
  );
  if (!$rows) {
    $output['norows'] = array(
      '#markup' => '<p>' . t('There are no menu configurations defined yet.') . '</p>',
    );
  }
  else {
    $output['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  return $output;
}

/**
 * Menu configuration add / edit form.
 */
function dominion_menu_menu_form($form, &$form_state, $cid = NULL) {
  $form['#cid'] = $cid;
  
  if ($cid) {
    $config = db_select('dominion_menu_config', 'c')->fields('c')->condition('c.cid', $cid)->execute()->fetchObject();
    if (!$config) {
      drupal_not_found();
      exit;
    }
  }
  else {
    // Generate default name suggestion.
    $defaults = array('primary', 'secondary', 'tertiary', 'quaternary', 'quinary', 'senary', 'septenary', 'octonary', 'nonary', 'denary', 'extra');
    $count = db_select('dominion_menu_config', 'c')->countQuery()->execute()->fetchField();
    $default = $defaults[min($count, 10)];
    
    $config = new stdClass();
    $config->name = ucfirst($default) . ' menu';
    $config->name_template = "[domain:id]-$default";
    $config->description_template = t('!name !counter links', array('!name' => '[domain:name]', '!counter' => t($default)));
    $config->default_links = '<front>|' . t('Home');
    $config->copy_source = 'main-menu';
    $config->inherit = FALSE;
    $config->inherit_allow_override = FALSE;
    $config->copy_by_default = TRUE;
  }
  
  drupal_add_js(drupal_get_path('module', 'dominion_menu') . '/admin.js');
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $config->name,
    '#description' => t('Name for this menu. This is used as the menu block title. This title will be translated.'),
    '#required' => TRUE,
  );
  
  $form['name_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu machine name template'),
    '#default_value' => $config->name_template,
    '#description' => t('You can use domain tokens here.'),
    '#required' => TRUE,
  );
  
  $form['description_template'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu description template'),
    '#default_value' => $config->description_template,
    '#description' => t('You can use domain tokens here.'),
    '#required' => TRUE,
  );
  
  $form['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['token_help']['help'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('domain'),
    '#global_types' => FALSE,
    '#click_insert' => TRUE,
  );
  
  $form['inherit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inherit menu items'),
    '#collapsible' => TRUE,
    '#description' => t('Menu items can be inherited from parent subsites. Dominion looks for the parent in the directory structure, or will use the main domain if the subsite is not in a directory.'),
  );
  
  $form['inherit']['inherit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Inherit menu links from parent subsite'),
    '#description' => t('Inherit items by default.'),
    '#default_value' => $config->inherit,
  );
  
  $form['inherit']['inherit_allow_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow editors to override the inherit setting'),
    '#description' => t('This allows editors to choose if the menu should be inheritted when creating a subsite. The site administrator can always use this option.'),
    '#default_value' => $config->inherit_allow_override,
  );
  
  $form['inherit']['copy_by_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Copy links by default'),
    '#description' => t('Check all menu\'s by default when adding menu items.'),
    '#default_value' => $config->copy_by_default,
  );
  
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu links'),
    '#collapsible' => TRUE,
  );
  
  $options = array(
    'default' => t('Provide default menu'),
    'copy' => t('Copy menu links'),
  );
  $form['links']['initial_links'] = array(
    '#type' => 'radios',
    '#title' => t('Initial menu links'),
    '#options' => $options,
    '#default_value' => empty($config->default_links) ? 'copy' : 'default',
  );
  
  $form['links']['default_links'] = array(
    '#type' => 'textarea',
    '#title' => t('Default menu links'),
    '#default_value' => $config->default_links,
    '#description' => t('Default menu to create for new subsites. One item per line, each having a path, title and optional a required function name separated by pipes. You can use domain tokens here.'),
    '#wysiwyg' => FALSE,
  );
  
  $options = menu_get_menus();
  $form['links']['copy_source'] = array(
    '#type' => 'select',
    '#title' => t('Copy menu from'),
    '#options' => $options,
    '#default_value' => $config->copy_source,
  );
  
  $form['blueprints'] = array(
    '#type' => 'fieldset',
    '#title' => t('Blueprints'),
    '#collapsible' => TRUE,
  );
  $blueprints = db_select('dominion_blueprint', 'b')
    ->fields('b', array('blueprint', 'name'))
    ->orderBy('b.is_default', 'desc')
    ->orderBy('b.name', 'asc')
    ->execute()
    ->fetchAllKeyed();
  if ($cid) {
    $default = db_select('dominion_menu_config_blueprint', 'mcb')
      ->fields('mcb', array('blueprint'))
      ->condition('mcb.cid', $cid)
      ->execute()
      ->fetchCol();
  }
  else {
    $default = db_select('dominion_blueprint', 'b')
      ->fields('b', array('blueprint'))
      ->condition('b.is_default', 1)
      ->execute()
      ->fetchCol();
  }
  $form['blueprints']['blueprints'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Blueprints'),
    '#options' => $blueprints,
    '#default_value' => drupal_map_assoc($default),
    '#description' => t('Check the blueprints on which this menu should be enabled.'),
  );
  
  if (module_exists('submenutree')) {
    $form['submenutree'] = array(
      '#type' => 'fieldset',
      '#title' => t('Submenutree'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $submenutree = !empty($cid) && ($cid == variable_get('dominion_menu_submenutree', FALSE));
    $form['submenutree']['submenutree'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use this menu for submenutree'),
      '#default_value' => $submenutree,
      '#description' => t('Submenutree provides a block with extended menu links from a fixed menu. Dominion is able to override this setting per subsite to allow the use the subsite menu.')
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for the menu configuration form.
 */
function dominion_menu_menu_form_submit(&$form, &$form_state) {
  $cid = $form['#cid'];
  
  $config = new stdClass();
  $config->name = $form_state['values']['name'];
  $config->name_template = $form_state['values']['name_template'];
  $config->description_template = $form_state['values']['description_template'];
  switch ($form_state['values']['initial_links']) {
    case 'default':
      $config->default_links = $form_state['values']['default_links'];
      break;
    case 'copy':
      $config->copy_source = $form_state['values']['copy_source'];
      break;
  }
  $config->inherit = !empty($form_state['values']['inherit']);
  $config->inherit_allow_override = !empty($form_state['values']['inherit_allow_override']);
  $config->copy_by_default = !empty($form_state['values']['copy_by_default']);
  
  if ($cid) {
    $config->cid = $cid;
    drupal_write_record('dominion_menu_config', $config, array('cid'));
    db_delete('dominion_menu_config_blueprint')->condition('cid', $cid)->execute();
  }
  else {
    drupal_write_record('dominion_menu_config', $config);
  }
  
  foreach ($form_state['values']['blueprints'] as $blueprint => $checked) {
    if ($checked) {
      db_insert('dominion_menu_config_blueprint')->fields(array(
        'cid' => $config->cid,
        'blueprint' => $blueprint,
      ))->execute();
    }
  }
  
  if (!empty($form_state['values']['submenutree'])) {
    variable_set('dominion_menu_submenutree', $config->cid);
  }
  
  drupal_set_message(t('The menu configuration has been saved.'));
  $form_state['redirect'] = 'admin/structure/dominion/settings/menu';
}

/**
 * Menu configuration delete form.
 */
function dominion_menu_menu_delete_form($form, &$form_state, $cid) {
  $config = db_select('dominion_menu_config', 'c')->fields('c')->condition('c.cid', $cid)->execute()->fetchObject();
  if (!$config) {
    drupal_not_found();
    exit;
  }
  return confirm_form(
    array('#cid' => $cid),
    t('Are you sure you want to delete the menu configuration %name?', array('%name' => $config->name)),
    'admin/structure/dominion/settings/menu',
    NULL,
    t('Delete')
  );
}

/**
 * Submit handler for menu configuration delete form.
 */
function dominion_menu_menu_delete_form_submit(&$form, &$form_state) {
  db_delete('dominion_menu_config')->condition('cid', $form['#cid'])->execute();
  db_delete('dominion_menu_config_blueprint')->condition('cid', $form['#cid'])->execute();
  drupal_set_message(t('The menu configuration has been deleted.'));
  $form_state['redirect'] = 'admin/structure/dominion/settings/menu';
}

/**
 * Menu callback which shows an overview page of all the custom menus and their descriptions.
 */
function dominion_menu_overview_page() {
  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  $header = array(t('Title'), array('data' => t('Operations'), 'colspan' => user_access('administer menu') ? 3 : 2));
  $rows = array();
  foreach ($result as $menu) {
    if (dominion_menu_menu_access($menu)) {
      $row = array(theme('menu_admin_overview', array('title' => $menu['title'], 'name' => $menu['menu_name'], 'description' => $menu['description'])));
      $row[] = array('data' => l(t('list links'), 'admin/structure/menu/manage/' . $menu['menu_name']));
      if (user_access('administer menu')) {
        $row[] = array('data' => l(t('edit menu'), 'admin/structure/menu/manage/' . $menu['menu_name'] . '/edit'));
      }
      $row[] = array('data' => l(t('add link'), 'admin/structure/menu/manage/' . $menu['menu_name'] . '/add'));
      $rows[] = $row;
    }
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Menu overview form.
 * 
 * This is a copy of menu_overview_form() without access check. All links needs
 * to be visible in the menu admin for people with access permissions to this
 * page, even without the 'administer menu' permission.
 */
function dominion_menu_overview_form($form, &$form_state, $menu) {
  global $menu_admin;
  module_load_include('inc', 'menu', 'menu');
  module_load_include('inc', 'menu', 'menu.admin');
  $form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');
  $sql = "
    SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
    FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
    WHERE ml.menu_name = :menu
    ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(':menu' => $menu['menu_name']), array('fetch' => PDO::FETCH_ASSOC));
  $links = array();
  foreach ($result as $item) {
    $item['access'] = TRUE;
    $links[] = $item;
  }
  $tree = menu_tree_data($links);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
  // We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;

  $form = array_merge($form, _menu_overview_tree_form($tree));
  $form['#menu'] =  $menu;

  if (element_children($form)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    );
  }
  else {
    $form['#empty_text'] = t('There are no menu links yet. <a href="@link">Add link</a>.', array('@link' => url('admin/structure/menu/manage/'. $form['#menu']['menu_name'] .'/add')));
  }
  $form['#theme'] = 'menu_overview_form';
  return $form;
}

/**
 * Menu overview form submit handler.
 */
function dominion_menu_overview_form_submit($form, &$form_state) {
  module_load_include('inc', 'menu', 'menu.admin');
  menu_overview_form_submit($form, $form_state);
}
