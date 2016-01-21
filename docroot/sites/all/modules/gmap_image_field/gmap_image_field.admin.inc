<?php

/**
 * @file
 * GMap Image Field admin configurations.
 */


/**
 * Global configuration form for gmap image field.
 *
 * @param array $form
 *   Form element.
 * @param array &$form_state
 *   Current form states.
 *
 * @return array
 *   Return form element.
 */
function gmap_image_field_settings_form($form, &$form_state) {

  $executable = variable_get('gmap_image_field_convert_executable', GMAP_IMAGE_FIELD_IMAGICK_DEFAULT_EXEC);

  if (!_gmap_image_field_check_image_magick()) {
    $message = 'ImageMagick executables (%s) not found, the module can not do maps' .
              ' without ImageMagick, please install it. More info at %s';
    $message = sprintf(t($message), $executable, l(t('http://www.imagemagick.org/'), 'http://www.imagemagick.org/'));
    drupal_set_message($message, 'error');
  }

  $form['system'] = array(
    '#type' => 'fieldset',
    '#title' => t('System settings'),
  );

  $form['system']['gmap_image_field_convert_executable'] = array(
    '#title' => t('System path to convert'),
    '#description' => t('System path to <a href="http://www.imagemagick.org/">ImageMagick</a> convert application,
      usually on *nix servers it is available in /usr/bin/convert, in Windows
      servers it may be available on c:\ImageMagick\convert.exe or so'),
    '#type' => 'textfield',
    '#default_value' => $executable,
  );

  $form['system']['gmap_image_field_store'] = array(
    '#title' => t('Directory where tiles will be stored'),
    '#description' => t('This have to be public accessible directory'),
    '#type' => 'textfield',
    '#default_value' => variable_get('gmap_image_field_store', 'public://gmap-image-field'),
  );

  $form['system']['gmap_image_field_run_tiler_in_background'] = array(
    '#title' => t('Run tiler in background'),
    '#description' => t("Run ImageMagick's convert as <a href='http://en.wikipedia.org/wiki/Background_process'>background job</a>.
          Actually this may decrease saving node time because user not need to wait for tiler's ends.
          This option is supported only by *NIX operation systems."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('gmap_image_field_run_tiler_in_background', FALSE),
    '#disabled' => (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN'),
  );

  $form['system']['gmap_image_field_export_feed'] = array(
    '#title' => t("Don't embed maps javascritp to the page markup, load js separately."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('gmap_image_field_export_feed', FALSE),
  );

  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
  );

  $form['appearance']['gmap_image_field_disable_module_css'] = array(
    '#title' => t('Disable default css file'),
    '#description' => sprintf(t("This action will disable default <a href='%s'>module's css</a>,
         and will leave styling of the map and custom controls to your theme"), url(drupal_get_path('module', 'gmap_image_field') . '/gmap-image-field.css')),
    '#type' => 'checkbox',
    '#default_value' => variable_get('gmap_image_field_disable_module_css', FALSE),
  );

  $form['appearance']['gmap_image_field_popups'] = array(
    '#title' => t('Markers popup style'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('google', 'modal')),
    '#default_value' => variable_get('gmap_image_field_popups', 'google'),
  );

  return system_settings_form($form);
}
