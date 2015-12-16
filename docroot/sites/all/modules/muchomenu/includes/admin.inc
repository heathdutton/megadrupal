<?php

// $Id: muchomenu.admin.inc,v 1.1.2.6 2010/09/28 21:33:00 pobster Exp $

/**
 * @file
 * Administrative page callbaks for muchomenu module.
 */

/**
 * Menu callback which shows an overview page of all the custom muchomenus and their descriptions.
 */
function muchomenu_overview_page() {
  $sql = "SELECT id, title, description FROM {muchomenu_menus} ORDER BY title;";
  $result = db_query($sql);
  $content = array();
  foreach ($result as $menu) {
    $menu->href = 'admin/structure/menu/muchomenu/muchomenu-customize/' . $menu->id;
    $menu->localized_options = array();
    $menu->description = filter_xss_admin($menu->description);
    $content[] = (array) $menu;
  }
  if (empty($content)) {
    $content[] = array(
        'title' => 'Add a MuchoMenu',
        'href' => 'admin/structure/menu/muchomenu/add',
        'description' => '<strong>' . t('There are no MuchoMenus yet.') .'</strong>',
        'localized_options' => array()
    );
  }
  return theme('admin_block_content', array('content' => $content));
}

/**
 * Title callback for the menu overview page and links.
 */
function muchomenu_overview_title($menu) {
  return check_plain($menu['title']);
}

/**
 * Form for editing an entire muchomenu tree at once.
 *
 * Shows for one menu the muchomenu items and relevant operations.
 */
function muchomenu_overview_form($form, &$form_state, $menu = array()) {
  $tree = muchomenu_tree_data($menu['id']);
  $form = array_merge($form, _muchomenu_overview_tree_form($tree));
  $form['#menu'] = $menu;


  if (element_children($form)) {
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration'),
    );
  }
  else {
    $form['empty_menu'] = array(
      '#markup' => t('There are no MuchoMenu items yet. <br />!addlink', array(
        '!addlink' => l('Add a MuchoMenu item', 'admin/structure/menu/muchomenu/muchomenu-customize/' . $menu['id'] . '/add'),
      )),
    );
  }

  drupal_set_title($menu['title']);
  return $form;
}

/**
 * Recursive helper function for muchomenu_overview_form().
 */
function _muchomenu_overview_tree_form($tree) {
  static $form = array('#tree' => TRUE);

  foreach ($tree as $data) {
    $title = '';
    $item = $data['link'];
    if ($item) {
      $id = 'id:' . $item['id'];
      $form[$id]['#item'] = $item;
      $form[$id]['#attributes']['class'] = (!$item['enabled']) ? array('menu-disabled') : array('menu-enabled');
      $form[$id]['title']['#markup'] = l($item['title'], $item['path']) . (!$item['enabled'] ? ' (' . t('disabled') . ')' : '');
      $form[$id]['enabled'] = array(
          '#type' => 'checkbox',
          '#default_value' => $item['enabled'],
      );

      $form[$id]['target'] = array(
          '#type' => 'checkbox',
          '#default_value' => $data['link']['target'],
      );
      $form[$id]['disable_click'] = array(
          '#type' => 'checkbox',
          '#default_value' => $data['link']['disable_click'],
      );
      $form[$id]['weight'] = array(
          '#type' => 'weight',
          '#delta' => 50,
          '#default_value' => isset($form_state[$id]['weight']) ? $form_state[$id]['weight'] : $item['weight'],
      );
      $form[$id]['id'] = array(
          '#type' => 'hidden',
          '#value' => $item['id'],
      );
      $form[$id]['menu_id'] = array(
          '#type' => 'hidden',
          '#default_value' => isset($form_state[$id]['menu_id']) ? $form_state[$id]['menu_id'] : $item['menu_id'],
          '#size' => 6,
      );
      // Build a list of operations.
      $operations = array();
      $operations['edit'] = l(t('edit'), 'admin/structure/menu/muchomenu/item/' . $item['id'] . '/edit');
      $operations['delete'] = l(t('delete'), 'admin/structure/menu/muchomenu/item/' . $item['id'] . '/delete');

      $form[$id]['operations'] = array();
      foreach ($operations as $op => $value) {
        $form[$id]['operations'][$op] = array('#markup' => $value);
      }
    }

    if ($data['below']) {
      _muchomenu_overview_tree_form($data['below']);
    }
  }
  return $form;
}

