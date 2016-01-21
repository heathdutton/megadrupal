<?php

/**
 * @file
 *   Drush hooks for hosting_variables.
 *
 * http://www.computerminds.co.uk/articles/storing-data-aegir
 */

/**
 * Implementation of hook_hosting_site_context_options().
 */
function hosting_variables_hosting_site_context_options(&$task) {
  $variables = hosting_variables_get_site_variables($task->ref->nid);

  // If the site has no variables, it will be an empty array.
  $task->context_options['hosting_variables_variables'] = $variables;
}

/**
 * Implements hook_drush_context_import().
 */
function hosting_variables_drush_context_import($context, &$node) {
  if ($context->type == 'site') {
    if (isset($context->hosting_variables_variables) && !empty($context->hosting_variables_variables)) {
      $node->hosting_variables_variables = $context->hosting_variables_variables;
    }
  }
}
