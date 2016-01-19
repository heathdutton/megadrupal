<?php
/**
 * @file
 * Administrative page callbacks for the video_sitemap module.
 */

/**
 * Video sitemap settings form.
 */
function video_sitemap_settings_form() {
  $form['video_sitemap_minimum_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Video sitemap minimum lifetime'),
    '#options' => array(0 => t('No minimum')) + drupal_map_assoc(array(
        43200,
        86400,
        172800,
        259200,
        604800,
      ), 'format_interval'),
    '#description' => t('The minimum amount of time that will elapse before the sitemap is regenerated.'),
    '#default_value' => variable_get('video_sitemap_minimum_lifetime', 0),
  );
  $form['video_sitemap_disable_cron_regeneration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable cron generation of video sitemap.'),
    '#default_value' => variable_get('video_sitemap_disable_cron_regeneration', 0),
    '#description' => t('This can be disabled if video sitemap is generated only from UI.'),
  );
  $types = db_select('file_managed', 'fm')
    ->fields('fm', array('filemime'))
    ->condition('fm.type', 'video')
    ->distinct()
    ->execute()
    ->fetchAll();
  foreach ($types as $type) {
    $options[$type->filemime] = $type->filemime;
  }
  $form['video_sitemap_exclude_mime_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exclude MIME types from the sitemap.'),
    '#options' => $options,
    '#default_value' => array_filter(variable_get('video_sitemap_exclude_mime_types', array())),
  );
  return system_settings_form($form);
}

/**
 * Rebuild video sitemap links form.
 */
function video_sitemap_rebuild_form() {
  $form['rebuild_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild Video sitemap.'),
    '#default_value' => TRUE,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to rebuild the Video sitemap?'),
    'admin/config/search/video-sitemap/rebuild',
    '',
    t('Rebuild Video sitemap'),
    t('Cancel')
  );
}

/**
 * Submit callback for rebuild video sitemap links form.
 */
function video_sitemap_rebuild_form_submit($form, &$form_state) {
  if ($form_state['values']['rebuild_links']) {
    video_sitemap_rebuild();
  }
}
