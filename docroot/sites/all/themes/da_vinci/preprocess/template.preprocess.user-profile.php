<?php
/**
 * @file
 * Preproccess functions for User Profile.
 */

/**
 * Implement da_vinci_preprocess_user_profile().
 */
function da_vinci_preprocess_user_profile(&$vars) {
  if (!empty ($vars['elements']['#view_mode'])) {
    // Add suggestion for user entity view modes:
    $vars['theme_hook_suggestions'][] = 'user_profile__' . $vars['elements']['#view_mode'];

    // Add view-mode class:
    $vars['classes_array'][] = 'user-' . drupal_clean_css_identifier($vars['elements']['#view_mode']);
  }
}
