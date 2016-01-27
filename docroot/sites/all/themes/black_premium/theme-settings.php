<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function black_premium_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['black_pre_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Black Premium Theme Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['black_pre_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs','black_premium'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
  $form['black_pre_settings']['slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front Page Slideshow'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['black_pre_settings']['slideshow']['slideshow_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show slideshow'),
    '#default_value' => theme_get_setting('slideshow_display','black_premium'),
    '#description'   => t("Check this option to show Slideshow in front page. Uncheck to hide."),
  );
  $form['black_pre_settings']['slideshow']['slideimage'] = array(
    '#markup' => t('To change the Slide Images, Replace the slide-image-1.jpg, slide-image-2.jpg and slide-image-3.jpg in the images folder of the Business theme folder.'),
  );
}