<?php
/**
 * @file
 * Admin form
 */

/**
 * Menu callback Build the admin form.
 */
function optify_admin($form = array(), $form_state = array()) {
  // Users enter the Optify embed javascript code.
  $form['optify_embed_code'] = array(
    '#title' => t('Optify Token'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('optify_embed_code'),
    '#description' => t('To find your Optify token, <a href="@url" target="_blank">log-in to your Optify account</a> and click the Settings button (gears icon) in upper right of the page. Click the Sites tab. Copy the "Site token" (8 character string) and paste or type in the field above.',
      array('@url' => url('https://dashboard.optify.net/login'))),
  );

  // Tell Drupal which pages to not track with Optify.
  $form['optify_no_display'] = array(
    '#title' => t('Pages Not to Track'),
    '#type' => 'textarea',
    '#rows' => 10,
    '#default_value' => variable_get('optify_no_display', "admin\nadmin/*\nbatch\nnode/add*\nnode/*/*\nuser/*/*"),
    '#description' => t("Specify pages that Optify does not need to track by entering their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
      array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  return system_settings_form($form);
}