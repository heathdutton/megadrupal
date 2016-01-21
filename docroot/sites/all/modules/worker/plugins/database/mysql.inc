<?php
/**
 * @file
 * Provide mysql database interface plugin.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('mysql'),
  'description' => t('Provide mysql database interface plugin.'),
  'create' => 'worker_mysql_create',
  'drop' => 'worker_mysql_drop',
);

function worker_mysql_create($connection, $database) {
  return (bool) $connection->query("CREATE DATABASE `$database`");
}

function worker_mysql_drop($connection, $database) {
  return (bool) $connection->query("DROP DATABASE IF EXISTS `$database`");
}
