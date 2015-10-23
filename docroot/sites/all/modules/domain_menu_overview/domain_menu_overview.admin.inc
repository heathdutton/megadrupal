<?php
/**
 * @file
 * Admin page callbacks for the menu domain module.
 */

 /**
  * Builds a form with all the menus to be enabled/disabled.
  */
function domain_menu_overview_settings() {
  $menus = menu_get_menus();

  foreach ($menus as $menu => $localized_name) {
    $var_name_mnu = 'domain_menu_overview_' . str_replace(' ', '_', $menu);
    $form[$var_name_mnu] = array(
      '#type' => 'checkbox',
      '#title' => t('Do you want to manage') . ' ' . $menu,
      '#default_value' => variable_get($var_name_mnu, FALSE),
    );
  }

  $form[DOMAIN_MENU_OVERVIEW_ADMIN_SETTING_STUCTURE_OVERVIEW] = array(
    '#type' => 'select',
    '#title' => t('What kind of menu listing do you want at : /admin/structure?') ,
    '#description' => t('Default means: You will have an link for each menu
    and each language at the admin/structure page on the menu per language
    page you will also have the tabbed navigation to quickly switch between
    languages for that menu. The Small version will only have the tabbed
    navigation and one menu entry at /admin/structure.'),
    '#options' => array(
      DOMAIN_MENU_OVERVIEW_DEFAULT_NAME => t('Default'),
      'small' => t('Small'),
      DOMAIN_MENU_OVERVIEW_LOCAL_ACTION => t('Local action'),
    ),
    '#default_value' => variable_get(DOMAIN_MENU_OVERVIEW_ADMIN_SETTING_STUCTURE_OVERVIEW, DOMAIN_MENU_OVERVIEW_LOCAL_ACTION),
  );

  $form[DOMAIN_MENU_OVERVIEW_ADMIN_SETTING_DETAIL_MENU] = array(
    '#type' => 'select',
    '#title' => t('What kind of menu detail do you want at : /admin/structure/[your-menu]?') ,
    '#options' => array(
      DOMAIN_MENU_OVERVIEW_DEFAULT_NAME => t('Default (Tabs as in screenshots)'),
      DOMAIN_MENU_OVERVIEW_LOCAL_ACTION => t('Local action links'),
    ),
    '#default_value' => variable_get(DOMAIN_MENU_OVERVIEW_ADMIN_SETTING_DETAIL_MENU, DOMAIN_MENU_OVERVIEW_DEFAULT_NAME),
  );

  $form['#submit'][] = 'domain_menu_overview_settings_form_submit';

  return system_settings_form($form);
}

 /**
  * Submit function that states flush caches.
  *
  * form @param $form the form.
  * form @param $form_state the form_state.
  */
function domain_menu_overview_settings_form_submit($form, &$form_state) {
  drupal_set_message(t('Please flush caches when you have changed a setting :') . ' ' . l(t('Flush all caches'), 'admin/config/development/performance'));
}
