<?php
/**
 * @file
 * Theme settings for Breadcrump and Social Block. 
 */

/**
 * Implements of hook_form_system_theme_settings_alter().
 */
function elimai_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['elimai_settings']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumb'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elimai_settings']['breadcrumb']['breadcrumb_delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb delimiter'),
    '#size' => 4,
    '#default_value' => theme_get_setting('breadcrumb_delimiter'),
    '#description' => t("Don't forget spaces at either end... if you're into that sort of thing."),
  );
  $form['elimai_settings']['socialblock'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social Icon'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['elimai_settings']['socialblock']['social_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Social Icons'),
    '#default_value' => theme_get_setting('social_block', 'elimai'),
    '#description'   => t("Check this option to show Social Icons."),
  );
  $form['elimai_settings']['socialblock']['twitter_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Profile URL'),
    '#default_value' => theme_get_setting('twitter_url', 'elimai'),
    '#description'   => t("Enter your Twitter Profile URL. Leave blank to hide."),
  );
  $form['elimai_settings']['socialblock']['facebook_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Profile URL'),
    '#default_value' => theme_get_setting('facebook_url', 'elimai'),
    '#description'   => t("Enter your Facebook Profile URL. Leave blank to hide."),
  );
  $form['elimai_settings']['socialblock']['gplus_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Plus Address'),
    '#default_value' => theme_get_setting('gplus_url', 'elimai'),
    '#description'   => t("Enter your Google Plus URL. Leave blank to hide."),
  );
  $form['elimai_settings']['socialblock']['pinterest_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Pinterest Address'),
    '#default_value' => theme_get_setting('pinterest_url', 'elimai'),
    '#description'   => t("Enter your Pinterest URL. Leave blank to hide."),
  );
  $libraries_options = array(
    'bootstrapcdn' => t('Bootstrap CDN'),
    'theme' => t('[current_theme]/libraries/bootstrap'),
  );
  if (module_exists('libraries')) {
    $bootstrap_path = libraries_get_path('bootstrap');
    if (!empty($bootstrap_path)) {
      $libraries_options['libraries'] = libraries_get_path('bootstrap');
    }
  }
  $form['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Libraries'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'bootstrap_source' => array(
      '#type' => 'radios',
      '#title' => t('Load Twitter Bootstrap library from:'),
      '#options' => $libraries_options,
      '#default_value' => theme_get_setting('bootstrap_source'),
    ),
    'bootstrap_version' => array(
      '#type' => 'textfield',
      '#title' => t('Twitter Bootstrap version:'),
      '#default_value' => theme_get_setting('bootstrap_version'),
    ),
  );

  $form['elimai_settings']['carousel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bootstrap Carousel'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  // Bootstrap Carousel require jquery_update version 1.7 or higher.
  if (module_exists('jquery_update')) {
    $carousel_enabled = theme_get_setting('carousel_enabled');
    // By default carousel should be enabled.
    $carousel_enabled = $carousel_enabled === NULL ? 1 : $carousel_enabled;

    $form['elimai_settings']['carousel']['carousel_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable the Bootstrap Carousel'),
      '#default_value' => $carousel_enabled,
    );

    $form['elimai_settings']['carousel']['arrow_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display next/previous arrows'),
      '#default_value' => theme_get_setting('arrow_enabled'),
    );

    $form['elimai_settings']['carousel']['bullets_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display navigation bullets'),
      '#default_value' => theme_get_setting('bullets_enabled'),
    );

    $slide_count = theme_get_setting('slide_count');
    $form['elimai_settings']['carousel']['slide_count'] = array(
      '#type' => 'select',
      '#title' => t('Number of slides'),
      '#options' => range(1, 20),
      '#default_value' => $slide_count,
      '#description' => t('Note: You need to save this page to refresh(add/remove) the slides'),
    );

    // Range indexes start's with 0, hence the slide_count+1 in the for
    // condition.
    for ($index = 1; $index <= $slide_count + 1; $index++) {
      $form['elimai_settings']['carousel']['slide_' . $index] = array(
        '#type' => 'fieldset',
        '#title' => t('Slide') . ' ' . $index,
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      // Use the #managed_file FAPI element to upload an image file.
      $form['elimai_settings']['carousel']['slide_' . $index]['carousel_image_' . $index] = array(
        '#title' => t('Image File') . ' ' . $index,
        '#type' => 'managed_file',
        '#default_value' => theme_get_setting('carousel_image_' . $index),
        '#upload_location' => 'public://elimai_slides/',
      );
      $form['elimai_settings']['carousel']['slide_' . $index]['carousel_alt_text_' . $index] = array(
        '#title' => t('Image Title') . ' ' . $index,
        '#type' => 'textfield',
        '#default_value' => theme_get_setting('carousel_alt_text_' . $index),
        '#description' => t('This field will be used for slide title as well as image ALT attribute'),
      );
      $form['elimai_settings']['carousel']['slide_' . $index]['slide_caption_' . $index] = array(
        '#title' => t('Slide Caption') . ' ' . $index,
        '#type' => 'textarea',
        '#default_value' => theme_get_setting('slide_caption_' . $index),
      );
      $theme_settings_path = drupal_get_path('theme', 'elimai') . '/theme-settings.php';
      if (file_exists(DRUPAL_ROOT . '/' . $theme_settings_path)) {
        require_once DRUPAL_ROOT . '/' . $theme_settings_path;
        $form_state['build_info']['files'][] = $theme_settings_path;
      }
      $form['#submit'][] = 'elimai_form_system_theme_settings_submit';
    }
  }
  else {
    $form['elimai_settings']['carousel']['#description'] = t('Bootstrap carousel requires minimum jQuery version of 1.7 or higher. You must manually set this in the configuration by downloading and enabling the <a href="https://drupal.org/project/jquery_update" target="_blank">jQuery Update</a> -7.x-2.3 or higher.');
  }
  return $form;
}

/**
 * Submit handler to save the carousel files.
 */
function elimai_form_system_theme_settings_submit(&$form, &$form_state) {
  global $user;
  if ($form_state['values']['carousel_enabled']) {
    $slide_count = check_plain(theme_get_setting('slide_count', 'elimai'));

    for ($index = 1; $index <= $slide_count + 1; $index++) {
      $carousel_image = $form_state['values']['carousel_image_' . $index];
      if (!empty($carousel_image)) {
        $file = file_load($carousel_image);
        if (isset($file->fid)) {
          $file->status = FILE_STATUS_PERMANENT;
          file_save($file);
          // Record that the module (in this example, user module) is
          // using the file.
          file_usage_add($file, 'user', 'user', $user->uid);
        }
      }
    }
  }
}
