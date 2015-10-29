<?php

/**
 * @file
 * Administration page callbacks for the jQuerymenu module
 */

/**
 * Admin form for jQuery menu module.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function jquerymenu_admin_settings() {

  // List all custom menus - $menulist
  $result = db_query("SELECT menu_name, title FROM {menu_custom}");
  $menulist = array();
  foreach ($result as $menu) {
    $menulist[$menu->menu_name] = $menu->title;
  }

  // get a list of all jquerymenus - $enabledmenus
  $result = db_query("SELECT menu_name FROM {jquerymenus}");
  $enabledmenus = array();  
  foreach ($result as $enabled) {
    $enabledmenus[] = $enabled->menu_name;
  }

  // find which items in $menulist are in $enabledmenus - $defaultvalue
  $defaultvalue = array();
  foreach ($menulist as $index => $value) {
    foreach ($enabledmenus as $em) {
      if ($index == $em) {
        $defaultvalue[] = $index;
      }
      else {
        $defaultvalue[] = 0;
      }
    }
  }

  // Create a list of available menus - jquerymenu_activate.
  $form['jquerymenu_activate'] = array(
    '#type' => 'checkboxes',
    '#title' => t('jQuery Menu blocks'),
    '#options' => $menulist,
    '#default_value' => $defaultvalue,
    '#multiple' => TRUE,
    '#description' => t('Choose which menus you wish to create a jQuery Menu version for. Please note that this only creates the block. You will still have to activate the block on the blocks page.'),
  );

  // Create a checkbox to disable animation - jquerymenu_animate.
  $form['jquerymenu_animate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable animation.'),
    '#description' => t('If checked your menus will open without any transition effects.'),
    '#default_value' => variable_get('jquerymenu_animate', 1),
  );

  // Create a checkbox tp display an edit link - jquerymenu_edit_link.
  $form['jquerymenu_edit_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Edit Link.'),
    '#description' => t('If checked the edit link will display when hovering over menu items to those having administration privileges. This setting will only affect menus directly created by the jQuery Menu module. Other modules that implement the edit link will have their own settings.'),
    '#default_value' => variable_get('jquerymenu_edit_link', 1),
  );

  // Create a submit button.
  $form['jquerymenu_admin_settings_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Handle submission of jquerymenu_admin_settings_form, save to module variables and table..
 */
function jquerymenu_admin_settings_submit($form, &$form_state) {

  // Save animation and edit link toggles.
  variable_set('jquerymenu_animate', $form_state['values']['jquerymenu_animate']);
  variable_set('jquerymenu_edit_link', $form_state['values']['jquerymenu_edit_link']);

  // Save selected menus.
  $choices = $form_state['values']['jquerymenu_activate'];
  // get list of currently enabled menus
  $result = db_query("SELECT menu_name FROM {jquerymenus}");
  $enabledmenus = array();
  foreach ($result as $enabled) {
    $enabledmenus[] = $enabled->menu_name;
  }

  // Loop through all menu choices.
  foreach ($choices as $menuname => $value) {
 
    // if value is not 0 and the menu is not already enabled, save it to the database
    // ??? should it insert it into the block table too?
    if ($value != '0' && !in_array($value, $enabledmenus)) {
      $data = array(
        'menu_name' => $value,
      );
      drupal_write_record('jquerymenus', $data);
    }

    // if the value is 0 remove any records from the database - both block and jquerymenu
    elseif ($value == '0') {
      $delta = db_query("SELECT mid FROM {jquerymenus} WHERE menu_name = :name", array(':name' => $menuname))->fetchField();
      $delta = $delta - 1;

      db_delete('jquerymenus')
        ->condition('menu_name', $menuname)
        ->execute();
      db_delete('block')
        ->condition('module', 'jquerymenu')
        ->condition('delta', $delta)
        ->execute();
    }
  }

  // Success message.
  drupal_set_message(t('Your settings have been saved.'), $type = 'status', $repeat = FALSE);
}
