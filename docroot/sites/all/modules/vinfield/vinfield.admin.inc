<?php
/**
 * @file
 *
 * Contains admin page callbacks
 */

/**
 * Settings page callback
 */
function vinfield_settings_form($form, &$form_state) {
  $providers = vinfield_providers();
  foreach ($providers as $name => $info) {
    if (!empty($info['page callback'])) {
      $form[$name]['#markup'] = l(!empty($info['title']) ? $info['title'] : $name, 'admin/config/content/vinfield/' . $name);
      if (!empty($info['description'])) {
        $form[$name]['#markup'] .= '<div class="description">' . $info['description'] . '</div>';
      }
    }
  }

  return $form;
}