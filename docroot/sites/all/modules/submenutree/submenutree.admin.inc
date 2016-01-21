<?php
/**
 * @file
 * Submenu Tree configuration options.
 */

function submenutree_configuration_form($form, &$form_state) {
  $form['block_title'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Block title'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['block_title']['submenutree_block_title'] = array(
    '#type' => 'radios',
    '#title' => t('Block title generation'),
    '#description' => t('Choose how the title of the sub content block and the sibling content block is generated when a sub or sibling title has not been set.'),
    '#default_value' => variable_get('submenutree_block_title', 'content_title'),
    '#options' => array(
      'leave_blank' => t('Leave blank'),
      'content_title' => t("Use the content's title"),
      'content_menu_direct_parent' => t("Use the title of the content's direct parent"),
      'content_menu_parent' => t("Use the title of the content's custom-level parent"),
    ),
  );
  $form['block_title']['submenutree_block_title_content_menu_parent_level'] = array(
    '#type' => 'select',
    '#title' => t('Block title menu parent level'),
    '#description' => t('Select a menu tree level to take the content menu parent from.'),
    '#default_value' => variable_get('submenutree_block_title_content_menu_parent_level', 1),
    '#options' => array(
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
      9 => 9,
      10 => 10,
    ),
    '#states' => array(
      'visible' => array(
        ':input[name="submenutree_block_title"]' => array(
          'value' => 'content_menu_parent',
        ),
      ),
    ),
  );
  $form['extended_menu'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Extended menu'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['extended_menu']['submenutree_extended_menu_name'] = array(
    '#type' => 'select',
    '#title' => t('Extended menu name'),
    '#options' => menu_get_menus(TRUE),
    '#default_value' => variable_get('submenutree_extended_menu_name', 'main_menu'),
    '#description' => t('Select a menu to provide extended links for. Extended links are all menu links that are two or more levels down in the menu tree.'),
   );
  $form['extended_menu']['submenutree_extended_menu_level'] = array(
    '#type' => 'select',
    '#title' => t('Extended menu level'),
    '#options' => array(
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
      9 => 9,
      10 => 10,
    ),
    '#default_value' => variable_get('submenutree_extended_menu_level', 1),
    '#description' => t('Select a menu tree level to start generating extended links from.'),
   );

  return system_settings_form($form);
}
