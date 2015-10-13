<?php

/**
 * @file
 * Domain Menu Access module admin functions.
 */

/**
 * Implementation of hook_form_FORM_ID_alter().
 */
function domain_menu_access_form_menu_edit_item_alter(&$form, &$form_state) {
  if (user_access('administer menus per domain')) {
    // Load default values.
    $show_default_value = (isset($form['original_item']['#value']['options']['domain_menu_access']['show'])) ? $form['original_item']['#value']['options']['domain_menu_access']['show'] : array();
    $hide_default_value = (isset($form['original_item']['#value']['options']['domain_menu_access']['hide'])) ? $form['original_item']['#value']['options']['domain_menu_access']['hide'] : array();
    // If any of domain boxes are selected, display fieldset as expanded.
    $collapsed = (count($show_default_value) || count($hide_default_value)) ? FALSE : TRUE;
    $form['domain_menu_access']['manage'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('Manage item visibility per domain'),
      '#collapsible'    => TRUE,
      '#collapsed'      => $collapsed,
      '#description'    => '<p>' . t('This section allows to show or hide this menu item on selected domains. See <a href="@help_url">module help</a> for more information.', array('@help_url' => url('admin/help/domain_menu_access'))) . '</p>',
    );
    // Get list of all available domains.
    // Add 'd' to array keys so it is saved properly even for domain_id = 0.
    $options = array();
    foreach (domain_domains() as $domain) {
      $options['d' . $domain['domain_id']] = $domain['sitename'];
    }
    // Show menu item per domain config.
    $form['domain_menu_access']['manage']['domain_menu_access_show'] = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Show menu item only on selected domains'),
      '#options'        => $options,
      '#default_value'  => $show_default_value,
      '#description'    => t('Show this menu item only on selected domain(s). If you select no domains, the menu item will be visible on all domains.'),
    );
    // Hide menu item per domain config.
    $form['domain_menu_access']['manage']['domain_menu_access_hide'] = array(
      '#type'           => 'checkboxes',
      '#title'          => t('Hide menu item on selected domains'),
      '#options'        => $options,
      '#default_value'  => $hide_default_value,
      '#description'    => t('Hide this menu item on selected domain(s). If you select no domains, the menu item will be visible on all domains.'),
    );
    // Add weight to Submit button, as it is not set by default by menu.module,
    // and because it sets weight of Delete button, it could happen that our
    // fieldset is displayed between Save and Delete buttons.
    $form['submit']['#weight'] = 9;
    // Add own validate function to avoid one menu item
    // being both displayed and hidden for the same domain.
    $form['#validate'][] = 'domain_menu_access_form_menu_edit_item_validate';
    // Make sure that our submit is called before menu_edit_item_submit()
    // from Drupal core menu.admin.inc, as there the changes are being saved.
    array_unshift($form['#submit'], 'domain_menu_access_form_menu_edit_item_submit');
  }
}

/**
 * Processes updated form submission and adds extra
 * information to menu $item['options'] element.
 */
function domain_menu_access_form_menu_edit_item_validate($form, &$form_state) {
  if ($form_state['submitted'] && user_access('administer menus per domain')) {
    // Make sure that menu item is not marked to be both shown and hidden for the same domain.
    foreach ($form_state['values']['domain_menu_access_hide'] as $key => $value) {
      if ($value && isset($form_state['values']['domain_menu_access_show'][$key]) && $form_state['values']['domain_menu_access_show'][$key]) {
        form_set_error('domain_menu_access_hide', t('Cannot both show and hide menu item for the same domain.'));
      }
    }
  }
}

/**
 * Processes updated form submission and adds extra
 * information to menu $item['options'] element.
 */
function domain_menu_access_form_menu_edit_item_submit($form, &$form_state) {
  if ($form_state['submitted'] && user_access('administer menus per domain')) {
    // Clear previous domain access values.
    $form_state['values']['options']['domain_menu_access'] = array(
      'show' => array(),
      'hide' => array(),
    );
    // Process showing menu item per domain.
    foreach ($form_state['values']['domain_menu_access_show'] as $key => $value) {
      if ($value) {
        $form_state['values']['options']['domain_menu_access']['show'][$key] = $key;
      }
    }
    // Process hiding menu item per domain.
    foreach ($form_state['values']['domain_menu_access_hide'] as $key => $value) {
      if ($value) {
        $form_state['values']['options']['domain_menu_access']['hide'][$key] = $key;
      }
    }
    // Verify that at least one checkbox for 'show' and for 'hide' was ticked.
    // Otherwise just unset relevant arrays, as no point in saving them then.
    if (count($form_state['values']['options']['domain_menu_access']['show']) == 0) {
      unset($form_state['values']['options']['domain_menu_access']['show']);
    }
    if (count($form_state['values']['options']['domain_menu_access']['hide']) == 0) {
      unset($form_state['values']['options']['domain_menu_access']['hide']);
    }
    if (count($form_state['values']['options']['domain_menu_access']) == 0) {
      unset($form_state['values']['options']['domain_menu_access']);
    }
    // Set 'alter' flag to true for this menu item if it was modified, 
    // otherwise hook_translated_menu_link_alter() will not be invoked:
    // http://api.drupal.org/api/drupal/modules--system--system.api.php/function/hook_translated_menu_link_alter/7
    if (isset($form_state['values']['options']['domain_menu_access'])) {
      $form_state['values']['options']['alter'] = TRUE;
    }
  }
}
