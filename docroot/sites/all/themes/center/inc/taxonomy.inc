<?php

/**
 * Implements hook_preprocess_node().
 */
function center_preprocess_taxonomy_term(&$vars) {
  // Set shortcut variables. Hooray for less typing!
  $vocab = $vars['vocabulary_machine_name'];
  $mode = $vars['view_mode'];
  // Create a development print variable.
  $vars['dev_deets'] = theme_get_setting('dev_mode') ?
    '<!-- Vocab: ' . $vocab . ' | View Mode: ' . $mode . ' -->' : '';

  // Add a theme hook suggestion based on view mode.
  $vars['theme_hook_suggestions'][] = 'taxonomy_term__' . $mode;
  $vars['theme_hook_suggestions'][] = 'taxonomy_term__' . $mode . '__' . $vocab;
}
