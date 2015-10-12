<?php
/**
Insert option to enter facebook and twitter username
*/
function splendio_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['splendio_settings']['footer_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('footer_copyright'),
	'#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['splendio_settings']['social_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show twitter, facebook, rss icons in header'),
    '#default_value' => theme_get_setting('social_icons'),
    '#description'   => t("Check this option to show twitter, facebook, rss icons in header. Uncheck to hide."),
  );
  $form['splendio_settings']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username'),
	'#description'   => t("Enter your Twitter username."),
  );
  $form['splendio_settings']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username'),
	'#description'   => t("Enter your Facebook username."),
  );
  $form['splendio_settings']['footer_megamenu'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer Mega menu'),
  );
  $form['splendio_settings']['footer_megamenu']['footer_mega_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Mega Menu in Footer'),
    '#default_value' => theme_get_setting('footer_mega_menu'),
    '#description'   => t("Check this option to show Mega menu in footer. Uncheck to hide."),
  );
  $form['splendio_settings']['footer_megamenu']['footer_mega_menu_1'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Mega-menu 1'),
    '#default_value' => theme_get_setting('footer_mega_menu_1'),
	'#description'   => t("Enter the Mega-Menu 1 region's html code."),
  );
  $form['splendio_settings']['footer_megamenu']['footer_mega_menu_2'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Mega-menu 2'),
    '#default_value' => theme_get_setting('footer_mega_menu_2'),
	'#description'   => t("Enter the Mega-Menu 2 region's html code."),
  );
  $form['splendio_settings']['footer_megamenu']['footer_mega_menu_3'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Mega-menu 3'),
    '#default_value' => theme_get_setting('footer_mega_menu_3'),
	'#description'   => t("Enter the Mega-Menu 3 region's html code."),
  );
  $form['splendio_settings']['footer_megamenu']['footer_mega_menu_4'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Mega-menu 4'),
    '#default_value' => theme_get_setting('footer_mega_menu_4'),
	'#description'   => t("Enter the Mega-Menu 4 region's html code."),
  );
  $form['splendio_settings']['footer_megamenu']['footer_mega_menu_5'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer Mega-menu 5'),
    '#default_value' => theme_get_setting('footer_mega_menu_5'),
	'#description'   => t("Enter the Mega-Menu 5 region's html code."),
  );
}
?>