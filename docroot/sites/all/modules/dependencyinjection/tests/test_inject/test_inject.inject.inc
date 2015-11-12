<?php
/**
 * @file
 * Configures the container.
 */

use Symfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Implements hook_inject_build().
 */
function test_inject_inject_build(ContainerBuilder $container) {
  if ($container->hasParameter('dummy.param')) {
    // Redefines the parameter.
    $container->setParameter('dummy.param', 'dummy');
    // Adds a test assertion that the parameter has been overriden.
    $container->setParameter('dummy.build', TRUE);
  }
}
