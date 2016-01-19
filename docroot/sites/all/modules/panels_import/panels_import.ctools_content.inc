<?php

/**
 * Implements hook_default_ctools_custom_content().
 */
function panels_import_default_ctools_custom_content() {
  return _panels_import_load_defaults('ctools', 'ctools_contents', 'content');
}
