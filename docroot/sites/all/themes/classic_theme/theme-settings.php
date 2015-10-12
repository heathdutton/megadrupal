<?php

function classic_theme_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['classic_theme_color_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Color Scheme'),
    '#weight' => -2,
    '#description'   => t("Select a predesigned color scheme for the site."),
  );

  $form['classic_theme_color_settings']['color_scheme'] = array(
    '#type'          => 'select',
    '#title'         => t('Color Scheme'),
    '#default_value' => theme_get_setting('color_scheme', 'classic_theme'),
    '#description'   => t('Select a predesigned color scheme.'),
    '#options'       => array(
      'white' => t('White'),
      'dark' => t('Dark'),
     ),
  );

  $form['classic_theme_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('classic_theme Theme Settings'),
    '#weight' => -1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['classic_theme_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs', 'classic_theme'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['classic_theme_settings']['backgroundimg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Body Background Image'),
    '#default_value' => theme_get_setting('backgroundimg', 'classic_theme'),
    '#description'   => t("Check this option to show Body Background Image. Uncheck to hide."),
  );
  $form['classic_theme_settings']['top_social_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social links in header'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['classic_theme_settings']['top_social_link']['social_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show social icons (Facebook, Twitter and RSS) in header'),
    '#default_value' => theme_get_setting('social_links', 'classic_theme'),
    '#description'   => t("Check this option to show twitter, facebook, rss icons in header. Uncheck to hide."),
  );
  $form['classic_theme_settings']['top_social_link']['twitter_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Username'),
    '#default_value' => theme_get_setting('twitter_username', 'classic_theme'),
	'#description'   => t("Enter your Twitter username."),
  );
  $form['classic_theme_settings']['top_social_link']['facebook_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Username'),
    '#default_value' => theme_get_setting('facebook_username', 'classic_theme'),
	'#description'   => t("Enter your Facebook username."),
  );
  $form['classic_theme_settings']['slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front Page Slideshow'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['classic_theme_settings']['slideshow']['slideshow_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show slideshow'),
    '#default_value' => theme_get_setting('slideshow_display','classic_theme'),
    '#description'   => t("Check this option to show Slideshow in front page. Uncheck to hide."),
  );
  $form['classic_theme_settings']['slideshow']['slideimage'] = array(
    '#markup' => t('To change the Slide Images, Replace the slide-image-1.jpg, slide-image-2.jpg and slide-image-3.jpg in the images folder of the classic_theme theme folder.'),
  );
  $form['classic_theme_settings']['footer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['classic_theme_settings']['footer']['footer_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('footer_copyright','classic_theme'),
    '#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['classic_theme_settings']['footer']['footer_credits'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show theme credits in footer'),
    '#default_value' => theme_get_setting('footer_credits','classic_theme'),
    '#description'   => t("Check this option to show site credits in footer. Uncheck to hide."),
  );
}

?>