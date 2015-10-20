<?php
/**
 * @file
 * drupal_symfony_inject helper functions.
 */

// Add the Symfony components.
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\Config\FileLocator;
use Symfony\Component\DependencyInjection\Loader\YamlFileLoader;
use Symfony\Component\ClassLoader\ClassLoader;
// Add our own implementation of a Cached Proxy Manager.
use DrupalSymfonyInject\CachedInstantiator;

/**
 * Load Yaml Config.
 *
 * Load the YAML Configuration files for Symfony.
 */
function drupal_symfony_inject_load_yaml_config($proxies_path) {
  // Get the container from ContainerBuilder.
  $container_builder = new ContainerBuilder();
  $set_proxy_instantiator = FALSE;

  // Load Symfony Yaml config files defined in the modules.
  $symfony_yaml_files = module_invoke_all('symfony_yaml_config');

  if (!empty($symfony_yaml_files)) {
    foreach ($symfony_yaml_files as $symfony_yaml_file_name => $symfony_yaml_file_path) {
      if (file_exists("$symfony_yaml_file_path/$symfony_yaml_file_name")) {
        // Register the Yaml config files.
        // YAML Loader.
        $yaml_loader = new YamlFileLoader($container_builder, new FileLocator($symfony_yaml_file_path));
        $yaml_loader->load($symfony_yaml_file_name);
      }
    }
  }

  // Add build params.
  $build_yaml_config_params = module_invoke_all('symfony_yaml_config_params', $container_builder);

  $definitions = $container_builder->getDefinitions();
  foreach ($definitions as $definition) {
    // If we have the lazy set to true in the YAML, we'll need to initialise
    // the class as Lazy.
    if ($definition->isLazy()) {
      if ($set_proxy_instantiator === FALSE) {
        $set_proxy_instantiator = TRUE;
        $container_builder->setProxyInstantiator(new CachedInstantiator($proxies_path));
      }
      $definition->setLazy(TRUE);
    }
  }

  if (!empty($build_yaml_config_params)) {
    foreach ($build_yaml_config_params as $param_key => $param_value) {
      if (!empty($param_key)) {
        $container_builder->setParameter($param_key, $param_value);
      }
    }
  }

  // Last chance to do any additions to the container before compiling.
  drupal_alter('symfony_container_builder_alter', $container_builder);

  // Compile the container.
  $container_builder->compile();

  // Return the Container Builder.
  return $container_builder;
}

/**
 * Helper function which resgutered the class loader.
 */
function drupal_symfony_inject_register_class_loader($reset = FALSE) {
  // Initiate Class Loader.
  $custom_namespaces = NULL;
  $class_loader = new ClassLoader();

  // If not a reset, try and load the class loader from cache.
  if ($reset == FALSE) {
    // Invoke the namespaces defined by other modules.
    $custom_namespaces = cache_get(SYMFONY_CLASS_LOADER_RESGITER_CACHE_KEY);
    $custom_namespaces = isset($custom_namespaces->data) ? $custom_namespaces->data : FALSE;
  }

  // Rebuild the namespaces set it on cache.
  if (!$custom_namespaces) {
    // Using hook_namespace_register.
    $custom_namespaces = module_invoke_all('namespace_register');
    cache_set(SYMFONY_CLASS_LOADER_RESGITER_CACHE_KEY, $custom_namespaces);
  }

  if (!empty($custom_namespaces)) {
    foreach ($custom_namespaces as $custom_namespace => $path) {
      if (!empty($custom_namespace)) {
        $class_loader->addPrefix($custom_namespace, $path);
      }
    }

    // Use the include path.
    $class_loader->setUseIncludePath(TRUE);

    // Register namespace.
    $class_loader->register();

  }
}

/**
 * Get the Symfony container manager.
 */
function drupal_symfony_inject_get_container_builder($proxies_path) {
  static $symfony_container_builder;

  if (!isset($symfony_container_builder)) {
    $symfony_container_builder = cache_get(SYMFONY_DI_CONTAINER_CACHE_KEY);

    if (
      isset($symfony_container_builder->data) &&
      $symfony_container_builder->data instanceof ContainerBuilder
    ) {
      $symfony_container_builder = $symfony_container_builder->data;
    }
    else {
      // Build the container.
      $symfony_container_builder = drupal_symfony_inject_load_yaml_config($proxies_path);

      if (
        isset($symfony_container_builder) &&
        $symfony_container_builder instanceof ContainerBuilder
      ) {
        cache_set(SYMFONY_DI_CONTAINER_CACHE_KEY, $symfony_container_builder);
      }
    }
  }

  return $symfony_container_builder;
}

/**
 * Flush the cached Symfony container.
 */
function drupal_symfony_inject_flush_container() {
  cache_clear_all(SYMFONY_DI_CONTAINER_CACHE_KEY, 'cache');
}
