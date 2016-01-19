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
function obiba_agate_admin_pages_settings() {
  // Login page.
  $form['login'] = array(
    '#type' => 'fieldset',
    '#title' => t('OBiBa Login Page'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('obiba_login_page_title');
  $form['login']['obiba_login_page_title'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('obiba_login_page_title'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('obiba_login_username_label');
  $form['login']['obiba_login_username_label'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('obiba_login_username_label'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  $info = variable_get_info('obiba_login_button_caption');
  $form['login']['obiba_login_button_caption'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('obiba_login_button_caption'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );

  $info = variable_get_info('enable_form_tooltips');
  $form['login']['enable_form_tooltips'] = array(
    '#type' => 'radios',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('enable_form_tooltips'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#description' => $info['description'],
  );

  // Registration page.
  $form['register'] = array(
    '#type' => 'fieldset',
    '#title' => t('OBiBa Registration Page'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('obiba_register_page_title');
  $form['register']['obiba_register_page_title'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('obiba_register_page_title'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  // Reset password page.
  $form['reset'] = array(
    '#type' => 'fieldset',
    '#title' => t('OBiBa Reset Password Page'),
    '#collapsible' => FALSE,
  );
  $info = variable_get_info('obiba_reset_password_button_caption');
  $form['reset']['obiba_reset_password_button_caption'] = array(
    '#type' => 'textfield',
    '#title' => $info['title'],
    '#default_value' => variable_get_value('obiba_reset_password_button_caption'),
    '#maxlength' => 255,
    '#description' => $info['description'],
  );
  return system_settings_form($form);
}
