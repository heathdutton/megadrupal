<?php

// $Id

/**
 * Form builder; Configure user revsion settings for this site.
 */
function user_revision_admin_settings() {
  $form['user_revision_by_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use revisions by default'),
    '#default_value' => variable_get('user_revision_by_default', 1),
    '#description' => t('If checked every save will save a new revision.'),
  );
  return system_settings_form($form);
}
