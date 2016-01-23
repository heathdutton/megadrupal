<?php

/**
 * Functionality for the admin views.
 */

/**
 * Form constructor for the admin form.
 */
function single_page_site_settings_form() {
  $settings = variable_get('single_page_site_settings', array());
  $form = array();

  $menus = menu_get_menus();

  $form['general-settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general-settings']['menu'] = array(
    '#title' => t('Menu'),
    '#type' => 'select',
    '#options' => $menus,
    '#default_value' => !empty($settings['menu']) ? $settings['menu'] : NULL,
    '#required' => TRUE,
    '#description' => t('Which menu should be used for the single page navigation?'),
  );

  $form['general-settings']['menuclass'] = array(
    '#title' => t('Menu Class/Id'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['menuclass']) ? $settings['menuclass'] : NULL,
    '#required' => TRUE,
    '#description' => t('Define the class/id of the menu wrapper. Eg: #main-menu or .main-menu'),
  );

  $description = t('Define the class of the menu items that should implement the single page navigation Eg: single-page-item. Leave blank if you want to use all the menu items');
  if (!module_exists('menu_attributes')) {
    $description = t('You need the <a href="!url" target="_blank">menu attributes module</a> to use this feature.',
            array('!url' => url('https://www.drupal.org/project/menu_attributes'))) . '<br />';
    $description .= t('Define the class of the menu items that should implement the single page navigation Eg: single-page-item. Leave blank if you want to use all the menu items');
  }
  $form['general-settings']['class'] = array(
    '#title' => t('Menu item selector'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['class']) ? $settings['class'] : NULL,
    '#required' => FALSE,
    '#disabled' => !module_exists('menu_attributes') ? TRUE : FALSE,
    '#description' => $description,
  );

  $form['general-settings']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['title']) ? $settings['title'] : NULL,
    '#required' => TRUE,
    '#description' => t('Configure the title of the page'),
  );

  $form['general-settings']['tag'] = array(
    '#title' => t('Tag'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['tag']) ? $settings['tag'] : NULL,
    '#required' => TRUE,
    '#description' => t('Define the HTML tag which should be used for title-wrapping. Eg: h2 or p'),
  );

  $form['general-settings']['homepage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Homepage'),
    '#description' => t('I will use this page as my homepage.'),
    '#default_value' => isset($settings['homepage']) ? $settings['homepage'] : 1,
  );

  $form['scroll-settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scroll settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['scroll-settings']['scroll-down'] = array(
    '#title' => t('Down'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['down']) ? $settings['down'] : 50,
    '#required' => TRUE,
    '#description' => t('Define the distance between the item and the viewport (px) when a menu item should be highlighted when scrolling down'),
  );

  $form['scroll-settings']['scroll-up'] = array(
    '#title' => t('Up'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['up']) ? $settings['up'] : 200,
    '#required' => TRUE,
    '#description' => t('Define the distance between the item and the viewport (px) when a menu item should be highlighted when scrolling up'),
  );

  $form['advanced-settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced-settings']['smooth-scrolling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use smooth scrolling'),
    '#default_value' => isset($settings['smoothscrolling']) ? $settings['smoothscrolling'] : 1,
  );

  $form['advanced-settings']['update-hash'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update url fragment while scrolling'),
    '#default_value' => isset($settings['updatehash']) ? $settings['updatehash'] : 0,
  );

  $form['advanced-settings']['offset_selector'] = array(
    '#title' => t('Which menu should be used for calculating the offset'),
    '#type' => 'textfield',
    '#default_value' => !empty($settings['offset_selector']) ? $settings['offset_selector'] : NULL,
    '#description' => t('The height of this selector will be used to determine the scroll to position. Most of the times this is will be the same selector as your "Menu Class/Id"'),
  );


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form validation handler for single_page_site_settings_form().
 */
function single_page_site_settings_form_validate($form, &$form_state) {
  // Validate menu class on special chars.
  $menu_class = $form_state['values']['menuclass'];
  if (preg_match('/[^A-Za-z0-9#.-]/', $menu_class)) {
    form_set_error('menuclass',
        t('"Menu Class/Id" contains forbidden chars. Only a-z, #, ., - allowed.'));
  }

  if (module_exists('menu_attributes')) {
    // Validate class on non alphapetic chars.
    $class = $form_state['values']['class'];
    if (preg_match('/[^A-Za-z0-9-]/', $class)) {
      form_set_error('class',
          t('"Menu item selector" contains forbidden chars. Only a-z, - allowed.'));
    }
  }

  // Validate tag on special chars.
  $tag = $form_state['values']['tag'];
  if (preg_match('/[^A-Za-z0-9]/', $tag)) {
    form_set_error('tag', t('"Tag" contains special characters.'));
  }
}

/**
 * Form submit handler for single_page_site_settings_form().
 */
function single_page_site_settings_form_submit($form, &$form_state) {
  $settings = variable_get('single_page_site_settings', array());
  if (!isset($settings['offset_selector'])) {
    // This option hasn't been set yet, populate it with menuclass.
    // This is for all people who already have this module installed,
    // Before this feature was introduced.
    $form_state['values']['offset_selector'] = $form_state['values']['menuclass'];
  }


  variable_set('single_page_site_settings',
      array(
    'menu' => $form_state['values']['menu'],
    'menuclass' => $form_state['values']['menuclass'],
    'class' => (module_exists('menu_attributes')) ? $form_state['values']['class'] : NULL,
    'tag' => $form_state['values']['tag'],
    'title' => $form_state['values']['title'],
    'down' => $form_state['values']['scroll-down'],
    'up' => $form_state['values']['scroll-up'],
    'smoothscrolling' => $form_state['values']['smooth-scrolling'],
    'updatehash' => $form_state['values']['update-hash'],
    'homepage' => $form_state['values']['homepage'],
    'offset_selector' => $form_state['values']['offset_selector'],
  ));

  if ($form_state['values']['homepage']) {
    // Set single-page-site as homepage.
    variable_set('site_frontpage', 'single-page-site');
  }

  drupal_set_message(t('Your settings have been saved.'));
}
