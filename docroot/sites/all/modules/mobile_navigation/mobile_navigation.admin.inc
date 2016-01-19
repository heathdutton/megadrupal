<?php

/**
 * @file
 * Mobile Navigation Administration page.
 */

/**
 * Mobile navigation admin form definition.
 */
function mobile_navigation_configuration_form($form, &$form_state) {
  $form['markup'] = array(
    '#markup' => t('Welcome to Mobile Navigation configuration page. Here you can administer Mobile Navigation settings.
          Please note that this Module currently works only for menus that uses tag < ul > for the menu and < li > for menu items.'),
  );

  $form['mobile_navigation_breakpoint'] = array(
    '#type' => 'textfield',
    '#title' => t('Not mobile range media query'),
    '#description' => t('Media Query for the NOT MOBILE range, meaning the range where you DO NOT want the mobile navigation to show up. 
          (The reason for this is that Media Queries are pretty complex stuff and usually responsive themes don\'t have mobile media query, 
          they have media queries for the NOT MOBILE displays, so you can easily copy this media query from your preferred theme configuration.)'),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => TRUE,
    '#default_value' => variable_get('mobile_navigation_breakpoint', MOBILE_NAVIGATION_BREAKPOINT),
  );

  $form['mobile_navigation_menuselector'] = array(
    '#type' => 'textfield',
    '#title' => t('JQuery selector for Menu element'),
    '#description' => t('The Selector for the menu element.'),
    '#size' => 40,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => variable_get('mobile_navigation_menuselector', MOBILE_NAVIGATION_MENUSELECTOR),
  );

  $form['mobile_navigation_mainPageSelector'] = array(
    '#title' => t('Main content selector. (The main content container just after the < body > tag)'),
    '#type' => 'textfield',
    '#default_value' => variable_get('mobile_navigation_mainPageSelector', MOBILE_NAVIGATION_PAGESELECTOR),
    '#description' => t('The main container for the page. On Bartik is "#page".'),
  );

  $form['mobile_navigation_menu_label'] = array(
    '#title' => t('Mobile menu label.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('mobile_navigation_menu_label', MOBILE_NAVIGATION_MENU_LABEL),
    '#description' => t('The label used on the mobile menu.'),
  );

  $form['mobile_navigation_behavior'] = array(
    '#title' => 'Mobile Navigation behavior',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#type' => 'fieldset',
    '#description' => t('Mobile navigation behaviors configuration.'),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_plugin'] = array(
    '#type' => 'radios',
    '#title' => t('Plugin Type'),
    '#default_value' => variable_get('mobile_navigation_plugin', MOBILE_NAVIGATION_PLUGIN),
    '#options' => mobile_navigation_get_plugins(),
    '#description' => t('Type of mobile navigation plugin.'),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_showitems'] = array(
    '#type' => 'radios',
    '#title' => t('Show Items policy (only for accordion plugin)'),
    '#default_value' => variable_get('mobile_navigation_showitems', MOBILE_NAVIGATION_SHOWITEMS),
    '#options' => array(
      'all' => t('Show and keep expanded all opened items.'),
      'active_trail' => t('Keep expanded only the items in the active trail, automaticaly close any other opened item.'),
    ),
    '#description' => t('When the user expands an item that has sub-items, they can presistently stay oppened or it can be that any item that is not in the current active trail will automaticaly close.'),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_showEffect'] = array(
    '#type' => 'radios',
    '#title' => t('Menu Effect'),
    '#default_value' => variable_get('mobile_navigation_showEffect', MOBILE_NAVIGATION_SHOWEFFECT),
    '#options' => mobile_navigation_get_effects(),

    '#description' => t('Choose an Effect to use for the mobile menu.'),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu Width (%)'),
    '#description' => t('Width of the mobile menu. (%)'),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('mobile_navigation_width', MOBILE_NAVIGATION_WIDTH),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_tabhandler'] = array(
    '#title' => t("Use tab handler on menu (Don't apply for Expand Down effect)"),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mobile_navigation_tabhandler', MOBILE_NAVIGATION_TABHANDLER),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_useMask'] = array(
    '#title' => t('Use mask over contents when mobile menu is openned'),
    '#type' => 'checkbox',
    '#description' => t('Block the page contents when the menu gets openned and place a clickable mask over to close the menu when it is clicked.'),
    '#default_value' => variable_get('mobile_navigation_useMask', MOBILE_NAVIGATION_USE_MASK),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_special_classes'] = array(
    '#title' => t('Add Special classes to mobile menu'),
    '#type' => 'checkbox',
    '#description' => t('Special Classes are added to Menu Items according to their position and level. (Like for instance: Item-1, Item-2-1, Item-3-1-1, etc.)'),
    '#default_value' => variable_get('mobile_navigation_special_classes', MOBILE_NAVIGATION_SPECIAL_CLASSES),
  );

  $form['mobile_navigation_behavior']['mobile_navigation_expand_active_trail'] = array(
    '#title' => t('Expand active trail automatically (only for accordion plugin)'),
    '#type' => 'checkbox',
    '#description' => t('Expands active trail accordion automatically'),
    '#default_value' => variable_get('mobile_navigation_expand_active_trail', MOBILE_NAVIGATION_EXPAND_ACTIVE_TRAIL),
  );

  return system_settings_form($form);
}
