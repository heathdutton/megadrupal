<?php

/**
 * Returns a form to exclude nedstat on certain
 * pages or user roles
 * 
 * @param $form
 * @param $form_state
 */
function nedstat_exclude_form($form, &$form_state) {
  $form['nedstat_settings_exclude_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude path settings'),
    '#description' => t('Add all pages that should be excluded. Specify pages by using their paths. Each line represent a exclude. An * is permitted'),
    '#default_value' => variable_get('nedstat_settings_exclude_pages', ''),
    '#rows' => 12,
  );
  
  $role_options = array_map('check_plain', user_roles());
  $form['nedstat_settings_exclude_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exclude roles settings'),
    '#description' => t('When a user logged in with an excluded user role, his/her click pattern will not be measured'),
    '#default_value' => variable_get('nedstat_settings_exclude_roles', array()),
    '#options' => $role_options,
  );
  
  return system_settings_form($form);
}