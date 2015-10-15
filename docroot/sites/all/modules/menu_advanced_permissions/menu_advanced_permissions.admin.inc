<?php

/**
 * @file
 * Administration page callbacks for the Menu advanced permissions module.
 */

/**
 * Form constructor for the menu advanced permissions settings form.
 *
 * @see menu_advanced_permissions_settings_validate()
 *
 * @ingroup forms
 */
function menu_advanced_permissions_settings($form, &$form_state) {
  $form['menu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['menu']['map_menu_list_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Access list of custom menus'),
    '#default_value' => variable_get('map_menu_list_access', 1),
    '#description' => t("In order to grant permissions for user to add, edit, "
        . "delete or change settings of custom menus you must enable this option. "
        . "<br /><strong><em>Note: if you don't enable this option you must create menu "
        . "item with path to the specific custom menu so that an user can "
        . "go and see a list of menu items for that menu.</em></strong>"
    ),
  );

  $form['menu']['map_menu_parents_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Access menu parents page'),
    '#default_value' => variable_get('map_menu_parents_access', 1),
    '#description' => t('Roles with this permission can access the parents menu.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_menu_list_access"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu']['map_menu_settings_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Access menu settings page'),
    '#default_value' => variable_get('map_menu_settings_access', 1),
    '#description' => t('Roles with this permission can access the menu settings page.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_menu_list_access"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu']['map_add_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add new custom menu'),
    '#default_value' => variable_get('map_add_menu', 1),
    '#description' => t('Roles with this permission can add new custom menu.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_menu_list_access"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu']['map_edit_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Edit custom menu'),
    '#default_value' => variable_get('map_edit_menu', 1),
    '#description' => t('Roles with this permission can edit custom menu.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_menu_list_access"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu']['map_delete_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete custom menu'),
    '#default_value' => variable_get('map_delete_menu', 1),
    '#description' => t('Roles with this permission can delete custom menu.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_menu_list_access"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu_item'] = array(
    '#type' => 'fieldset',
    '#title' => t('Menu item settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['menu_item']['map_view_menu_items'] = array(
    '#type' => 'checkbox',
    '#title' => t('Access list of menu items'),
    '#default_value' => variable_get('map_view_menu_items', 1),
    '#description' => t("In order to grant permissions for user to add, edit, "
        . "delete or reset menu items you must enable this option. "
    ),
  );

  $form['menu_item']['map_add_menu_item'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add menu item'),
    '#default_value' => variable_get('map_add_menu_item', 1),
    '#description' => t('Roles with this permission can add menu item to specific menu for which they have the view list of links permission.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_view_menu_items"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu_item']['map_edit_menu_item'] = array(
    '#type' => 'checkbox',
    '#title' => t('Edit menu item'),
    '#default_value' => variable_get('map_edit_menu_item', 1),
    '#description' => t('Roles with this permission can edit menu item from specific menu for which they have the view list of links permission.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_view_menu_items"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu_item']['map_reset_menu_item'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reset menu item'),
    '#default_value' => variable_get('map_reset_menu_item', 1),
    '#description' => t('Roles with this permission can reset menu item from specific menu for which they have the view list of links permission.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_view_menu_items"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['menu_item']['map_delete_menu_item'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete menu item'),
    '#default_value' => variable_get('map_delete_menu_item', 1),
    '#description' => t('Roles with this permission can delete menu item from specific menu for which they have the view list of links permission.'),
    '#states' => array(
      'visible' => array(
        ':input[name="map_view_menu_items"]' => array('checked' => TRUE),
      ),
    )
  );

  $form['#validate'][] = 'menu_advanced_permissions_settings_validate';

  return system_settings_form($form);
}

/**
 * Validate form values for the menu advanced permissions settings form.
 */
function menu_advanced_permissions_settings_validate($form, &$form_state) {
  if (empty($form_state['values']['map_menu_list_access']) || empty($form_state['values']['map_view_menu_items'])) {
    $roles = user_roles();
    $menu_roles = user_roles(FALSE, 'administer menu');
    $menus = menu_get_menus();

    if (empty($form_state['values']['map_menu_list_access'])) {
      $form_state['values']['map_menu_parents_access'] = 0;
      $form_state['values']['map_menu_settings_access'] = 0;
      $form_state['values']['map_add_menu'] = 0;
      $form_state['values']['map_edit_menu'] = 0;
      $form_state['values']['map_delete_menu'] = 0;

      foreach ($roles as $rid => $role) {
        user_role_revoke_permissions($rid, array('menu list access', 'menu parents access', 'menu settings access', 'add menu access'));

        foreach ($menus as $name => $title) {
          user_role_revoke_permissions($rid, array("view $name menu", "edit $name menu", "delete $name menu"));
        }
      }
    } else {
      foreach ($menu_roles as $rid => $role) {
        user_role_grant_permissions($rid, array('menu list access', 'menu parents access', 'menu settings access', 'add menu access'));

        foreach ($menus as $name => $title) {
          user_role_grant_permissions($rid, array("view $name menu", "edit $name menu", "delete $name menu"));
        }
      }
    }

    if (empty($form_state['values']['map_view_menu_items'])) {
      $form_state['values']['map_add_menu_item'] = 0;
      $form_state['values']['map_edit_menu_item'] = 0;
      $form_state['values']['map_reset_menu_item'] = 0;
      $form_state['values']['map_delete_menu_item'] = 0;

      foreach ($roles as $rid => $role) {
        foreach ($menus as $name => $title) {
          user_role_revoke_permissions($rid, array("add $name menu", "edit $name menu item", "reset $name menu item", "delete $name menu item"));
        }
      }
    } else {
      foreach ($menu_roles as $rid => $role) {
        foreach ($menus as $name => $title) {
          user_role_grant_permissions($rid, array("add $name menu", "edit $name menu item", "reset $name menu item", "delete $name menu item"));
        }
      }
    }
  }
}
