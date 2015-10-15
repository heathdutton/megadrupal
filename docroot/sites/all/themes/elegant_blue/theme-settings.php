<?php
/**
 * @file
 * Theme setting callbacks for the elegant_blue theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function elegant_blue_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['elegant_blue_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('elegant Blue Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['elegant_blue_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs', 'elegant_blue'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['elegant_blue_settings']['slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front Page Slideshow'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elegant_blue_settings']['slideshow']['slideshow_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show slideshow'),
    '#default_value' => theme_get_setting('slideshow_display', 'elegant_blue'),
    '#description'   => t("Check this option to show Slideshow in front page. Uncheck to hide."),
  );
  $form['elegant_blue_settings']['slideshow']['slide'] = array(
    '#markup' => t('You can change the description of each slide in the following Slide Settings.'),
  );
  $form['elegant_blue_settings']['slideshow']['slide1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 1'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['elegant_blue_settings']['slideshow']['slide1']['slide1_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide1_desc', 'elegant_blue'),
  );
  $form['elegant_blue_settings']['slideshow']['slide2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 2'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['elegant_blue_settings']['slideshow']['slide2']['slide2_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide2_desc', 'elegant_blue'),
  );
  $form['elegant_blue_settings']['slideshow']['slide3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 3'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['elegant_blue_settings']['slideshow']['slide3']['slide3_desc'] = array(
    '#type' => 'textfield',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide3_desc', 'elegant_blue'),
  );
  $form['elegant_blue_settings']['slideshow']['slideimage'] = array(
    '#markup' => t('To change the Slide Images, Replace the slide-image-1.jpg, slide-image-2.jpg and slide-image-3.jpg in the images folder of the elegant_blue theme folder.'),
  );
  $form['elegant_blue_settings']['social'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elegant_blue_settings']['social']['display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icon'),
    '#default_value' => theme_get_setting('display', 'elegant_blue'),
    '#description'   => t("Check this option to show Social Icon. Uncheck to hide."),
  );
  $form['elegant_blue_settings']['social']['twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter URL'),
    '#default_value' => theme_get_setting('twitter', 'elegant_blue'),
    '#description' => t("Enter your Twitter Profile URL. example:: http://www.xyz.com"),
  );
  $form['elegant_blue_settings']['social']['facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook URL'),
    '#default_value' => theme_get_setting('facebook', 'elegant_blue'),
    '#description'   => t("Enter your Facebook Profile URL. example:: http://www.xyz.com"),
  );
  $form['elegant_blue_settings']['social']['linkedin'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkedIn URL'),
    '#default_value' => theme_get_setting('linkedin', 'elegant_blue'),
    '#description'   => t("Enter your LinkedIn Profile URL. example:: http://www.xyz.com"),
  );
  $form['elegant_blue_settings']['Welcome'] = array(
    '#type' => 'fieldset',
    '#title' => t('Welcome Text'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elegant_blue_settings']['Welcome']['welcome_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Add Title'),
    '#default_value' => theme_get_setting('welcome_title', 'elegant_blue'),
    '#description'   => t("Enter Title for Welcome text at frontpage."),
  );
  $form['elegant_blue_settings']['Welcome']['welcome_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Add Description'),
    '#default_value' => theme_get_setting('welcome_text', 'elegant_blue'),
    '#description'   => t("Enter Description for Welcome text at frontpage."),
  );
  $form['elegant_blue_settings']['Columns'] = array(
    '#type' => 'fieldset',
    '#title' => t('Columns'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elegant_blue_settings']['Columns']['colonetitle'] = array(
    '#type' => 'textfield',
    '#title' => t('First Column Title'),
    '#default_value' => theme_get_setting('colonetitle', 'elegant_blue'),
    '#description'   => t("Enter Title for First Column."),
  );
  $form['elegant_blue_settings']['Columns']['colone'] = array(
    '#type' => 'textarea',
    '#title' => t('First Column Description'),
    '#default_value' => theme_get_setting('colone', 'elegant_blue'),
    '#description'   => t("Enter Description for First Column."),
  );
  $form['elegant_blue_settings']['Columns']['coltwotitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Second Column Title'),
    '#default_value' => theme_get_setting('coltwotitle', 'elegant_blue'),
    '#description'   => t("Enter Title for Second Column."),
  );
  $form['elegant_blue_settings']['Columns']['coltwo'] = array(
    '#type' => 'textarea',
    '#title' => t('Second Column Description'),
    '#default_value' => theme_get_setting('coltwo', 'elegant_blue'),
    '#description'   => t("Enter Description for Second Column."),
  );
  $form['elegant_blue_settings']['Columns']['colthreetitle'] = array(
    '#type' => 'textfield',
    '#title' => t('Third Column Title'),
    '#default_value' => theme_get_setting('colthreetitle', 'elegant_blue'),
    '#description'   => t("Enter Title for Third Column."),
  );
  $form['elegant_blue_settings']['Columns']['colthree'] = array(
    '#type' => 'textarea',
    '#title' => t('Third Column Description'),
    '#default_value' => theme_get_setting('colthree', 'elegant_blue'),
    '#description'   => t("Enter Description for Third Column."),
  );
  $form['elegant_blue_settings']['footer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elegant_blue_settings']['footer']['footer_copyright'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show copyright text in footer'),
    '#default_value' => theme_get_setting('footer_copyright', 'elegant_blue'),
    '#description'   => t("Check this option to show copyright text in footer. Uncheck to hide."),
  );
  $form['elegant_blue_settings']['footer']['footer_developed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show theme developed by in footer'),
    '#default_value' => theme_get_setting('footer_developed', 'elegant_blue'),
    '#description'   => t("Check this option to show design & developed by text in footer. Uncheck to hide."),
  );
  $form['elegant_blue_settings']['footer']['footer_developedby'] = array(
    '#type' => 'textfield',
    '#title' => t('Add name developed by in footer'),
    '#default_value' => theme_get_setting('footer_developedby', 'elegant_blue'),
    '#description'   => t("Add name developed by in footer"),
  );
  $form['elegant_blue_settings']['footer']['footer_developedby_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Add link to developed by in footer'),
    '#default_value' => theme_get_setting('footer_developedby_url', 'elegant_blue'),
    '#description'   => t("Add url developed by in footer. example:: http://www.xyz.com"),
  );
}
