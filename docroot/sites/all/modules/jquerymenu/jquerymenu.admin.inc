<?php
/**
 * @file
 * Administration page callbacks for the jQuerymenu module
 */

/**
 * Admin form for jQuery menu module.
 *
 * @ingroup forms
 * @see system_settings_form(),
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

  // Create a checkbox to enable animation - jquerymenu_animate.
  $form['jquerymenu_animate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable animation.'),
    '#description' => t('If checked your menus will open with transition effects.'),
    '#default_value' => variable_get('jquerymenu_animate', 1),
  );

  // Create a checkbox to display an edit link - jquerymenu_edit_link.
  $form['jquerymenu_edit_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Edit Link.'),
    '#description' => t('If checked the edit link will display when hovering over menu items to those having administration privileges. This setting will only affect menus directly created by the jQuery Menu module. Other modules that implement the edit link will have their own settings.'),
    '#default_value' => variable_get('jquerymenu_edit_link', 1),
  );
  
  // create a checkbox to disable link on items with children
  $form['jquerymenu_parentlink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable links for menu items with children.'),
    '#description' => t('If checked, menu items which contain children will not have hyperlinks, rather will use plain titles.'),
    '#default_value' => variable_get('jquerymenu_parentlink', 0),
  );
  
  // create a checkbox to display children when a link is clicked, rather than to follow the link
  $form['jq_clicktoexpand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Click to expand'),
    '#description' => t('If checked, a parent menu item will expand/collapse its children instead of following the link.'),
    '#default_value' => variable_get('jq_click_to_expand', 0),
  );   
  
  // create a checkbox to enable hover
  $form['jquerymenu_hover'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expand children on hover.'),
    '#description' => t('If checked the child menu items will be displayed when hovering over the parent menu item.'),
    '#default_value' => variable_get('jquerymenu_hover', 0),
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

  // Save animation, edit link and parent link toggles.
  variable_set('jquerymenu_animate', $form_state['values']['jquerymenu_animate']);
  variable_set('jquerymenu_edit_link', $form_state['values']['jquerymenu_edit_link']);
  variable_set('jquerymenu_parentlink', $form_state['values']['jquerymenu_parentlink']);
  variable_set('jquerymenu_hover', $form_state['values']['jquerymenu_hover']);  
  variable_set('jq_click_to_expand', $form_state['values']['jq_clicktoexpand']);
  
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
