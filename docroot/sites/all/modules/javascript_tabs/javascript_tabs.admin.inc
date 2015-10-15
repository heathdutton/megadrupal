<?php
/**
 * @file
 * The admin page include.
 *
 * Gives admins the option to choose which themes implement javascript_tabs.
 */


/**
 * Set up our admin form - plagiarised shamelessly from the block module.
 */
function javascript_tabs_admin_settings() {
  $themes = array();
  $theme_info = list_themes();

  foreach ($theme_info as $theme_name => $theme) {
    if ($theme->status > 0) {
      $themes[$theme_name] = $theme->info['name'];
    }
  }

  $form['javascript_tabs_theme_visibility'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#size' => min(count($themes), 8),
    '#title' => t('Select theme(s)'),
    '#options' => $themes,
    '#default_value' => variable_get('javascript_tabs_theme_visibility', array()),
    '#description' => t('Select which themes should have JavaScript Tabs added.'),
  );
  $form['page_visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
  );
  $form['page_visibility']['javascript_tabs_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Replace tabs on specific pages'),
    '#options' => array(t('Replace on every page except the listed pages.'), t('Replace on only the listed pages.')),
    '#default_value' => variable_get('javascript_tabs_visibility', 0),
  );
  $form['page_visibility']['javascript_tabs_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('javascript_tabs_pages', ''),
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
      array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>',
      )
    ),
  );

  return system_settings_form($form);
}
