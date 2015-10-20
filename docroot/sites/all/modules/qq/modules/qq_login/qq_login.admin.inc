<?php

/**
 * @file
 * Admin page callbacks for the qq_login module.
 */

/**
 * qq login openapi config.
 */
function qq_login_setting() {
  // Referral income ratio.
  $form['qq_login_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('qq login openapi config'),
  );
  $form['qq_login_config']['appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Appid'),
    '#default_value' => variable_get('appid', '222222'),
    '#required' => TRUE,
  );
  $form['qq_login_config']['appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Appkey'),
    '#default_value' => variable_get('appkey', '005831692a444765a0db25a4a5ac052c'),
    '#required' => TRUE,
  );
  $form['qq_login_config']['scope'] = array(
    '#type' => 'textfield',
    '#title' => t('Scope'),
    '#default_value' => variable_get('scope', 'get_user_info,add_share,list_album,add_album,upload_pic,add_topic,add_one_blog,add_weibo'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
