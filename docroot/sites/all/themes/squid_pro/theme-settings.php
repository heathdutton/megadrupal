<?php
/**
 * @file
 * Implements theme settings for the Squid Pro theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function squid_pro_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['squid_pro_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Squid Pro Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['squid_pro_settings']['show_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumb'),
    '#default_value' => theme_get_setting('show_breadcrumb', 'squid_pro'),
    '#description'   => t("Check this option to show breadcrumb in a page. Uncheck to hide."),
  );
  $form['squid_pro_settings']['show_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Search Block'),
    '#default_value' => theme_get_setting('show_search', 'squid_pro'),
    '#description'   => t("Check this option to show Search Block by default. Uncheck to hide."),
  );
  $form['squid_pro_settings']['show_sitename'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Site Name'),
    '#default_value' => theme_get_setting('show_sitename', 'squid_pro'),
    '#description'   => t("Squid Pro theme hides site name by default. Check here to show site name."),
  );
  $form['squid_pro_settings']['slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Home Page Slideshow'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['squid_pro_settings']['slideshow']['show_slideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Slideshow'),
    '#default_value' => theme_get_setting('show_slideshow', 'squid_pro'),
    '#description'   => t("Check this option to enable Slideshow in home page. Uncheck to hide."),
  );
  $form['squid_pro_settings']['slideshow']['slide'] = array(
    '#markup' => t('Update the description and URL for slides respectively:'),
  );
  $form['squid_pro_settings']['slideshow']['slide1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 1'),
  );
  $form['squid_pro_settings']['slideshow']['slide1']['slide_1_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide_1_desc', 'squid_pro'),
  );
  $form['squid_pro_settings']['slideshow']['slide2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 2'),
  );
  $form['squid_pro_settings']['slideshow']['slide2']['slide_2_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide_2_desc', 'squid_pro'),
  );
  $form['squid_pro_settings']['slideshow']['slide3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 3'),
  );
  $form['squid_pro_settings']['slideshow']['slide3']['slide_3_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide_3_desc', 'squid_pro'),
  );
  $form['squid_pro_settings']['slideshow']['slideimage'] = array(
    '#markup' => t('Note: To replace the current Slide Images, update slider_image_1.jpg, slider_image_2.jpg and slider_image_3.jpg accordingly in the images folder.'),
  );
  $form['squid_pro_settings']['social_icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['squid_pro_settings']['social_icons']['show_social_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icon'),
    '#default_value' => theme_get_setting('show_social_icons', 'squid_pro'),
    '#description'   => t("Check this option to show Social Icon. Uncheck to hide."),
  );
  $form['squid_pro_settings']['social_icons']['twitter_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Address:'),
    '#default_value' => theme_get_setting('twitter_link', 'squid_pro'),
    '#description'   => t("Enter your Twitter URL. Leave blank to hide."),
  );
  $form['squid_pro_settings']['social_icons']['facebook_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Address:'),
    '#default_value' => theme_get_setting('facebook_link', 'squid_pro'),
    '#description'   => t("Enter your Facebook URL. Leave blank to hide."),
  );
  $form['squid_pro_settings']['social_icons']['googleplus_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus Address:'),
    '#default_value' => theme_get_setting('googleplus_link', 'squid_pro'),
    '#description'   => t("Enter your Google Plus URL. Leave blank to hide."),
  );
  $form['squid_pro_settings']['social_icons']['linkedin_link'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn Address:'),
    '#default_value' => theme_get_setting('linkedin_link', 'squid_pro'),
    '#description'   => t("Enter your LinkedIn URL. Leave blank to hide."),
  );
  $form['squid_pro_settings']['social_icons']['youtube_link'] = array(
    '#type' => 'textfield',
    '#title' => t('YouTube Address:'),
    '#default_value' => theme_get_setting('youtube_link', 'squid_pro'),
    '#description'   => t("Enter your youtube URL. Leave blank to hide."),
  );
    $form['squid_pro_settings']['contact_map'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact Map'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
    $form['squid_pro_settings']['contact_map']['show_map'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Map on Contact Page'),
    '#default_value' => theme_get_setting('show_map', 'squid_pro'),
    '#description'   => t("Check this option to show map on the contact page. Uncheck to hide."),
  );
    $form['squid_pro_settings']['contact_map']['iframe_link'] = array(
    '#type' => 'textarea',
    '#title' => t('Location Map (iframe value):'),
    '#default_value' => theme_get_setting('iframe_link', 'squid_pro'),
    '#description'   => t("Enter the Google Map address of your location. Leave blank to hide."),
  );
    $form['squid_pro_settings']['office_one'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact - Office One'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
    $form['squid_pro_settings']['office_one']['show_office_one'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show First Office Address on Contact Page'),
    '#default_value' => theme_get_setting('show_office_one', 'squid_pro'),
    '#description'   => t("Check this option to show First Office Address on the contact page. Uncheck to hide."),
  );
    $form['squid_pro_settings']['office_one']['address'] = array(
    '#type' => 'textarea',
    '#title' => t('First Office Address'),
    '#default_value' => theme_get_setting('address', 'squid_pro'),
    '#description'   => t("Enter your First Office Address."),
  );
    $form['squid_pro_settings']['office_two'] = array(
    '#type' => 'fieldset',
    '#title' => t('Contact - Office Two'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
    $form['squid_pro_settings']['office_two']['show_office_two'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Second Office Address on Contact Page'),
    '#default_value' => theme_get_setting('show_office_two', 'squid_pro'),
    '#description'   => t("Check this option to show Second Office Address on the contact page. Uncheck to hide."),
  );
    $form['squid_pro_settings']['office_two']['address2'] = array(
    '#type' => 'textarea',
    '#title' => t('Second Office Address'),
    '#default_value' => theme_get_setting('address2', 'squid_pro'),
    '#description'   => t("Enter your Second Office Address."),
  );
}
