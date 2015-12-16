<?php
/**
Extra Theme settings
*/
function hertz_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['hertz_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hertz Theme Settings'),
    '#collapsible' => False,
	'#collapsed' => False,
	'#description'   => t("<a href='http://drupar.com/theme/hertzpro' target='_blank' style='color: #be6813'><strong>Upgrade to Pro Version for just $29</strong></a><br /><a href='http://drupar.com/hertz-theme-documentation' target='_blank'>Theme Documentation</a> || <a href='http://drupar.com/forum' target='_blank'>Theme Support Forum</a> || <a href='http://drupar.com/theme/hertz' target='_blank'>Theme Homepage</a> || <a href='http://demo.drupar.com/hertz' target='_blank'>Live Demo</a>"),
  );
  $form['hertz_settings']['social_profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Network'),
    '#collapsible' => True,
	'#collapsed' => True,
  );
    $form['hertz_settings']['social_profile']['social_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Twitter, Facebook, Google Plus icons in header'),
    '#default_value' => theme_get_setting('social_icons'),
	'#description'   => t("Check this option to show twitter, facebook, google plus icons in header. Uncheck to hide."),
  );
  $form['hertz_settings']['social_profile']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Page URL'),
    '#default_value' => theme_get_setting('facebook_username'),
	'#description'   => t("Enter your Facebook profile or page URL. Example: https://www.facebook.com/techyag"),
  );
  $form['hertz_settings']['social_profile']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Profile URL'),
    '#default_value' => theme_get_setting('twitter_username'),
	'#description'   => t("Enter your Twitter profile url. Example: http://www.twitter.com/techyag"),
  );
  $form['hertz_settings']['social_profile']['googleplus_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus Page URL'),
    '#default_value' => theme_get_setting('googleplus_username'),
	'#description'   => t("Enter your Google+ profile or page URL. Example: https://plus.google.com/u/0/118215801984610638841"),
  );
  $form['hertz_settings']['homepage_slider'] = array(
    '#type' => 'fieldset',
    '#title' => t('Homepage Slider'),
    '#collapsible' => True,
	'#collapsed' => True,
  );
    $form['hertz_settings']['homepage_slider']['home_slider'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show jQuery Slider on homepage'),
    '#default_value' => theme_get_setting('home_slider'),
	'#description'   => t("Check this option to show slider on homepage. Uncheck to hide."),
  );
  $form['hertz_settings']['homepage_slider']['slider_one_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Image of Slider One'),
    '#default_value' => theme_get_setting('slider_one_image'),
	'#description'   => t("Enter the url of first slider image."),
  );
  $form['hertz_settings']['homepage_slider']['slider_one_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of Slider One'),
    '#default_value' => theme_get_setting('slider_one_title'),
	'#description'   => t("Enter the title of first slider."),
  );
  $form['hertz_settings']['homepage_slider']['slider_one_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Description of Slider One'),
    '#default_value' => theme_get_setting('slider_one_desc'),
	'#description'   => t("Enter the description of first slider."),
  );
  $form['hertz_settings']['homepage_slider']['slider_two_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Image of Slider two'),
    '#default_value' => theme_get_setting('slider_two_image'),
	'#description'   => t("Enter the url of second slider image."),
  );
  $form['hertz_settings']['homepage_slider']['slider_two_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of Slider two'),
    '#default_value' => theme_get_setting('slider_two_title'),
	'#description'   => t("Enter the title of second slider."),
  );
  $form['hertz_settings']['homepage_slider']['slider_two_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Description of Slider two'),
    '#default_value' => theme_get_setting('slider_two_desc'),
	'#description'   => t("Enter the description of second slider."),
  );
  $form['hertz_settings']['homepage_slider']['slider_three_image'] = array(
    '#type' => 'textfield',
    '#title' => t('Image of Slider Three'),
    '#default_value' => theme_get_setting('slider_three_image'),
	'#description'   => t("Enter the url of third slider image."),
  );
  $form['hertz_settings']['homepage_slider']['slider_three_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title of Slider three'),
    '#default_value' => theme_get_setting('slider_three_title'),
	'#description'   => t("Enter the title of third slider."),
  );
  $form['hertz_settings']['homepage_slider']['slider_three_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Description of Slider three'),
    '#default_value' => theme_get_setting('slider_three_desc'),
	'#description'   => t("Enter the description of third slider."),
  );
  $form['hertz_settings']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => True,
	'#collapsed' => True,
  );
     $form['hertz_settings']['breadcrumb']['breadcrumb_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumb navigation'),
    '#default_value' => theme_get_setting('breadcrumb_link'),
	'#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['hertz_settings']['footer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer settings'),
    '#collapsible' => True,
	'#collapsed' => True,
  );
    $form['hertz_settings']['footer']['footer_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('footer_copyright'),
	'#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['hertz_settings']['upgrade_pro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upgrade to Pro Version'),
	'#description'   => t("Purchase pro version for advance features like Responsive layout, Unlimited colors option, Shortcodes etc. Click Below link to purchase and read more about pro version."),
    '#collapsible' => True,
	'#collapsed' => True,
  );
    $form['hertz_settings']['upgrade_pro']['upgrade'] = array(
	'#type' => 'link',
	'#title' => 'Purchase Hertz Pro',
	'#href'	=> 'http://drupar.com/theme/hertzpro',
  );
}
?>