<?php
/**
 * @file
 * Obiba Agate Mdule.
 *
 * Copyright (c) 2015 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Provides settings pages.
 */
function obiba_agate_admin_settings() {

  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('OBiBa central authentication server (Agate)'),
    '#collapsible' => FALSE,
  );

  $info = variable_get_info('agate_url');
  $form['server']['agate_url'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#required' => TRUE,
    '#default_value' => variable_get_value('agate_url'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('agate_application_name');
  $form['server']['agate_application_name'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#required' => TRUE,
    '#default_value' => variable_get_value('agate_application_name'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('agate_application_key');
  $form['server']['agate_application_key'] = array(
    '#type' => 'password',
    '#title' => $info['title'],
    '#required' => TRUE,
    '#default_value' => variable_get_value('agate_application_key'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('User accounts'),
    '#collapsible' => FALSE,
  );

  $form['account']['obiba_user_register'] = array(
    '#markup' => t('A Drupal account is always created the first time a OBiBa user logs into the site. Specific Drupal roles can be applied on this account.'),
  );

  // Taken from Drupal's User module.
  $roles = array_map('check_plain', user_roles(TRUE));
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $info = variable_get_info('obiba_auto_assigned_role');
  $form['account']['obiba_auto_assigned_role'] = array(
    '#type' => 'checkboxes',
    '#title' => $info['title'],
    '#description' => $info['description'],
    '#default_value' => variable_get_value('obiba_auto_assigned_role', array()),
    '#options' => $roles,
    '#access' => user_access('administer permissions'),
    DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
  );
  return system_settings_form($form);
}
