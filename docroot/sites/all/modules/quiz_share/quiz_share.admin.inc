<?php

function quiz_share_admin_settings() {
	$form['quiz_share_fb_appid'] = array(
    '#type' => 'textfield',
    '#title' => t('The appId of your facebook app'),
    '#default_value' => variable_get('quiz_share_fb_appid', ''),
    '#size' => 100,
    '#maxlength' => 300,
    '#required' => TRUE, 
    '#description' => t("the App ID/API Key of your app registered at https://developers.facebook.com/"),
  );
	return system_settings_form($form);
}
