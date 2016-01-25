<?php
/**
Insert option to enter facebook and twitter username
*/
function fold_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['Fold_settings']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username'),
	'#description'   => t("Enter your Twitter username."),
  );
  $form['Fold_settings']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username'),
	'#description'   => t("Enter your Facebook username."),
  );
}
?>