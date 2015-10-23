<?php

/**
 * @file
 * Administrative page callbacks for the userbar module.
 */

/**
 * Admin settings for Userbar
 */
function userbar_admin_settings() {

  //set the default refresh time for the site
  $form['userbar_refresh']=array('#type' => 'textfield', '#title' => t('Auto-refresh interval (s)'), '#size' => 5,
          '#description' => t('Set the auto-refresh interval in seconds.'),
          '#default_value' => _userbar_get_refresh_rate(FALSE));
  $form['userbar_usedock']=array('#type' => 'checkbox', '#title' => t('Dock Userbar'),
      '#default_value' => variable_get('userbar_usedock', 1), '#description' => t('Userbar will dock at the right bottom by default. You can change this via css styling.'));
  $form['userbar_select_content']=array('#markup' => '<ul class="action-links"><li>' . l(t('Set default userbar contents'), 'admin/config/user-interface/userbar/default') . '</li></ul>');
  return system_settings_form($form);
}
