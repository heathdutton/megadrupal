<?php

/**
 * Implements twbs_bootstrap_THEME_theme_registry().
 */
function twbs_bootstrap_nav_theme_registry(&$theme_registry) {
  $theme_registry['twbs_bootstrap_nav'] = array(
    'base hook' => 'links',
    'file' => 'theme/theme_twbs_bootstrap_nav.inc',
  );
}
