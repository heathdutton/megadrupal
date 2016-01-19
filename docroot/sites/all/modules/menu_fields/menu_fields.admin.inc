<?php
/**
 * @file
 * Contains admin forms and code.
 */

/**
 * Menu callback which shows an overview page of all the custom menus and
 * their descriptions.
 *
 * Basic version taken from menu_overview_page(). Since that functions output
 * can not be altered easily, it has been token over and modified here. The
 * drawback ist, that changes to the core function do not propagate in here. So
 * new core releases must be examined for changes to this function and the
 * changes must be ported in here.
 */
function menu_fields_menu_overview_page() {
  module_load_include('inc', 'menu', 'admin.menu');
  $enabled_menus = menu_fields_enabled_menus();

  // Direct copy from menu_overview_page().
  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  $header = array(t('Title'), array('data' => t('Operations'), 'colspan' => '5')); // <- colspan has been changed from 3 to 5
  $rows = array();
  foreach ($result as $menu) {
    $row = array(theme('menu_admin_overview', array('title' => $menu['title'], 'name' => $menu['menu_name'], 'description' => $menu['description'])));
    $row[] = array('data' => l(t('list links'), 'admin/structure/menu/manage/' . $menu['menu_name']));
    $row[] = array('data' => l(t('edit menu'), 'admin/structure/menu/manage/' . $menu['menu_name'] . '/edit'));

    // Start: Specific to menu_fields
    if (in_array($menu['menu_name'], $enabled_menus)) {
      $row[] = array('data' => l(t('manage fields'), 'admin/structure/menu/manage/' . $menu['menu_name'] . '/fields'));
      $row[] = array('data' => l(t('manage display'), 'admin/structure/menu/manage/' . $menu['menu_name'] . '/display'));
    }
    else {
      $row[] = '';
      $row[] = '';
    }
    // End: Specific to menu_fields

    $row[] = array('data' => l(t('add link'), 'admin/structure/menu/manage/' . $menu['menu_name'] . '/add'));
    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Form definition for menu fields configuration.
 */
function menu_fields_field_configuration_form() {

  $form['fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled Menu Fields'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $header = array(
    'menu_name' => t('Menu name'),
    'operation_field' => t('Fields'),
    'operation_display' => t('Display'),
  );

  $options = array();
  $default_values = array();

  $menus = menu_get_menus();
  $selected_menus = menu_fields_enabled_menus();

  foreach ($menus as $key => $value) {

    $options[$key] = array('menu_name' => $value);

    // Add manage fields/display link when menu is selected
    if (in_array($key, $selected_menus)) {
      $default_values[$key] = TRUE;

      $options[$key]['operation_field'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Manage fields'),
          '#href' => 'admin/structure/menu/manage/' . $key . '/fields',
        ),
      );
      $options[$key]['operation_display'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Manage display'),
          '#href' => 'admin/structure/menu/manage/' . $key . '/display',
        ),
      );

    }
    else {
      $default_values[$key] = FALSE;
      $options[$key]['operation_field'] = '';
      $options[$key]['operation_display'] = '';
    }
  }

  $form['fields']['menus'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No menus available.'),
    '#multiple' => TRUE,
    '#default_value' => $default_values,
    '#js_select' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for the menu fields administration form.
 *
 * @see menu_fields_field_configuration_form().
 */
function menu_fields_field_configuration_form_submit($form, &$form_state) {
  $menus = array();

  foreach ($form_state['values']['menus'] as $key => $value) {
    if ($key === $value) {
      $menus[] = $value;
    }
  }

  menu_fields_save_enabled_menus($menus);
  drupal_set_message(t('Field settings saved.'));
}