/**
 * Submit handler for the menu overview form.
 */
function muchomenu_overview_form_submit($form, &$form_state) {
  $updated_items = array();
  $fields = array('enabled', 'type', 'weight', 'menu_id', 'target', 'disable_click');
  foreach ($form_state['values'] as $id => $value) {
    if (isset($form[$id]['#item'])) {
      $element = $form[$id];
      // Update any fields that have changed in this menu item.
      foreach ($fields as $field) {
        if ($element[$field]['#value'] != $element[$field]['#default_value']) {
          $element['#item'][$field] = $element[$field]['#value'];
          $updated_items[$id] = $element['#item'];
        }
      }
    }
  }

  // Save all our changed items to the database.
  foreach ($updated_items as $item) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("
     *    UPDATE {muchomenu_items} SET 
     *    menu_id = %d, enabled = %d, weight = %d, target = %d, disable_click = %d
     *    WHERE id = %d
     * ",
     *  $item['menu_id'], $item['enabled'], $item['weight'],
     *  $item['target'], $item['disable_click'], $item['id'])
     */
    db_update('muchomenu_items')
            ->fields(array(
                'menu_id' => $item['menu_id'],
                'enabled' => $item['enabled'],
                'weight' => $item['weight'],
                'target' => $item['target'],
                'disable_click' => $item['disable_click'],
            ))
            ->condition('id', $item['id'])
            ->execute();
  }
}

/**
 * Menu callback; Build the form that handles the adding/editing of a custom menu.
 */
function muchomenu_edit_menu($form, &$form_state, $type, $menu = array()) {
  if ($type == 'edit') {
    $form['id'] = array('#type' => 'value', '#value' => $menu['id']);
    $form['#insert'] = FALSE;
    $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#submit' => array('muchomenu_delete_menu_submit'),
        '#weight' => 10,
    );
  }
  else {
    $menu = array('title' => '', 'description' => '', 'orientation' => 'horizontal');
    $form['#insert'] = TRUE;
  }
  $form['#title'] = isset($menu['title']) ? $menu['title'] : t('MuchoMenu');
  $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => isset($menu['title']) ? $menu['title'] : '',
      '#required' => TRUE,
  );
  $form['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => isset($menu['description']) ? $menu['description'] : '',
  );
  $form['orientation'] = array(
      '#type' => 'radios',
      '#title' => t('Menu Layout'),
      '#options' => array(
          'horizontal' => t('Horizontal'),
          'vertical' => t('Vertical'),
      ),
      '#default_value' => isset($menu['orientation']) ? $menu['orientation'] : 'horizontal',
      '#description' => t('Select whether the menu will extend horizontally and drop down, or if it will extend vertically and fly out.'),
  );
  $form['animation_effect'] = array(
      '#type' => 'select',
      '#title' => t('Animation Effect'),
      '#options' => array(
          '' => t('None'),
          'fade' => t('Fade'),
          'slide' => t('Slide'),
      ),
      '#default_value' => isset($menu['animation_effect']) ? $menu['animation_effect'] : '',
      '#description' => t('Choose the effect to use when showing or hiding the menu'),
  );
  $form['animation_speed'] = array(
    '#type' => 'select',
    '#title' => t('Animation Speed'),
    '#options' => array(
      'slow' => t('Slow'),
      'fast' => t('Fast'),
    ),
    '#default_value' => isset($menu['animation_speed']) ? $menu['animation_speed'] : 'slow',
    '#description' => t('Choose the speed at which to show the effect'),
    '#states' => array(
      'invisible' => array(
        'select[name=animation_effect]' => array('value' => ''),
      ),
    ),
   );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit function for adding or editing a custom menu.
 */
