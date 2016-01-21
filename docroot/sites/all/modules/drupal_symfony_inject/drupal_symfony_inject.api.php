<?php
/**
 * @file
 * API documentation for Drupal Symfony inject.
 */

/**
 * Declare Symfony YAML config files.
 *
 * @return array
 *   Where keys are file-names, and values are paths wrt the Drupal root.
 */
function hook_symfony_yaml_config() {
  return array(
    'my_module.yaml' => drupal_get_path('module', 'my_module'),
  );
}

/**
 * Declare Symfony DI Container parameters.
 *
 * These may appear in any YAML file as a placeholder.
 *
 * @param Symfony\Component\DependencyInjection\ContainerBuilder $container_builder
 *   The Symfony DI container being built up.
 *
 * @return array
 *   Placeholder keys and values. By convention, the period character "." is
 *   used to namespace the keys.
 */
function hook_symfony_yaml_config_params($container_builder) {
  $params = array();

  $params['my_namespace.component.key'] = "value";
  $params['my_namespace.component2.key'] = "value2";

  return $params;
}

/**
 * Alter Symfony DI Container parameters.
 *
 * @param array $params
 *   The array of DI container parameter key-values. Alter this by reference.
 */
function hook_symfony_yaml_config_params_alter(&$params) {
  $params['their_namespace.component.key'] = "valueOverride";
}

/**
 * Last chance to alter the DI Container before it is cached.
 *
 * @param Symfony\Component\DependencyInjection\ContainerBuilder $container_builder
 *   The Symfony DI container, fully built up from the YAML config.
 */
function hook_symfony_container_builder_alter($container_builder) {

}

/**
 * Add extra include paths into the class loader.
 *
 * @return array
 *   Where keys are namespaces, and values are system paths.
 */
function hook_namespace_register() {
  return array(
    'MyNamespace' => DRUPAL_ROOT . '/' . drupal_get_path('module', 'my_module') . '/src/MyNamespace',
  );
}
