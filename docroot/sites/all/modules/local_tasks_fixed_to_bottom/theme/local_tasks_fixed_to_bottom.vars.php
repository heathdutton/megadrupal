<?php

/**
 * @file
 * Processes variables for local_tasks_fixed_to_bottom.tpl.php.
 */

/**
 * Add tabs html.
 *
 * @see local_tasks_fixed_to_bottom.tpl.php
 */
function template_preprocess_local_tasks_fixed_to_bottom_local_tasks(&$variables) {
  $variables['local_tasks'] = theme('menu_local_tasks', $variables);
}