function muchomenu_edit_menu_submit($form, &$form_state) {
  $menu = $form_state['values'];
  $path = 'admin/structure/menu/muchomenu/muchomenu-customize/';

  if ($form['#insert']) {
    $obj = new stdClass();
    $key = array();
  }
  else {
    $obj = (object) muchomenu_load($menu['id']);
    $key = 'id';
  }

  $obj->title = $menu['title'];
  $obj->description = $menu['description'];
  $obj->orientation = $menu['orientation'];
  $obj->animation_effect = $menu['animation_effect'];
  $obj->animation_speed = $menu['animation_speed'];

  drupal_write_record('muchomenu_menus', $obj, $key);

  if ($form['#insert']) {
    $menu['id'] = $obj->id;
  }

  $form_state['redirect'] = $path . $menu['id'];
}

/**
 * Submit function for the 'Delete' button on the menu editing form.
 */
function muchomenu_delete_menu_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/menu/muchomenu/muchomenu-customize/' . $form_state['values']['id'] . '/delete';
}

/**
 * Build a confirm form for deletion of a custom menu.
 */
function muchomenu_delete_menu_confirm($form, &$form_state, $menu) {
  $form['#menu'] = $menu;
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';
  return confirm_form($form, t('Are you sure you want to delete the muchomenu %title?', array('%title' => $menu['title'])), 'admin/structure/menu/muchomenu/muchomenu-customize/' . $menu['id'], $caption, t('Delete'));
}

/**
 * Delete a muchomenu and all items in it.
 */
function muchomenu_delete_menu_confirm_submit($form, &$form_state) {
  $menu = $form['#menu'];
  $form_state['redirect'] = 'admin/structure/menu/muchomenu';
  $menu_id = $menu['id'];
  $items = muchomenu_get_items_by_menu($menu_id);
  if ($items) {
    foreach ($items as $item) {
      // TODO Please review the conversion of this statement to the D7 database API syntax.
      /* db_query('DELETE FROM {muchomenu_items} WHERE id = %d', $item['id']) */
      db_delete('muchomenu_items')
              ->condition('id', $item['id'])
              ->execute();
    }
  }
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query('DELETE FROM {muchomenu_menus} WHERE id = %d', $menu_id) */
  db_delete('muchomenu_menus')
          ->condition('id', $menu_id)
          ->execute();

  cache_clear_all();
  drupal_set_message(t('The menu %title and all its menu items have been deleted.', array('%title' => $menu['title'])));
  watchdog('muchomenu', 'Deleted menu %title and all its menu items.', array('%title' => $menu['title']), WATCHDOG_NOTICE);
}

/**
 * Menu callback; Build the menu link editing form.
 */
