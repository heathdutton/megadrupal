<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function responsive_business_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['res_busi'] = array(
    '#type' => 'fieldset',
    '#title' => t('Responsive Business Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['res_busi']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs','responsive_business'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['res_busi']['slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front Page Slideshow'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['res_busi']['slideshow']['slideshow_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show slideshow'),
    '#default_value' => theme_get_setting('slideshow_display','responsive_business'),
    '#description'   => t("Check this option to show Slideshow in front page. Uncheck to hide."),
  );
  $form['res_busi']['slideshow']['slide'] = array(
    '#markup' => t('You can change the description and URL of each slide in the following Slide Setting fieldsets.'),
  );
  $form['res_busi']['slideshow']['slide1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 1'),
  );
  $form['res_busi']['slideshow']['slide1']['slide1_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide1_desc','responsive_business'),
  );
  $form['res_busi']['slideshow']['slide2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 2'),
  );
  $form['res_busi']['slideshow']['slide2']['slide2_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide2_desc','responsive_business'),
  );
  $form['res_busi']['slideshow']['slide3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slide 3'),
  );
  $form['res_busi']['slideshow']['slide3']['slide3_desc'] = array(
    '#type' => 'textarea',
    '#title' => t('Slide Description'),
    '#default_value' => theme_get_setting('slide3_desc','responsive_business'),
  );
  $form['res_busi']['slideshow']['slideimage'] = array(
    '#markup' => t('To change the Slide Images, Replace the slide-image-1.jpg, slide-image-2.jpg and slide-image-3.jpg in the images folder of the theme folder.'),
  );
}
