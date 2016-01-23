<?php

/**
 * Process variables for user-profile-item.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $element
 *
 * @see user-profile-item.tpl.php
 */
function twbs_preprocess_user_profile_item(&$variables) {
  $variables['title'] = $variables['element']['#title'];
  $variables['value'] = $variables['element']['#markup'];
  $variables['attributes'] = '';
  if (isset($variables['element']['#attributes'])) {
    $variables['attributes'] = drupal_attributes($variables['element']['#attributes']);
  }
}
