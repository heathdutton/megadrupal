<?php
/**
 * @file
 */


/**
 * Implements hook_slate_twig_loaders().
 *
 * @return array $loaders
 */
function hook_slate_twig_loaders() {
  $loaders = array();

  $loaders['my-loader'] = new Twig_Loader_Array(array(
    'hello-world' => 'Hello world, this is {{ name }} calling!',
  ));

  return $loaders;
}

/**
 * Implements hook_slate_twig_loaders_alter().
 *
 * @param array $loaders
 */
function hook_slate_twig_loaders_alter(array $loaders) {
  unset($loaders['my-loader']);
}

/**
 * Implements hook_slate_twig_environment_alter().
 *
 * @param array $environment
 */
function hook_slate_twig_environment_alter(array &$environment) {
  $environment['debug'] = FALSE;
}

/**
 * Implements hook_slate_twig_alter().
 *
 * @param $twig
 */
function hook_slate_twig_alter($twig) {
  $twig->addExtension(new Twig_Extension_Debug());
}
