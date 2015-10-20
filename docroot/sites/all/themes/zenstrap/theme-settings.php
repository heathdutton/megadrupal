<?php
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function zenstrap_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL)  {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }
  $form['zenstrap_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Zenstrap Settings'),
    '#weight' => 0,
  );
  $form['zenstrap_settings']['zenstrap_login_modal'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use modal dialog for login'),
    '#default_value' => theme_get_setting('zenstrap_login_modal'),
    '#description'   => t('Login links will be opened as a modal dialog'),
  );

  $menus = menu_get_menus();
  $form['zenstrap_settings']['zenstrap_single_accordion'] = array(
    '#type'          => 'select',
    '#options'       => $menus,
    '#title'         => t('Select menu to show as accordion'),
    '#default_value' => theme_get_setting('zenstrap_single_accordion'),
    '#description'   => t('Ensure that these menu block is added to the required region.'),
    '#empty_option' => t('Select'),
    '#empty_value' => 0,
  );
  $def_accor = theme_get_setting('zenstrap_menu_accordion');
  if (!is_array($def_accor)) {
    $def_accor = array();
  }
  $form['zenstrap_settings']['zenstrap_menu_accordion'] = array(
    '#type'          => 'checkboxes',
    '#options'       => $menus,
    '#title'         => t('Select menus to group as accordion'),
    '#default_value' => $def_accor,
    '#description'   => t('Ensure that these menu blocks are added to the same block region.'),
  );
 
  $form['zenstrap_settings']['zenstrap_disable_sticky_tables'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable sticky tables'),
    '#default_value' => theme_get_setting('zenstrap_disable_sticky_tables'),
    '#description'   => t('Selecting this will not generate sticky tables'),
  );
  $form['zenstrap_settings']['zenstrap_searchbox'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show search box in navigation'),
    '#default_value' => theme_get_setting('zenstrap_searchbox'),
    '#description'   => t('Shows the search box in top navigation bar'),
  );
  $form['zenstrap_settings']['zenstrap_horizontal_forms'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use horizontal forms'),
    '#default_value' => theme_get_setting('zenstrap_horizontal_forms'),
    '#description'   => t('Forms are styled in horizontal form style. See !link', 
      array('!link' => l(t('Base styles'), 'http://twitter.github.com/bootstrap/base-css.html#forms'))),
  );
  $form['zenstrap_settings']['zenstrap_modal_forms'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Launch Modal'),
    '#default_value' => theme_get_setting('zenstrap_modal_forms'),
    '#description'   => t('Enter the one URL per line that needs must be launched in modal frame'),
  );
}
