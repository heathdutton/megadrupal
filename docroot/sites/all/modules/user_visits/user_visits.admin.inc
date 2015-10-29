<?php

/**
 * @file
 * User visits - admin pages
 */

/**
 * Settings page
 */
function user_visits_settings() {
  $form['user_activity'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#description' => t("Choose if you want the visitors to be displayed on the user's profile page or not. Alternatively you may use the provided !blocks to display a user's visitors.", array('!blocks' => l(t('blocks'), 'admin/structure/block'))),
  );
  $form['user_activity']['user_visits_display'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('user_visits_display', 0),
    '#options' => array(t("Don't display."), t('Display on user profile page')),
  );

  $roles = user_roles(TRUE);
  $form['user_activity_role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role visibility'),
    '#description' => t("Choose roles and visits of selected roles will be not shown in user visit block."),
  );
  $form['user_activity_role']['user_visits_hidden_roles'] = array(
    '#type' => 'select',
    '#title' => t('Hidden Roles'),
    '#description' => t('visits of selected roles will be not shown in user visit block.'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#default_value' => variable_get('user_visits_hidden_roles', array()),
  );
  return system_settings_form($form);
}
