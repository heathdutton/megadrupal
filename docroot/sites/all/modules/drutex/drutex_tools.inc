<?php

/**
 * @file
 * Tools for more debugging and error logging.
 */

/**
 * Implements hook_info().
 */
function drutex_tools_info($filter) {
  return (object) array(
    'title' => t('Toolbox'),
    'description' => t('Tools for more debugging and error logging.'),
    'toggle' => FALSE,
    'weight' => 20,
  );
}

/**
 * Implements hook_defaults().
 */
function drutex_tools_defaults() {
  $D['drutex_tools_active'] = TRUE;
  $D['drutex_debug'] = FALSE;

  return $D;
}

/**
 * Implements hook_filter_settings().
 */
function drutex_tools_filter_settings($filter) {
  $choices = array(
    'drupal' => 'Drupal style',
    'latex' => 'LaTeX style',
  );

  $form["drutex_debug"] = array(
    '#type' => 'checkbox',
    '#title' => t('Debugging mode'),
    '#default_value' => $filter->settings['drutex_tools']['drutex_debug'],
    '#description' => t('If enabled, the temporary files will no longer be deleted automatically. Also enables super-verbose logging.'),
  );

  $div_error = '<div class="filter-settings-error">';
  $div_correct = '<div class="filter-settings-correct">';

  /*
   Temporary directory check
   */
  $temporary_dir = $filter->settings['drutex_base']['drutex_dir_temporary'];
  $is_writable = is_writable($temporary_dir);

  if ($is_writable == FALSE) {
    $form["drutex_check_tmpdir"] = array(
      '#markup' => $div_error . t('The temporary directory (') . $temporary_dir . ') isn\'t writable.' . '</div>',
    );
  }
  else {
    $form["drutex_check_tmpdir"] = array(
      '#markup' => $div_correct . t('The temporary directory is writable.') . '</div>',
    );
  }

  /*
   Image directory check
   */
  $image_dir = $filter->settings['drutex_base']['drutex_url_images'];
  $is_writable = is_writable($image_dir);

  if ($is_writable == FALSE) {
    $form["drutex_check_imgdir"] = array(
      '#markup' => $div_error . t('The image directory (') . $image_dir . ') isn\'t writable.' . '</div>',
    );
  }
  else {
    $form["drutex_check_imgdir"] = array(
      '#markup' => $div_correct . t('The image directory is writable.') . '</div>',
    );
  }

  /*
   LaTeX check - We grab -version which should work on all platforms
   */
  exec('latex -version', $latex_output, $latex_return_value);

  if ($latex_return_value == 127) {
    $form["drutex_check_latex"] = array(
      '#markup' => $div_error . t('Executing "latex" failed. Either LaTeX isn\'t installed or the "latex" executable is not in one of the search paths. <br /> You cannot render high-quality images without LaTeX. <br /> Mimetex maybe an alternative for you. Consult the documentation on how to use Mimetex.') . '</div>',
    );
  }
  else {
    $form["drutex_check_latex"] = array(
      '#markup' => $div_correct . t('Executing "latex" was successful: ') . "<tt>{$latex_output[0]}</tt>" . '</div>',
    );
  }

  /*
   dvipng check
   */
  exec('dvipng', $dvipng_output, $dvipng_return_value);

  if ($dvipng_return_value == 127) {
    $form["drutex_check_dvipng"] = array(
      '#markup' => $div_error . t('Executing "dvipng" failed. Either dvipng isn\'t installed or the "dvipng" executable is not in one of the search paths. <br /> You cannot use dvipng for rendering images (check the conversion method above).') . '</div>',
    );
  }
  else {
    $form["drutex_check_dvipng"] = array(
      '#markup' => $div_correct . t('Executing "dvipng" was successful: ') . "<tt>{$dvipng_output[0]}</tt>" . '</div>',
    );
  }

  /*
   Imagemagick check
   */
  exec('convert', $imagemagick_output, $imagemagick_return_value);

  if ($imagemagick_return_value == 127) {
    $form["drutex_check_imagemagick"] = array(
      '#markup' => $div_error . t('Executing "convert" (ImageMagick) failed. Either ImageMagick isn\'t installed or the "convert" executable is not in one of the search paths. <br /> You cannot use ImageMagick for rendering images (check the conversion method above).') . '</div>',
    );
  }
  else {
    $form["drutex_check_imagemagick"] = array(
       '#markup' => $div_correct . t('Executing "convert" (ImageMagick) was successful: ') . "<tt>{$imagemagick_output[0]}</tt>" . '</div>',
    );
  }

  return $form;
}