function muchomenu_edit_item($form, &$form_state, $type, $item, $menu) {
  if ($type == 'add' || empty($item)) {
    $menu_id = isset($menu['id']) ? $menu['id'] : 0;
  }
  if ($type == 'edit') {
    $menu_id = $item['menu_id'];
  }

  $panes = panels_mini_load_all();
  $pane_options = array();

  $pane_options[''] = t('Select one...');
  foreach ($panes as $name => $pane) {
    $pane_options[$name] = $pane->admin_title;
  }

  $form['menu'] = array(
      '#type' => 'fieldset',
      '#title' => t('Menu settings'),
      '#collapsible' => FALSE,
      '#tree' => TRUE,
      '#weight' => -2,
      '#attributes' => array('class' => array('menu-item-form')),
      '#item' => $item,
  );
  if ($type == 'add' || empty($item)) {
    // This is an add form, initialize the menu link.
    $item = array('id' => 0, 'menu_id' => $menu_id, 'title' => '', 'path' => '', 'panel_name' => '', 'alt' => '', 'enabled' => 1, 'type' => 0, 'weight' => 0);
  }
  if ($type == 'edit') {
    $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#submit' => array('muchomenu_item_delete_submit'),
        '#weight' => 10,
    );
  }
  if (!isset($item['type'])) {
    $item['type'] = $type;
  }
  foreach (array('id', 'menu_id', 'title', 'path', 'panel_name', 'alt', 'enabled', 'type', 'weight') as $key) {
    $form['menu'][$key] = array('#type' => 'value', '#value' => $item[$key]);
  }

  $form['menu']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => isset($item['title']) ? $item['title'] : '',
      '#description' => t('The name of the menu item (displayed when hovering over a menu image).'),
      '#required' => TRUE,
      '#weight' => -10,
  );

  $form['menu']['panel_name'] = array(
      '#type' => 'select',
      '#title' => t('Panel'),
      '#description' => t('Select a mini panel.'),
      '#options' => $pane_options,
      '#default_value' => isset($item['panel_name']) ? $item['panel_name'] : '',
      '#weight' => -8,
  );

  $form['menu']['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => isset($item['path']) ? $item['path'] : '',
      '#description' => t('The path this menu item links to. This can be an internal Drupal path such as %add-node or an external URL such as %drupal. Enter %front to link to the front page.', array('%front' => '<front>', '%add-node' => 'node/add', '%drupal' => 'http://drupal.org')),
      '#weight' => -7,
  );
  $form['menu']['alt'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#default_value' => isset($item['alt']) ? $item['alt'] : '',
      '#description' => t('The ALT tag for the image if its not displayed.'),
      '#weight' => -6,
  );
  $form['menu']['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enabled'),
      '#default_value' => (isset($item['enabled']) && $item['enabled'] == TRUE) ? 1 : 0,
      '#description' => t('Menu items that are not enabled will not be listed in any menu.'),
      '#weight' => -5,
  );
  $form['menu']['target'] = array(
      '#type' => 'checkbox',
      '#title' => t('Open in New Window'),
      '#default_value' => (isset($item['target']) && $item['target'] == TRUE) ? 1 : 0,
      '#description' => t('If checked, this link will open in a new window.'),
      '#weight' => -5,
  );
  $form['menu']['disable_click'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable Click'),
      '#default_value' => (isset($item['disable_click']) && $item['disable_click'] == TRUE) ? 1 : 0,
      '#description' => t('Disable clicking directly on this menu item.  Use this to force people to use the flyout menu'),
      '#weight' => -5,
  );

  $form['menu']['menu_id'] = array('#type' => 'hidden', '#value' => $menu_id);

  $form['menu']['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#delta' => 50,
      '#default_value' => isset($item['weight']) ? $item['weight'] : 0,
      '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
      '#weight' => -2,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Validate form values for a menu link being added or edited.
 */
function muchomenu_edit_item_validate($form, &$form_state) {
  $item = &$form_state['values']['menu'];
}

/**
 * Process menu and menu item add/edit form submissions.
 */
function muchomenu_edit_item_submit($form, &$form_state) {
  $item = $form_state['values']['menu'];

  if ($item['path'] == 'node') {
    $item['path'] = '<front>';
  }

  foreach (array('enabled', 'target', 'disable_click') as $key) {
    $item[$key] = ($item[$key]) ? 1 : 0;
  }

  if (!$item['id']) {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("INSERT INTO {muchomenu_items} (menu_id, path, panel_name, title, alt, weight, enabled, target, disable_click) VALUES (%d, '%s', '%s', '%s', '%s', %d, %d, %d, %d)", $item['menu_id'], $item['path'], $item['panel_name'], $item['title'], $item['alt'], $item['weight'], $item['enabled'], $item['target'], $item['disable_click']) */
    $id = db_insert('muchomenu_items')
                    ->fields(array(
                        'menu_id' => $item['menu_id'],
                        'path' => $item['path'],
                        'panel_name' => $item['panel_name'],
                        'title' => $item['title'],
                        'alt' => $item['alt'],
                        'weight' => $item['weight'],
                        'enabled' => $item['enabled'],
                        'target' => $item['target'],
                        'disable_click' => $item['disable_click'],
                    ))
                    ->execute();
    drupal_set_message(t('Menu item successfully added.'));
    $t_args = array('%title' => $item['title']);
    watchdog('muchomenu', 'Added new muchomenu menu item %title', $t_args, WATCHDOG_NOTICE);
  }
  else {
    // TODO Please review the conversion of this statement to the D7 database API syntax.
    /* db_query("UPDATE {muchomenu_items} SET menu_id = %d, path = '%s', panel_name = '%s', title = '%s', alt = '%s', weight = %d, enabled = %d, target = %d, disable_click = %d WHERE id = %d", $item['menu_id'], $item['path'], $item['panel_name'], $item['title'], $item['alt'], $item['weight'], $item['enabled'], $item['target'], $item['disable_click'], $item['id']) */
    db_update('muchomenu_items')
            ->fields(array(
                'menu_id' => $item['menu_id'],
                'path' => $item['path'],
                'panel_name' => $item['panel_name'],
                'title' => $item['title'],
                'alt' => $item['alt'],
                'weight' => $item['weight'],
                'enabled' => $item['enabled'],
                'target' => $item['target'],
                'disable_click' => $item['disable_click'],
            ))
            ->condition('id', $item['id'])
            ->execute();
    drupal_set_message(t('Menu item successfully updated.'));
    $t_args = array('%title' => $item['title']);
    watchdog('muchomenu', 'Updated muchomenu menu item %title', $t_args, WATCHDOG_NOTICE);
  }

  $form_state['redirect'] = 'admin/structure/menu/muchomenu/muchomenu-customize/' . $item['menu_id'];
}

/**
 * Submit function for the delete button on the menu item editing form.
 */
function muchomenu_item_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/menu/muchomenu/item/' . $form_state['values']['menu']['id'] . '/delete';
}

