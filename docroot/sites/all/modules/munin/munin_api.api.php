<?php

/**
 * @file
 * Hooks provided by the Munin Api module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows modules to define their own munin plugins.
 *
 * Posible proprieties of plugins:
 * - #title: Title of the graph.
 * - #description: The description of the plugin used in the admin area.
 * - #vlabel: Munin plugin label.
 * - #config: Array of additional config data printed when called with config
 * argument.
 *
 * Possible proprieties of plugin fields:
 * - #label: Lable of the field.
 * - #description: The description of the field used in drupal admin area.
 * - #sql: An sql query which should return the current value of the field.
 * - #statement: The same as #sql. Left for backwords compatibility.
 * - #callback: Function callback that should return current value of the field.
 * If both #sql and #callback are defined, #sql takes precedence and sets the
 * value.
 * - #file: File in which the callback is defined.
 * - #config: Array of field proprieties sent to munin. Ex:
 *   - type: GAUGE, COUNTER, DERIVE or ABSOLUTE;
 *   - warning: Value over which a warning is shown in the graph.
 *   - critical: Value over which a error is shown in the graph.
 *   - ...any available Munin field propriety can be setted here.
 *
 * @return array
 *   Array of plugins definitions.
 */
function hook_muninplugin() {
  $plugins = array();

  // Set the category. There will be a munin element for each category.
  $plugins['user'] = array(
    '#title' => t('User information'),
    '#description' => t('Information about the users on your site.'),
    '#vlabel' => t('Number'),
  );
  // Add the data that fits into that category.
  $plugins['user']['curuser'] = array(
    '#label' => t('Currently logged in user'),
    '#description' => t('This item shows the number of currently logged in users.'),
    '#statement' => 'select count(uid) from {users} where unix_timestamp() - access < (60*5)',
  );
  $plugins['user']['newuser'] = array(
    '#label' => t('New users'),
    '#description' => t('This item shows the number of users that have been created.'),
    '#statement' => 'select count(uid) from {users} where unix_timestamp() - created < (60*5)',
  );

  $watchdog_severity_levels = watchdog_severity_levels();

  $plugins['errors'] = array(
    '#title' => t('Error information'),
    '#description' => t('Information about Drupal errors.'),
    '#vlabel' => t('Number'),
  );

  foreach ($watchdog_severity_levels as $id => $severity) {
    $plugins['errors'][$severity] = array(
      '#label' => t('!severity', array('!severity' => ucfirst($severity))),
      '#description' => t('Number errors of !severity severity.'),
      '#config' => array(
        'type' => 'DERIVE',
        'min' => 0,
        'draw' => 'LINE1'
      ),
      '#statement' => 'select count(wid) from {watchdog} where severity = ' . $id . ';',
    );
  }

  return $plugins;
}

/**
 * Alter plugins definition.
 *
 * @param array $plugins
 *   Array of alterable plugin definitions.
 */
function hook_muninplugin_alter($plugins) {
  $plugins['user']['#title'] = 'New graph title.';
}

/**
 * @} End of "addtogroup hooks".
 */
