<?php

/**
 * @file
 * Configures the container.
 */

use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\DependencyInjection\Definition;
use Symfony\Component\DependencyInjection\Reference;

/**
 * Implements hook_inject_build().
 */
function doctrine_inject_build(ContainerBuilder $container) {
  global $databases;
  // Configures an instance of Doctrine DBAL Connection which is a wrapper
  // around the underlying driver connection (which is in this case a PDO instance).
  // This connection details identify the database to connect to as well as the
  // credentials to use. The connection details can differ depending on the used driver.
  $container->setDefinition('doctrine.dbal.connection', new Definition(
    'Doctrine\DBAL\Connection',
    array(doctrine_connection_params($databases['default']['default']))
  ))
  ->setFactoryClass('Doctrine\DBAL\DriverManager')
  ->setFactoryMethod('getConnection');

  // As we are early in the bootstrapping, we need to include file.inc in order to
  // call file_directory_temp() below.
  require_once(DRUPAL_ROOT . '/includes/file.inc');

  // Registers doctrine configuration.
  $container->setDefinition('doctrine.orm.configuration', new Definition('Doctrine\ORM\Configuration'))
  ->setFactoryClass('Doctrine\ORM\Tools\Setup')
  ->setFactoryMethod('createConfiguration')
  // Sets the directory where Doctrine generates any proxy classes.
  // A proxy object is an object that is put in place or used instead of the
  // "real" object. A proxy object can add behavior to the object being proxied
  // without that object being aware of it. In Doctrine 2, proxy object are used
  // to realize several features but mainly for transparent lazy-loading.
  ->addMethodCall('setProxyDir', array(variable_get('file_temporary_path', file_directory_temp())))
  // Sets the meta-data driver implementation that is used by Doctrine to
  // acquire the object-relational meta-data for entities. The meta-data driver
  // used here is the Drupal Schema API defined which is responsible to map
  // schema definitions, data type map, to Doctrine understandable language.
  ->addMethodCall('setMetadataDriverImpl', array(new Reference('doctrine.orm.driver')));

  // Registers doctrine entity manager.
  $container->setDefinition('doctrine.orm.manager', new Definition(
    'Doctrine\ORM\EntityManager',
    array(new Reference('doctrine.dbal.connection'), new Reference('doctrine.orm.configuration'))
  ))
  ->setFactoryClass('Doctrine\ORM\EntityManager')
  ->setFactoryMethod('create');
}

function doctrine_connection_params($database) {
  return array(
    'dbname' => $database['database'],
    'user' => $database['username'],
    'password' => $database['password'],
    'host' => $database['host'],
    'port' => $database['port'],
    'driver' => 'pdo_' . $database['driver'],
  );
}