/**
 * Build a confirm form for deletion of a single menu link.
 */
function muchomenu_item_delete_form($form, &$form_state, $item) {
  $form['#item'] = $item;
  $menu_id = muchomenu_item_get_menu_id($item);
  return confirm_form($form, t('Are you sure you want to delete the muchomenu item %item?', array('%item' => $item['title'])), 'admin/structure/menu/muchomenu/muchomenu-customize/' . $menu_id);
}

/**
 * Process menu delete form submissions.
 */
function muchomenu_item_delete_form_submit($form, &$form_state) {
  $item = $form['#item'];
  $menu_id = muchomenu_item_get_menu_id($item);
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query('DELETE FROM {muchomenu_items} WHERE id = %d', $item['id']) */
  db_delete('muchomenu_items')
          ->condition('id', $item['id'])
          ->execute();
  drupal_set_message(t('The muchomenu item %title has been deleted.', array('%title' => $item['title'])));
  watchdog('menu', 'Deleted muchomenu item %title.', array('%title' => $item['title']), WATCHDOG_NOTICE);
  $form_state['redirect'] = 'admin/structure/menu/muchomenu/muchomenu-customize/' . $menu_id;
}

/**
 * Menu callback; Build the form presenting menu configuration options.
 */
function muchomenu_configure($form, &$form_state) {
  $form = array();

  $form['muchomenu_style_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom CSS Path for MuchoMenu Styles'),
      '#description' => t('This path should be relative to your drupal installation. (i.e., sites/default/files/my-custom-css.css)'),
      '#default_value' => variable_get('muchomenu_style_path', ''),
  );

  return system_settings_form($form);
}
