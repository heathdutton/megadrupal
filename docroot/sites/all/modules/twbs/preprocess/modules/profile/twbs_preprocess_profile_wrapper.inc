<?php

/**
 * Process variables for profile-wrapper.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $content
 *
 * @see profile-wrapper.tpl.php
 */
function twbs_preprocess_profile_wrapper(&$variables) {
  $variables['current_field'] = '';
  if ($field = arg(1)) {
    $variables['current_field'] = $field;
    $variables['theme_hook_suggestions'][] = 'profile_wrapper__' . $field;
  }
}
