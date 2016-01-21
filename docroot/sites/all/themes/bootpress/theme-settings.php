<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 *    @author Pitabas Behera
 */
function boot_press_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['boot_press_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Boot Press Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['boot_press_settings']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['boot_press_settings']['tabs']['site_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Info'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['boot_press_settings']['tabs']['site_info']['header_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Header Message'),
    '#default_value' => theme_get_setting('header_message'),
    '#description'   => t('Enter your site header message'),
  );
  $form['boot_press_settings']['tabs']['site_info']['copyright_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Copyright Info'),
    '#default_value' => theme_get_setting('copyright_info'),
    '#description'   => t('Enter your site copyright info'),
  );
  $form['boot_press_settings']['tabs']['basic_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['boot_press_settings']['tabs']['basic_settings']['breadcrumb_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumb'),
    '#description'   => t('Use the checkbox to enable or disable the breadcrumb.'),
    '#default_value' => theme_get_setting('breadcrumb_display', 'boot_press'),
  );
  $form['boot_press_settings']['tabs']['basic_settings']['sticky_main_navigation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky Main Navigation'),
    '#description'   => t('Use the checkbox to enable or disable the sticky main navigation.'),
    '#default_value' => theme_get_setting('sticky_main_navigation', 'boot_press'),
  );
  $form['boot_press_settings']['tabs']['basic_settings']['enable_jquery_appear'] = array(
    '#type' => 'checkbox',
    '#title' => t('jQuery Appear'),
    '#description'   => t('Use the checkbox to enable or disable the jQuery plugin for tracking elements appearance in browser viewport.'),
    '#default_value' => theme_get_setting('enable_jquery_appear', 'boot_press'),
  );
  $form['boot_press_settings']['tabs']['basic_settings']['scrolltop_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show scroll-to-top button'),
    '#description'   => t('Use the checkbox to enable or disable scroll-to-top button.'),
    '#default_value' => theme_get_setting('scrolltop_display', 'boot_press'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['boot_press_settings']['tabs']['ie8_support'] = array(
    '#type' => 'fieldset',
    '#title' => t('IE8 support'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['boot_press_settings']['tabs']['ie8_support']['responsive_respond'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Respond.js [<em>boot-press/js/respond.min.js</em>] JavaScript to add basic CSS3 media query support to IE 6-8.'),
    '#default_value' => theme_get_setting('responsive_respond', 'boot_press'),
    '#description'   => t('IE 6-8 require a JavaScript polyfill solution to add basic support of CSS3 media queries. Note that you should enable <strong>Aggregate and compress CSS files</strong> through <em>/admin/config/development/performance</em>.'),
  );
  $form['boot_press_settings']['tabs']['social_media'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Media'),
    '#description' => t('<strong>Enter your username / URL to show or leave blank to hide Social Media Icons</strong><hr>'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['boot_press_settings']['tabs']['social_media']['twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('twitter'),
    '#description'   => t('Enter URL to your Twitter Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('facebook'),
    '#description'   => t('Enter URL to your Facebook Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['linkedin'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('linkedin'),
    '#description'   => t('Enter URL to your LinkedIn Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['dribbble'] = array(
    '#type' => 'textfield',
    '#title' => t('Dribbble URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('dribbble'),
    '#description'   => t('Enter URL to your Dribbble Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['flickr'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('flickr'),
    '#description'   => t('Enter URL to your Flickr Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['skype'] = array(
    '#type' => 'textfield',
    '#title' => t('Skype URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('skype'),
    '#description'   => t('Enter URL to your Skype Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['digg'] = array(
    '#type' => 'textfield',
    '#title' => t('Digg URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('digg'),
    '#description'   => t('Enter URL to your Digg Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['googleplus'] = array(
    '#type' => 'textfield',
    '#title' => t('Google+ URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('googleplus'),
    '#description'   => t('Enter URL to your Google+ Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['instagram'] = array(
    '#type' => 'textfield',
    '#title' => t('Instagram URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('instagram'),
    '#description'   => t('Enter URL to your Instagram Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['vimeo'] = array(
    '#type' => 'textfield',
    '#title' => t('Vimeo URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('vimeo'),
    '#description'   => t('Enter URL to your Vimeo Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['tumblr'] = array(
    '#type' => 'textfield',
    '#title' => t('Tumblr URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('tumblr'),
    '#description'   => t('Enter URL to your Tumblr Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['youtube'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('youtube'),
    '#description'   => t('Enter URL to your YouTube Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['pinterest'] = array(
    '#type' => 'textfield',
    '#title' => t('pinterest URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('pinterest'),
    '#description'   => t('Enter URL to your Pinterest Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['paypal'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('paypal'),
    '#description'   => t('Enter URL to your PayPal Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['xing'] = array(
    '#type' => 'textfield',
    '#title' => t('xing URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('xing'),
    '#description'   => t('Enter URL to your XING Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['delicious'] = array(
    '#type' => 'textfield',
    '#title' => t('Delicious URL'),
                '#size' => 10,
    '#default_value' => theme_get_setting('delicious'),
    '#description'   => t('Enter URL to your Delicious Account'),
  );
  $form['boot_press_settings']['tabs']['social_media']['rss']  = array(
    '#type' => 'checkbox',
    '#title' => t('Show RSS'),
    '#description'   => t('Check to Show RSS Icon'),
    '#default_value' => theme_get_setting('rss', 'boot_press'),
  );
}