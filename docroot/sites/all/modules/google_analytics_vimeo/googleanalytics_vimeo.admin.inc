<?php
/**
 * @file
 * Google Analytics Vimeo admin configurations.
 */

/**
 * Google Analytics Vimeo settings.
 */
function googleanalytics_vimeo_settings() {
  $form['googleanalytics_vimeo_tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select Tracking'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['googleanalytics_vimeo_tracking']['googleanalytics_vimeo_tracking_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable Tracking'),
    '#options' => array(
      'googleanalytics_vimeo_progress25' => t('25% Progress'),
      'googleanalytics_vimeo_progress50' => t('50% Progress'),
      'googleanalytics_vimeo_progress75' => t('75% Progress'),
      'googleanalytics_vimeo_videoPlayed' => t('Video Played'),
      'googleanalytics_vimeo_videoPaused' => t('Video Paused'),
      'googleanalytics_vimeo_videoResumed' => t('Video Resumed'),
      'googleanalytics_vimeo_videoSeeking' => t('Video Seeking'),
      'googleanalytics_vimeo_videoCompleted' => t('Video Completed'),
    ),
  );
  $form['googleanalytics_vimeo_tracking']['googleanalytics_vimeo_tracking_options']['#default_value'] = array_keys(array_filter(variable_get('googleanalytics_vimeo_tracking_options', $form['googleanalytics_vimeo_tracking']['googleanalytics_vimeo_tracking_options']['#options'])));
  // Page specific visibility configurations.
  $php_access = user_access('use PHP for vimeo tracking visibility');
  $visibility = variable_get('googleanalytics_vimeo_visibility_options', 0);
  $pages = variable_get('googleanalytics_vimeo_visibility_pages', GOOGLEANALYTICS_PAGES);
  $options = array();
  $title = '';
  $description = '';
  if ($visibility == GOOGLEANALYTICS_VIMEO_USE_PHP_FOR_TRACKING && !$php_access) {
    $form['googleanalytics_vimeo_visibility_options'] = array('#type' => 'value', '#value' => 1);
    $form['googleanalytics_vimeo_visibility_pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    ));
    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
  }
  $form['googleanalytics_vimeo_visibility_options'] = array(
    '#type' => 'radios',
    '#title' => t('Add Google Analytics Vimeo Video Tracking to specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('googleanalytics_vimeo_visibility_options', 0),
  );
  $form['googleanalytics_vimeo_visibility_pages'] = array(
    '#type' => 'textarea',
    '#title' => $title,
    '#default_value' => $pages,
    '#description' => $description,
    '#wysiwyg' => FALSE,
    '#rows' => 10,
  );
  return system_settings_form($form);
}
