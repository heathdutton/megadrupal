drupal_symfony_inject.module
============================

Installation
============

You must create a directory to store proxy classes. The default location of this
directory is /tmp/doctrine/proxies, but you can specify an alternative via the
following variable:

    symfony_class_loader_proxies_dir


Hooks
=====

1. hook_namespace_register : Register custom namespaces you need from your
                             module. The implementation expects a key value paired
                             array. The key should be the namespace and the value
                             should be the path to the namespace.

2. hook_symfony_yaml_config : Define custom paths to your Symfony DI Yaml config
                              files. Implementation should return an array with
                              key value pair, The key should be the Yaml file
                              name and the value should be the path to the Yaml
                              file.

                              ex: array(
                                'config.yaml' => '/config/yaml'
                              );

                              In the  above example, the config file is config.yaml
                              and the path to it is '/config/yaml'.

3. hook_symfony_yaml_config_params : Any custom parameters you need to set in the Yaml
                                config. Again this should return an array where
                                keys should be the parameters and the values as the
                                values for the parameter.

Drupal alters
=============

Module provide one drupal_alter - symfony_container_builder_alter to do any
final changes to the Symfony container before compiling and caching it.
