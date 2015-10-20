<?php
/**
 * @file
 * panels_import.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function panels_import_default_panels_mini() {
  return _panels_import_load_defaults('panels', 'mini', 'mini');
}
