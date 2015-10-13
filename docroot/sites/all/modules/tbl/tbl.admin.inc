<?php

/**
 * Settings form as implemented by hook_menu
 */

function tbl_admin_settings($form, &$form_state) {
  $form['tbl_block_num'] = array(
    '#type' => 'select',
    '#title' => t('Number of Taxonomy blocks'),
    '#description' => t('The total number of independent Taxonomy blocks you want.'),
    '#options' => range(0, TBL_BLOCK_MAX_NUM),
    '#default_value' => variable_get('tbl_block_num', TBL_BLOCK_DEFAULT_NUM),
    '#required' => true,
  );
  return system_settings_form($form);
}