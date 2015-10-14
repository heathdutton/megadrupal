<?php
/**
 * @file
 * Hooks provided by Assetic.
 */

/**
 * Alter the filter instance before it is used.
 *
 * @param Assetic\Filter\FilterInterface $filter
 *   A FilterInterface instance.
 * @param array
 *   The context of the alter hook.
 */
function hook_assetic_filter_instance_alter(&$filter, $context) {
  // @TODO Add a very simple example.
}

/**
 * Alter a specifc filter instance before it is used.
 *
 * You must replace FILTER_ALIAS with the alias string of the filter.
 * This hook is used by each submodule to add the class of their Assetic plugin
 * to the main module. Check these modules for use cases of this hook.
 *
 * @param Assetic\Filter\FilterInterface $filter
 *   A FilterInterface instance.
 * @param array
 *   The context of the alter hook.
 */
function hook_assetic_filter_instance_FILTER_ALIAS_alter(&$filter, $context) {
  // @TODO Add a very simple example.
}

/**
 * Alter the filter information before it is cached and returned.
 *
 * @see hook_assetic_filters_info()
 */
function hook_assetic_filters_alter(&$filters) {
  // @TODO Add a very simple example.
}

/**
 * Register filters that can be used by Assetic through this hook.
 *
 * @return array
 *   An associative array where each key is the alias of a filter.
 *   Each filter arary should be defined like the following:
 *   - title: The title of the filter in the administrative forms.
 *   - class (required): The class for the filter to use.
 *   - constructor callback: A callback that returns an array of arguments for
 *     the constructor.
 *   - apply to: A regular expression that checks if the filter should be
 *     applied.
 *   - admin: A form callback that should return a settings form.
 *   - settings: An array with the filter's settings and their defaults.
 *   - debug mode: A boolean indicating that the filter should be
 *     applied in debug modus.
 */
function hook_assetic_filters_info() {
  return array(
    'yui_css' => array(
      'title' => 'CSS Compressor',
      'class' => 'Assetic\\Filter\\Yui\\CssCompressorFilter',
      'constructor callback' => 'assetic_css_compress_constructor_callback',
      'apply to' => '/.css$/',
      'admin' => 'assetic_sass_admin_form_scss',
      'settings' => array(
        'jar_path' => '/usr/local/jars/compressor.jar',
      ),
      'debug mode' => FALSE,
    ),
  );
}
