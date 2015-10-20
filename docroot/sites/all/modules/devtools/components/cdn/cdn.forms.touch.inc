<?php

/**
 * Form callback for /admin/config/development/cdn/touch;
 * Provides options to touch (invalidate) individual site components or paths.
 * @return array Form API array.
 */
function devtools_cdn_admin_touch_form($form, &$form_state) {
  $form = array();

  $form['operation'] = array(
    '#title' => t('Operation'),
    '#type' => 'radios',
    '#options' => array(
      'touch' => t('Touch'),
    ),
    '#default_value' => 'touch',
    '#description' => t('Selected operation will be performed on all static files within the selected components.'),
  );

  // do we need a cloudfront operation?
  $cdn = \devtools\components\CDN::instance();
  if ($cf = $cdn->get('cloudfront')) {
    $form['operation']['#options']['cloudfront'] = t('Invalidate CloudFront (!link)', array('!link' => l(t('Current invalidation queue'), 'admin/config/development/cdn/cloudfront/queue')));
  }

  $form['path'] = array(
    '#title' => t('File or folder path'),
    '#type' => 'textfield',
    '#description' => t('Path should be relative to the document root.'),
  );

  $form['components'] = array(
    '#title' => t('Components'),
    '#type' => 'checkboxes',
    '#options' => array(
      'themes' => t('Themes'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Validation handler for the devtools_cdn_admin_touch_form form.
 * @param  array $form       The FAPI form.
 * @param  array $form_state Current form state.
 */
function devtools_cdn_admin_touch_form_validate($form, &$form_state) {
  $values =& $form_state['values'];

  if (!empty($values['path']) && !file_exists($values['path'])) {
    form_set_error('path', t('The file/path %path does not exist.', array('%path' => $values['path'])));
  }
}

/**
 * Submit handler for the devtools_cdn_admin_touch_form form.
 * @param  array $form       The FAPI form.
 * @param  array $form_state Current form state.
 */
function devtools_cdn_admin_touch_form_submit($form, &$form_state) {
  $values =& $form_state['values'];

  $paths = array();

  if (!empty($values['path'])) {
    $paths[] = $values['path'];
  }

  if ($values['components']['themes']) {
    foreach (list_themes() as $theme) {
      if ($theme->status) {
        // figure out the theme folder's path from the .info file's filepath
        $path_parts = explode('/', $theme->filename);
        array_pop($path_parts);

        $paths[] = implode('/', $path_parts);
      }
    }
  }

  if (!empty($paths)) {
    // clean up possible leading & trailing slashes
    foreach ($paths as &$path) {
      $path = trim($path, '/');
    }

    $cdn = \devtools\components\CDN::instance();

    switch ($values['operation']) {
      case 'touch':
        foreach ($paths as $path) {
          $cdn->touch($path);
        }

        break;

      case 'cloudfront':
        if ($cf = $cdn->get('cloudfront')) {
          $cf->invalidate($paths);
        }

        break;
    }
  }
}