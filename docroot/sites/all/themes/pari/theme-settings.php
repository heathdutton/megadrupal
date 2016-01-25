<?php
/**
Extra Pari Theme settings
*/
function pari_form_system_theme_settings_alter(&$form, &$form_state) {
	$form['pari_settings'] = array(
		'#type' => 'fieldset',
		'#title' => t('Pari Theme Settings'),
		'#collapsible' => False,
		'#collapsed' => False,
		'#description'   => t("<a href='http://www.drupar.com/theme/pari' target='_blank'>Theme Homepage</a> || <a href='http://demo.drupar.com/pari' target='_blank'>Theme Live Demo</a> || <a href='http://www.drupar.com/pari-theme-documentation' target='_blank'>Theme Documentation</a> || <a href='http://www.drupar.com/forum' target='_blank'>Theme Support</a><br /><a href='http://drupar.com/theme/paripro' target='_blank' style='color: #be6813'>Upgrade to Pari Pro Version for just $29</a>"),
	);
	$form['pari_settings']['tabs'] = array(
		'#type' => 'vertical_tabs',
	);
	$form['pari_settings']['tabs']['header'] = array(
		'#type' => 'fieldset',
		'#title' => t('Header'),
		'#collapsible' => True,
		'#collapsed' => True,
	);
	$form['pari_settings']['tabs']['header']['site_logo'] = array(
		'#type' => 'select',
		'#title' => t('Site Logo'),
		'#default_value' => theme_get_setting('site_logo'),
		'#description'   => t("From the drop-down menu, select the type of logo for your website. Please <a href='http://www.drupar.com/pari-theme-documentation/website-logo' target='_blank'>read tutorial</a>."),
		'#options' => array(
			'logo_image' => t('Logo Image'),
			'logo_text' => t('Site Name Text'),
		),
	);
	$form['pari_settings']['tabs']['social'] = array(
		'#type' => 'fieldset',
		'#title' => t('Social Network'),
		'#collapsible' => True,
		'#collapsed' => True,
	);
	$form['pari_settings']['tabs']['social']['show_icons'] = array(
		'#type' => 'fieldset',
		'#title' => t('Show / Hide Icons'),
		'#collapsible' => False,
		'#collapsed' => False,
	);
	$form['pari_settings']['tabs']['social']['show_icons']['social_icons'] = array(
		'#type' => 'checkbox',
		'#title' => t('Show Twitter, Facebook, Google Plus icons in footer'),
		'#default_value' => theme_get_setting('social_icons'),
		'#description'   => t("Check this option to show twitter, facebook, google plus icons in footer. Uncheck to hide all icons."),
	);
	$form['pari_settings']['tabs']['social']['social_url'] = array(
		'#type' => 'fieldset',
		'#title' => t('Social Profile URLs'),
		'#collapsible' => False,
		'#collapsed' => False,
	);
	$form['pari_settings']['tabs']['social']['social_url']['twitter_username'] = array(
		'#type' => 'textfield',
		'#title' => t('Twitter Profile URL'),
		'#default_value' => theme_get_setting('twitter_username'),
		'#description'   => t("Enter your Twitter profile url. Example: http://www.twitter.com/DewDots"),
	);
	$form['pari_settings']['tabs']['social']['social_url']['facebook_username'] = array(
		'#type' => 'textfield',
		'#title' => t('Facebook Page URL'),
		'#default_value' => theme_get_setting('facebook_username'),
		'#description'   => t("Enter your Facebook profile or page URL. Example: https://www.facebook.com/iDewDot"),
	);
	$form['pari_settings']['tabs']['social']['social_url']['googleplus_username'] = array(
		'#type' => 'textfield',
		'#title' => t('Google Plus Page URL'),
		'#default_value' => theme_get_setting('googleplus_username'),
		'#description'   => t("Enter your Google+ profile or page URL. Example: https://plus.google.com/u/0/118215801984610638841"),
	);
	$form['pari_settings']['tabs']['social']['hide_icons'] = array(
		'#type' => 'fieldset',
		'#title' => t('Hide Individual Icons'),
		'#collapsible' => False,
		'#collapsed' => False,
		'#description'   => t("Managing individual social icon is actually feature of our premium <a href='http://www.drupar.com/theme/paripro' target='_blank'>PariPro theme</a>. But if you want to remove any particular social icon, please ask on our <a href='http://www.drupar.com/forum' target='_blank'>support forum</a>. We will help you to get this done."),
	);
	$form['pari_settings']['tabs']['social']['add_icons'] = array(
		'#type' => 'fieldset',
		'#title' => t('Add More Social Icons'),
		'#collapsible' => False,
		'#collapsed' => False,
		'#description'   => t("This feature is available in <a href='http://www.drupar.com/theme/paripro' target='_blank'>PariPro version</a>. You can purchase pro version for $29 (one time) only."),
	);
	$form['pari_settings']['tabs']['slider'] = array(
		'#type' => 'fieldset',
		'#title' => t('Homepage Slider'),
		'#collapsible' => True,
		'#collapsed' => True,
	);
	$form['pari_settings']['tabs']['slider']['enable_slider'] = array(
		'#type' => 'checkbox',
		'#title' => t('Enable homepage slider'),
		'#default_value' => theme_get_setting('enable_slider'),
		'#description'   => t("Check this option to show slider on homepage. Uncheck to hide."),
	);
	$form['pari_settings']['tabs']['slider']['slider_code'] = array(
		'#type' => 'textarea',
		'#title' => t('Slider Code'),
		'#default_value' => theme_get_setting('slider_code'),
		'#description'   => t("Enter slider code to override default slider. Please refer <a href='http://www.drupar.com/pari-theme-documentation/how-manage-homepage-slider' target='_blank'>here</a> for details about creating slider."),
	);

	$form['pari_settings']['tabs']['breadcrumb'] = array(
		'#type' => 'fieldset',
		'#title' => t('Breadcrumb'),
		'#collapsible' => True,
		'#collapsed' => True,
	);
	$form['pari_settings']['tabs']['breadcrumb']['breadcrumb_link'] = array(
		'#type' => 'checkbox',
		'#title' => t('Show breadcrumb navigation'),
		'#default_value' => theme_get_setting('breadcrumb_link'),
		'#description'   => t("Check this option to show breadcrumb navigation link in header. Uncheck to hide.<br /> Please <a href='http://www.drupar.com/pari-theme-documentation/breadcrumb-navigation-link' target='_blank'>read tutorial</a>."),
	);
	$form['pari_settings']['tabs']['footer'] = array(
		'#type' => 'fieldset',
		'#title' => t('Footer'),
		'#collapsible' => TRUE,
		'#collapsed' => FALSE,
	);
	$form['pari_settings']['tabs']['footer']['footer_copyright'] = array(
		'#type' => 'fieldset',
		'#title' => t('Show / Hide copyright text in footer'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE,
	);
	$form['pari_settings']['tabs']['footer']['footer_copyright']['footer_copyright_show'] = array(
		'#type' => 'checkbox',
		'#title' => t('Show or hide copyright text in footer'),
		'#default_value' => theme_get_setting('footer_copyright_show'),
		'#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
	);
	$form['pari_settings']['tabs']['footer']['footer_copyright']['footer_copyright_text'] = array(
		'#type' => 'textfield',
		'#title' => t('Footer Copyright Text'),
		'#default_value' => theme_get_setting('footer_copyright_text'),
		'#description'   => t("Enter your custom website copyright text to override default copyright text. Please <a href='http://www.drupar.com/pari-theme-documentation/how-manage-footer-copyright-text' target='_blank'>read tutorial</a>."),
	);
	$form['pari_settings']['tabs']['update'] = array(
		'#type' => 'fieldset',
		'#title' => t('Update Pari Theme'),
		'#description'   => t("<strong>Please refresh this page to reload theme update information</strong><br /><br /><hr />
		<strong>Current Pari Theme Version:</strong> 7.x-1.1<br><img src='http://www.drupar.com/images/theme-update/pari-update.jpg'><br /><hr /><br /><a href='https://www.drupal.org/project/pari' target='_blank'>Download latest version</a> || <a href='http://www.drupar.com/pari-theme-documentation/update-theme' target='_blank'>How to update theme</a>"),
		'#collapsible' => True,
		'#collapsed' => True,
	);
	$form['pari_settings']['tabs']['upgrade_pro'] = array(
		'#type' => 'fieldset',
		'#title' => t('Upgrade to Pro Version'),
		'#description'   => t("Purchase pro version for advance features like Responsive layout, Colors option, Shortcodes, Animated page elements etc. Click Below link to purchase and read more about pro version."),
		'#collapsible' => True,
		'#collapsed' => True,
	);
	$form['pari_settings']['tabs']['upgrade_pro']['upgrade'] = array(
		'#type' => 'fieldset',
		'#title' => 'Purchase pari Pro',
		'#description'   => t("<a href='http://www.drupar.com/theme/paripro' target='_blank'>Purpuse Pari Pro Version</a> || <a href='http://demo.drupar.com/paripro' target='_blank'>PariPro Live Demo</a>"),
	);
}
?>