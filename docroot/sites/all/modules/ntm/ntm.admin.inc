<?php

/**
 * Settings form as implemented by hook_menu
 */
function ntm_admin_settings($form, &$form_state) {
  $form['ntm_menu_num'] = array(
    '#type' => 'select',
    '#title' => t('Number of Metro Menus'),
    '#description' => t('The total number of independent Metro Menus blocks you want.'),
    '#options' => range(0, NTM_MAX_MENU_NUM),
    '#default_value' => variable_get('ntm_menu_num', NTM_DEFAULT_MENU_NUM),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}