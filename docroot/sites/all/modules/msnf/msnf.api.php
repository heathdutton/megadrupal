<?php

/**
 * @file
 * Hooks provided by the Multistep Nodeform module.
 */


/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the step definition that will be attached to a form.
 *
 * @param <array> $steps_cached
 *   List of steps. @see msnf_read_steps() for structure details.
 * @param <string> $entity_type
 *   Name of entity type the steps are defined for (for example "node").
 * @param <string> $bundle
 *   Name of bundle the steps are defined for (for example "article").
 * @param <array> $form
 *   The form the steps will be attached to (for example the node_form).
 * @param <array> $form_state
 *   Current state of the form.
 */
function hook_msnf_info_steps_alter(&$steps_cached, $entity_type, $bundle, $form, $form_state) {
  if ($entity_type == 'node' && $bundle == 'article' && !empty($form['#node']->nid)) {
    // Disable steps on node/[nid]/edit for nodes of type "article".
    return array();
  }
}

/**
 * @} End of "addtogroup hooks".
 */
