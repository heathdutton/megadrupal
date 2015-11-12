<?php

/**
 * @file
 * Hooks provided by the inject system for container configuration support.
 */

use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\DependencyInjection\Reference;
use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Configures the container.
 *
 * It is only ever called once when the cache is empty.
 *
 * This hook can be implemented in a custom module.inject.inc file to register
 * compilation passes,services, other extensions, ...
 *
 * @param ContainerBuilder $container
 * @param string $phase
 *   The phase when the container is being built, either 'boot' or 'init'.
 */
function hook_inject_build(ContainerBuilder $container, $phase) {
  $loader = new XmlFileLoader(
    $container,
    new FileLocator(__DIR__.'/../config')
  );

  $loader->load('services.xml');

  if ($config['advanced']) {
    $loader->load('advanced.xml');
  }

  $container->setParameter('mailer.transport', 'sendmail');
  $container->register('mailer', 'Mailer')
    ->addArgument('%mailer.transport%');

  $container->register('newsletter_manager', 'NewletterManager')
    ->addArgument(new Reference('mailer'));
}

/**
 * @} End of "addtogroup hooks".
 */
