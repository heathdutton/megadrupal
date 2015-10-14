<?php

/**
 * Implements hook_views_plugins().
 */
function views_node_type_default_arg_views_plugins() {
  return array(
    'argument default' => array(
      'param' => array(
        'title' => t('Content type from URL'),
        'handler' => 'views_node_type_default_arg_plugin_argument_default_param',
      ),
    ),
  );
}

/**
 * Default argument plugin to use the current node's type.
 *
 * @ingroup views_argument_default_plugins
 */
class views_node_type_default_arg_plugin_argument_default_param extends views_plugin_argument_default {
  /**
   * Return the default argument.
   */
  function get_argument() {
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      $node = node_load(arg(1));
      return $value = $node->type;
    }
    else {
      return $value = FALSE;
    }
  }
}
