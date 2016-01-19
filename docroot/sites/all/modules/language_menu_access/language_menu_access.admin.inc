<?php

/**
 * @file
 * Language Menu Access module admin functions.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function language_menu_access_form_menu_edit_item_alter(&$form, &$form_state) {
  if (user_access('administer menus per language')) {
    // Load default values.
    $show_default_value = (isset($form['original_item']['#value']['options']['language_menu_access']['show'])) ? $form['original_item']['#value']['options']['language_menu_access']['show'] : array();
    $hide_default_value = (isset($form['original_item']['#value']['options']['language_menu_access']['hide'])) ? $form['original_item']['#value']['options']['language_menu_access']['hide'] : array();
    // If any of language boxes are selected, display fieldset as expanded.
    $collapsed = (count($show_default_value) || count($hide_default_value)) ? FALSE : TRUE;
    $form['language_menu_access']['manage'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Manage item visibility per language'),
      '#collapsible'    => TRUE,
      '#collapsed'      => $collapsed,
      '#description'    => '<p>' . t('This section allows to show or hide this menu item on selected languages. See <a href="@help_url">module help</a> for more information.', array('@help_url' => url('admin/help/language_menu_access'))) . '</p>',
    );
    // Get list of all available languages.
    // Add 'd' to array keys so it is saved properly even for domain_id = 0.
    $options = array();
    foreach (language_list() as $language) {
      $options[$language->language] = $language->name;
    }
    // Show menu item per domain config.
    $form['language_menu_access']['manage']['language_menu_access_show'] = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Show menu item only on selected languages'),
      '#options'        => $options,
      '#default_value'  => $show_default_value,
      '#description'    => t('Show this menu item only on selected language(s). If you select no languages, the menu item will be visible on all languages.'),
    );
    // Hide menu item per language config.
    $form['language_menu_access']['manage']['language_menu_access_hide'] = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Hide menu item on selected languages'),
      '#options'        => $options,
      '#default_value'  => $hide_default_value,
      '#description'    => t('Hide this menu item on selected language(s). If you select no languages, the menu item will be visible on all languages.'),
    );
    // Add weight to Submit button, as it is not set by default by menu.module,
    // and because it sets weight of Delete button, it could happen that our
    // fieldset is displayed between Save and Delete buttons.
    $form['submit']['#weight'] = 9;
    // Add own validate function to avoid one menu item
    // being both displayed and hidden for the same language.
    $form['#validate'][] = 'language_menu_access_form_menu_edit_item_validate';
    // Make sure that our submit is called before menu_edit_item_submit()
    // from Drupal core menu.admin.inc, as there the changes are being saved.
    array_unshift($form['#submit'], 'language_menu_access_form_menu_edit_item_submit');
  }
}

/**
 * Validate function for menu_edit_item form.
 *
 * Processes updated form submission and adds extra
 * information to menu $item['options'] element.
 */
function language_menu_access_form_menu_edit_item_validate($form, &$form_state) {
  if ($form_state['submitted'] && user_access('administer menus per language')) {
    // Ensure menu item is not shown and hidden for the same language.
    foreach ($form_state['values']['language_menu_access_hide'] as $key => $value) {
      if ($value && isset($form_state['values']['language_menu_access_show'][$key]) && $form_state['values']['language_menu_access_show'][$key]) {
        form_set_error('language_menu_access_hide', t('Cannot both show and hide menu item for the same language.'));
      }
    }
  }
}

/**
 * Submit function for menu_edit_item form.
 *
 * Processes updated form submission and adds extra
 * information to menu $item['options'] element.
 */
function language_menu_access_form_menu_edit_item_submit($form, &$form_state) {
  if ($form_state['submitted'] && user_access('administer menus per language')) {
    // Clear previous language access values.
    $form_state['values']['options']['language_menu_access'] = array(
      'show' => array(),
      'hide' => array(),
    );
    // Process showing menu item per language.
    foreach ($form_state['values']['language_menu_access_show'] as $key => $value) {
      if ($value) {
        $form_state['values']['options']['language_menu_access']['show'][$key] = $key;
      }
    }
    // Process hiding menu item per language.
    foreach ($form_state['values']['language_menu_access_hide'] as $key => $value) {
      if ($value) {
        $form_state['values']['options']['language_menu_access']['hide'][$key] = $key;
      }
    }
    // Verify that at least one checkbox for 'show' and for 'hide' was ticked.
    // Otherwise just unset relevant arrays, as no point in saving them then.
    if (count($form_state['values']['options']['language_menu_access']['show']) == 0) {
      unset($form_state['values']['options']['language_menu_access']['show']);
    }
    if (count($form_state['values']['options']['language_menu_access']['hide']) == 0) {
      unset($form_state['values']['options']['language_menu_access']['hide']);
    }
    if (count($form_state['values']['options']['language_menu_access']) == 0) {
      unset($form_state['values']['options']['language_menu_access']);
    }
    // Set 'alter' flag to true for this menu item if it was modified,
    // otherwise hook_translated_menu_link_alter() will not be invoked:
    // https://api.drupal.org/api/function/hook_translated_menu_link_alter/7
    if (isset($form_state['values']['options']['language_menu_access'])) {
      $form_state['values']['options']['alter'] = TRUE;
    }
  }
}
