<?php

/**
 * Implements hook_process_html().
 */
function semantic_ui_omega_process_html(&$variables) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}
