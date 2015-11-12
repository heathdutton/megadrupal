<?php
/**
 * @file
 * Implements theme settings for the royal_olive theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function royal_olive_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['royal_olive_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Royal Olive Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['royal_olive_settings']['show_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Breadcrumb'),
    '#default_value' => theme_get_setting('show_breadcrumb', 'royal_olive'),
    '#description'   => t("Check this option to show breadcrumb in a page. Uncheck to hide."),
  );
  $form['royal_olive_settings']['show_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Search Block'),
    '#default_value' => theme_get_setting('show_search', 'royal_olive'),
    '#description'   => t("Check this option to show Search Block by default. Uncheck to hide."),
  );
  $form['royal_olive_settings']['slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Home Page Slideshow'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['royal_olive_settings']['slideshow']['show_slideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Slideshow'),
    '#default_value' => theme_get_setting('show_slideshow', 'royal_olive'),
    '#description'   => t("Check this option to enable Slideshow in home page. Uncheck to hide."),
  );
  $form['royal_olive_settings']['slideshow']['slide'] = array(
    '#markup' => t('Update the description and URL for slides respectively:'),
  );
  $form['royal_olive_settings']['slideshow']['slide1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 1'),
  );
  $form['royal_olive_settings']['slideshow']['slide1']['slide_1_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide_1_desc', 'royal_olive'),
  );
  $form['royal_olive_settings']['slideshow']['slide2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 2'),
  );
  $form['royal_olive_settings']['slideshow']['slide2']['slide_2_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide_2_desc', 'royal_olive'),
  );
  $form['royal_olive_settings']['slideshow']['slide3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 3'),
  );
  $form['royal_olive_settings']['slideshow']['slide3']['slide_3_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide_3_desc', 'royal_olive'),
  );
  $form['royal_olive_settings']['slideshow']['slideimage'] = array(
    '#markup' => t('Note: To replace the current Slide Images, update slider_image_1.jpg, slider_image_2.jpg and slider_image_3.jpg accordingly in the images folder.'),
  );
  $form['royal_olive_settings']['social_icons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['royal_olive_settings']['social_icons']['show_social_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icon'),
    '#default_value' => theme_get_setting('show_social_icons', 'royal_olive'),
    '#description'   => t("Check this option to show Social Icon. Uncheck to hide."),
  );
  $form['royal_olive_settings']['social_icons']['twitter_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Address:'),
    '#default_value' => theme_get_setting('twitter_link', 'royal_olive'),
    '#description'   => t("Enter your Twitter URL. Leave blank to hide."),
  );
  $form['royal_olive_settings']['social_icons']['facebook_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Address:'),
    '#default_value' => theme_get_setting('facebook_link', 'royal_olive'),
    '#description'   => t("Enter your Facebook URL. Leave blank to hide."),
  );
  $form['royal_olive_settings']['social_icons']['googleplus_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus Address:'),
    '#default_value' => theme_get_setting('googleplus_link', 'royal_olive'),
    '#description'   => t("Enter your Google Plus URL. Leave blank to hide."),
  );
  $form['royal_olive_settings']['social_icons']['linkedin_link'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn Address:'),
    '#default_value' => theme_get_setting('linkedin_link', 'royal_olive'),
    '#description'   => t("Enter your LinkedIn URL. Leave blank to hide."),
  );
  $form['royal_olive_settings']['social_icons']['pinterest_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Pinterest Address:'),
    '#default_value' => theme_get_setting('pinterest_link', 'royal_olive'),
    '#description'   => t("Enter your Pinterest URL. Leave blank to hide."),
  );
}
