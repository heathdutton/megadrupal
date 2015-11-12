<?php

/**
 * @file
 * Administration page callbacks for Moodle Connector.
 */

/**
 * Implements hook_admin_settings().
 */
function moodle_connector_admin_settings() {

  // Moodle Database Settings.
  $form['moodle_connector_db_type'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Database Type'),
    '#default_value' => variable_get('moodle_connector_db_type', 'mysql'),
    '#description'   => t("The type of the Moodle database. Common values for this field are 'mysql' or 'pgsql'."),
    '#size'          => 20,
    '#required'      => TRUE,
  );
  $form['moodle_connector_db_server'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Database Server'),
    '#default_value' => variable_get('moodle_connector_db_server', 'localhost'),
    '#description'   => t('The database host where Moodle is installed.'),
    '#required'      => TRUE,
  );
  $form['moodle_connector_db_port'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Database TCP Port'),
    '#default_value' => variable_get('moodle_connector_db_port', '3306'),
    '#description'   => t('The TCP port number of the database server. Default for MySQL is 3306.'),
    '#size'          => 20,
    '#required'      => TRUE,
  );
  $form['moodle_connector_db_database'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Database Name'),
    '#default_value' => variable_get('moodle_connector_db_database'),
    '#description'   => t('The database name where Moodle is installed.'),
    '#size'          => 20,
    '#required'      => TRUE,
  );
  $form['moodle_connector_db_prefix'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Database Prefix'),
    '#default_value' => variable_get('moodle_connector_db_prefix', 'mdl_'),
    '#description'   => t("The prefix for the Moodle database tables. Default is 'mdl_'."),
    '#size'          => 20,
    '#required'      => FALSE,
  );
  $form['moodle_connector_db_username'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Database User'),
    '#default_value' => variable_get('moodle_connector_db_username'),
    '#description'   => t('User to access to the Moodle database.'),
    '#size'          => 20,
    '#required'      => TRUE,
  );
  $form['moodle_connector_db_password'] = array(
    '#type'          => 'password',
    '#title'         => t('Database Password'),
    '#default_value' => variable_get('moodle_connector_db_password'),
    '#description'   => t('Password for the database user.'),
    '#size'          => 20,
    '#required'      => TRUE,
  );

  // Moodle URL.
  $form['moodle_connector_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Moodle URL'),
    '#default_value' => variable_get('moodle_connector_url'),
    '#description'   => t("The full URL of the Moodle instance. Example: 'http://moodle.example.com'. Don't put the trailing '/'."),
    '#required'      => FALSE,
  );

  return system_settings_form($form);
}
