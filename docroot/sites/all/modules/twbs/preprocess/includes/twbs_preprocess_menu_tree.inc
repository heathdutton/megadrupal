<?php

/**
 * Implements twbs_preprocess_HOOK() for theme_menu_tree().
 */
function twbs_preprocess_menu_tree(&$variables) {
  $variables['tree'] = $variables['tree']['#children'];
}
