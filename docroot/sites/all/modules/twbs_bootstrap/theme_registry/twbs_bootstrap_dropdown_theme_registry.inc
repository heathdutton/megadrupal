<?php

/**
 * Implements twbs_bootstrap_THEME_theme_registry().
 */
function twbs_bootstrap_dropdown_theme_registry(&$theme_registry) {
  $theme_registry['twbs_bootstrap_dropdown'] = array(
    'variables' => array(
      'html_tag' => NULL,
      'link' => NULL,
      'links' => NULL,
    ),
    'file' => 'theme/theme_twbs_bootstrap_dropdown.inc',
  );
}
