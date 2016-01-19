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
 * Implements hook_variable_info().
 */
function obiba_agate_variable_info($options) {
  $variable['agate_url'] = array(
    'title' => t('Agate address'),
    'description' => t('URL of the Agate server. Note that cross-domain is not supported. Example: https://agate.example.org:8444'),
    'type' => 'string',
    'default' => 'https://localhost:8444',
  );
  $variable['agate_application_name'] = array(
    'title' => t('Application name'),
    'description' => t('The name under which the Drupal server is known by Agate.'),
    'type' => 'string',
    'default' => 'drupal',
  );
  $variable['agate_application_key'] = array(
    'title' => t('Application key'),
    'description' => t('The key used by the Drupal server when issuing requests to Agate.'),
    'type' => 'string',
    'default' => 'changeit',
  );
  $variable['obiba_auto_assigned_role'] = array(
    'title' => t('Roles'),
    'description' => t('The selected roles will be automatically assigned to each OBiBa user on login. Use this to automatically give OBiBa users additional privileges or to identify OBiBa users to other modules.'),
    'type' => 'options',
    'default' => array(),
  );
  $variable['obiba_login_page_title'] = array(
    'title' => t('Page title'),
    'description' => t('User Login page title.'),
    'type' => 'string',
    'default' => 'User account',
  );
  $variable['obiba_login_username_label'] = array(
    'title' => t('Username label'),
    'description' => t('Username/password input label.'),
    'type' => 'string',
    'default' => 'Username or E-mail',
  );
  $variable['enable_form_tooltips'] = array(
    'title' => t('Enable / Disable tooltips forms'),
    'description' => t('Enable / Disable tooltips forms'),
    'type' => 'options',
    'default' => array(),
  );
  $variable['obiba_login_button_caption'] = array(
    'title' => t('Log in button'),
    'description' => t('Log in button caption'),
    'type' => 'string',
    'default' => 'Log in',
  );
  $variable['obiba_register_page_title'] = array(
    'title' => t('Page title'),
    'description' => t('User registration page title.'),
    'type' => 'string',
    'default' => 'User registration',
  );
  $variable['obiba_reset_password_button_caption'] = array(
    'title' => t('Email button'),
    'description' => t('Email button caption.'),
    'type' => 'string',
    'default' => 'E-mail new password',
  );
  return $variable;
}
