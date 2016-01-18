<?php
/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function classic_blog_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['classic_blog_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Classic Blog Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['classic_blog_settings']['image_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show <strong>Image Logo</strong> instead of text logo in a page'),
    '#default_value' => theme_get_setting('image_logo','classic_blog'),
    '#description'   => t("Check this option to show Image Logo in page. Uncheck to show the text logo."),
  );
  $form['classic_blog_settings']['breadcrumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show breadcrumbs in a page'),
    '#default_value' => theme_get_setting('breadcrumbs','classic_blog'),
    '#description'   => t("Check this option to show breadcrumbs in page. Uncheck to hide."),
  );
}
