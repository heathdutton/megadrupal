<?php

/**
 * Implement pagecallback for sweetcapcha admin settings.
 *
 * 
 */
function sweetcaptcha_admin_settting_form($form, $form_state) {
  $form['sweetcaptcha_app_id'] = array(
    "#type" => "textfield",
    "#title" => t("App ID"),
    "#description" => t('Enter APP ID provided by Sweetcapcha'),
    "#default_value" => variable_get("sweetcaptcha_app_id", NULL));
  $form['sweetcaptcha_key'] = array(
    "#type" => "textfield",
    "#title" => t("App Key"),
    "#description" => t('Enter APP KEY Provided by Sweetcapcha'),
    "#default_value" => variable_get("sweetcaptcha_key", NULL));
  $form['sweetcaptcha_secret_key'] = array(
    "#type" => "textfield",
    "#title" => t("Secret Key"),
    "#description" => t('Enter SECRET KEY provided by Sweetcapcha'),
    "#default_value" => variable_get("sweetcaptcha_secret_key", NULL));
  $form['sweetcaptcha_public_url'] = array(
    "#type" => "textfield",
    "#title" => t("Enter the library file name ex: sweetcaptcha.php"),
    "#default_value" => variable_get("sweetcaptcha_public_url", 'sweetcaptcha.php'));
  $form['sweetcaptcha_form_id'] = array(
    "#type" => "textfield",
    "#title" => t("Forms to be sweetcapcha added"),
    "#description" => t('Forms to which you want to include sweeetcapcha, Enter "," Comma separated form ids Ex: user_login for user login form'),
    "#default_value" => variable_get("sweetcaptcha_form_id", NULL));
  return system_settings_form($form);
}
