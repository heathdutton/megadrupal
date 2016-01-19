<?php

/**
 * @file
 * Implement a lightbox field, based on the Nivo Lightbox jQuery Plugin.
 */

/**
 * Implements hook_field_formatter_info().
 */
function nivo_lightbox_field_formatter_info() {
  $formatters['nivo_formatter'] = array(
    'label' => t('Nivo Lightbox'),
    'field types' => array('image'),
    'settings' => array(
      'nivo_lightbox_target_image_style' => '',
      'nivo_lightbox_lightbox_image_style' => '',
      'nivo_lightbox_effect' => 'fade',
      'nivo_lightbox_theme' => 'default',
      'nivo_lightbox_keyboard_nav' => TRUE,
      'nivo_lightbox_lightbox_type' => '',
      'nivo_lightbox_error_message' => 'The requested content cannot be loaded. Please try again later.',
    ),
  );

  if (module_exists('retina_images')) {
    $formatters['nivo_formatter']['settings'] += array(
      'nivo_lightbox_hidpi' => '',
      'nivo_lightbox_hidpi_image_style' => FALSE,
    );
  }

  if (module_exists('file_entity')) {
    $formatters['nivo_formatter']['field types'][] = 'file';
    $formatters['nivo_formatter']['settings'] += array(
      'nivo_lightbox_target_view_mode' => '',
      'nivo_lightbox_html_lightbox' => FALSE,
      'nivo_lightbox_lightbox_view_mode' => '',
    );
  }

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function nivo_lightbox_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $type = 'file';
  if (!empty($field['type'])) {
    if ($field['type'] == 'image') {
      $type = 'image';
    }
  }

  switch ($type) {
    case 'image':
      $image_styles = image_style_options(FALSE);
      $element['nivo_lightbox_target_image_style'] = array(
        '#title' => t('Image style'),
        '#type' => 'select',
        '#default_value' => $settings['nivo_lightbox_target_image_style'],
        '#empty_option' => t('None (original image)'),
        '#options' => $image_styles,
      );
      $element['nivo_lightbox_lightbox_image_style'] = array(
        '#title' => t('Lightbox style'),
        '#type' => 'select',
        '#default_value' => $settings['nivo_lightbox_lightbox_image_style'],
        '#empty_option' => t('None (original image)'),
        '#options' => $image_styles,
      );
      if (module_exists('retina_images')) {
        $element['nivo_lightbox_hidpi'] = array(
          '#title' => t('Use HiDPI images'),
          '#type' => 'checkbox',
          '#default_value' => $settings['nivo_lightbox_hidpi'],
        );
        $element['nivo_lightbox_hidpi_image_style'] = array(
          '#title' => t('HiDPI style'),
          '#type' => 'select',
          '#default_value' => $settings['nivo_lightbox_hidpi_image_style'],
          '#empty_option' => t('None (original image)'),
          '#options' => $image_styles,
          '#states' => array(
            'visible' => array(
              ':input[name="fields[field_image][settings_edit_form][settings][nivo_lightbox_hidpi]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
      break;
    case 'file':
      $view_modes = array();
      $entity_info = entity_get_info('file');
      $file_view_modes = $entity_info['view modes'];
      foreach ($file_view_modes as $view_mode_name => $view_mode_info) {
        $view_modes[$view_mode_name] = $view_mode_info['label'];
      }
      // Unset the current view mode to prevent recursion.
      $current_view_mode = ($view_mode == 'default') ? 'full' : $view_mode;
      unset($view_modes[$current_view_mode]);
      $element['nivo_lightbox_target_view_mode'] = array(
        '#title' => t('Target view mode'),
        '#type' => 'select',
        '#default_value' => $settings['nivo_lightbox_target_view_mode'],
        '#options' => $view_modes,
      );
      $element['nivo_lightbox_html_lightbox'] = array(
        '#title' => t('HTML lightbox'),
        '#type' => 'checkbox',
        '#default_value' => $settings['nivo_lightbox_html_lightbox'],
      );
      $element['nivo_lightbox_lightbox_view_mode'] = array(
        '#title' => t('Lightbox view mode'),
        '#type' => 'select',
        '#default_value' => $settings['nivo_lightbox_lightbox_view_mode'],
        '#options' => $view_modes,
        '#states' => array(
          'visible' => array(
            ':input[name="fields[field_file][settings_edit_form][settings][nivo_lightbox_html_lightbox]"]' => array('checked' => TRUE),
          ),
        ),
      );
      break;
  }

  $effects = array(
    'fade' => t('Fade'),
    'fadeScale' => t('Fast scale'),
    'slideLeft' => t('Slide left'),
    'slideRight' => t('Slide right'),
    'slideUp' => t('Slide up'),
    'slideDown' => t('Slide down'),
    'fall' => t('Fall'),
  );
  $element['nivo_lightbox_effect'] = array(
    '#title' => t('Effect'),
    '#type' => 'select',
    '#default_value' => $settings['nivo_lightbox_effect'],
    '#options' => $effects,
  );

  // Create a list of the currently available themes
  $themes = module_invoke_all('nivo_lightbox_theme_info');

  // Allow theme information to be altered
  drupal_alter('nivo_lightbox_theme_info', $themes);

  $available_themes = array();

  foreach ($themes as $theme => $properties) {
    $available_themes[$theme] = $properties['name'];
  }

  $element['nivo_lightbox_theme'] = array(
    '#title' => t('Theme'),
    '#type' => 'select',
    '#default_value' => $settings['nivo_lightbox_theme'],
    '#empty_option' => t('None (no theme)'),
    '#options' => $available_themes,
  );

  $element['nivo_lightbox_keyboard_nav'] = array(
    '#title' => t('Keyboard navigation'),
    '#type' => 'checkbox',
    '#default_value' => $settings['nivo_lightbox_keyboard_nav'],
  );

  $types = array(
    'ajax' => t('Load content via AJAX'),
  );
  $element['nivo_lightbox_lightbox_type'] = array(
    '#title' => t('Lightbox type'),
    '#type' => 'select',
    '#default_value' => $settings['nivo_lightbox_lightbox_type'],
    '#empty_option' => t('Default'),
    '#options' => $types,
  );

  $element['nivo_lightbox_error_message'] = array(
    '#title' => t('Error message'),
    '#type' => 'textfield',
    '#default_value' => $settings['nivo_lightbox_error_message'],
  );

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function nivo_lightbox_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $summary = array();

  $type = 'file';
  if (!empty($field['type'])) {
    if ($field['type'] == 'image') {
      $type = 'image';
    }
  }

  switch ($type) {
    case 'image':
      $image_styles = image_style_options(FALSE);
      // Unset possible 'No defined styles' option.
      unset($image_styles['']);
      // Styles could be lost because of enabled/disabled modules that define
      // their styles in code.
      if (isset($image_styles[$settings['nivo_lightbox_target_image_style']])) {
        $summary[] = t('Image style: @style', array('@style' => $image_styles[$settings['nivo_lightbox_target_image_style']]));
      }
      else {
        $summary[] = t('Original image');
      }
      if (isset($image_styles[$settings['nivo_lightbox_lightbox_image_style']])) {
        $summary[] = t('Lightbox style: @style', array('@style' => $image_styles[$settings['nivo_lightbox_lightbox_image_style']]));
      }
      else {
        $summary[] = t('Original image');
      }
      if (module_exists('retina_images')) {
        if (!empty($settings['nivo_lightbox_hidpi'])) {
          if (isset($image_styles[$settings['nivo_lightbox_hidpi_image_style']])) {
            $summary[] = t('HiDPI style: @style', array('@style' => $image_styles[$settings['nivo_lightbox_hidpi_image_style']]));
          }
          else {
            $summary[] = t('Original image');
          }
        }
      }
      break;
    case 'file':
      $view_modes = array();
      $entity_info = entity_get_info('file');
      $file_view_modes = $entity_info['view modes'];
      foreach ($file_view_modes as $view_mode_name => $view_mode_info) {
        $view_modes[$view_mode_name] = $view_mode_info['label'];
      }
      // Unset the current view mode to prevent recursion.
      $current_view_mode = ($view_mode == 'default') ? 'full' : $view_mode;
      unset($view_modes[$current_view_mode]);
      // View modes could be lost because of enabled/disabled modules that
      // define their view modes in code.
      if (isset($view_modes[$settings['nivo_lightbox_target_view_mode']])) {
        $summary[] = t('View mode: @view_mode', array('@view_mode' => $view_modes[$settings['nivo_lightbox_target_view_mode']]));
      }
      else {
        $summary[] = t('Default view mode');
      }
      if (!empty($settings['nivo_lightbox_html_lightbox'])) {
        if (isset($view_modes[$settings['nivo_lightbox_lightbox_view_mode']])) {
          $summary[] = t('View mode: @view_mode', array('@view_mode' => $view_modes[$settings['nivo_lightbox_lightbox_view_mode']]));
        }
        else {
          $summary[] = t('Default view mode');
        }
      }
      break;
  }

  $effects = array(
    'fade' => t('Fade'),
    'fadeScale' => t('Fast scale'),
    'slideLeft' => t('Slide left'),
    'slideRight' => t('Slide right'),
    'slideUp' => t('Slide up'),
    'slideDown' => t('Slide down'),
    'fall' => t('Fall'),
  );

  // Display this setting only if an effect has been chosen.
  if (isset($effects[$settings['nivo_lightbox_effect']])) {
    $summary[] = t('Effect: @effect', array('@effect' => $effects[$settings['nivo_lightbox_effect']]));
  }

  // Create a list of the currently available themes
  $themes = module_invoke_all('nivo_lightbox_theme_info');

  // Allow theme information to be altered
  drupal_alter('nivo_lightbox_theme_info', $themes);

  $available_themes = array();

  foreach ($themes as $theme => $properties) {
    $available_themes[$theme] = $properties['name'];
  }

  // Themes could be lost because of enabled/disabled modules that defines
  // their themes in code.
  if (isset($available_themes[$settings['nivo_lightbox_theme']])) {
    $summary[] = t('Theme: @theme', array('@theme' => $available_themes[$settings['nivo_lightbox_theme']]));
  }
  else {
    $summary[] = t('No theme');
  }

  return implode('<br />', $summary);
}

/**
 * Implements hook_field_formatter_view().
 */
function nivo_lightbox_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  $theme = 'nivo_lightbox_file_formatter';
  if (!empty($field['type'])) {
    if ($field['type'] == 'image') {
      $theme = 'nivo_lightbox_image_formatter';
    }
  }

  list($id, ,) = entity_extract_ids($entity_type, $entity);
  $id = drupal_html_id('nivo-lightbox' . '-' . $id);

  foreach ($items as $delta => $item) {
    $element[$delta] = array(
      '#theme' => $theme,
      '#item' => $item,
      '#id' => $id,
      '#gallery' => count($items) > 1 ? TRUE : NULL,
      '#group' => !empty($instance['id']) ? $instance['id'] : NULL,
      '#target_image_style' => $display['settings']['nivo_lightbox_target_image_style'],
      '#lightbox_image_style' => $display['settings']['nivo_lightbox_lightbox_image_style'],
      '#lightbox_type' => $display['settings']['nivo_lightbox_lightbox_type'],
      '#attached' => array(
        'libraries_load' => array(
          array('nivo-lightbox'),
        ),
        'js' => array(
          array(
            'data' => drupal_get_path('module', 'nivo_lightbox') . '/js/nivo_lightbox.js',
            'type' => 'file',
          ),
          array(
            'data' => array(
              'nivo_lightbox' => array(
                array(
                  'id' => $id,
                  'effect' => $display['settings']['nivo_lightbox_effect'],
                  'theme' => $display['settings']['nivo_lightbox_theme'],
                  'keyboardNav' => $display['settings']['nivo_lightbox_keyboard_nav'],
                  'errorMessage' => $display['settings']['nivo_lightbox_error_message'],
                ),
              ),
            ),
            'type' => 'setting',
          ),
        ),
      ),
    );

    if (module_exists('retina_images')) {
      $element[$delta]['#hidpi'] = $display['settings']['nivo_lightbox_hidpi'];
      $element[$delta]['#hidpi_image_style'] = $display['settings']['nivo_lightbox_hidpi_image_style'];
    }

    if (module_exists('file_entity')) {
      $element[$delta]['#target_view_mode'] = $display['settings']['nivo_lightbox_target_view_mode'];
      $element[$delta]['#html_lightbox'] = $display['settings']['nivo_lightbox_html_lightbox'];
      $element[$delta]['#lightbox_view_mode'] = $display['settings']['nivo_lightbox_lightbox_view_mode'];
    }

    // Create a list of the currently available themes
    $themes = module_invoke_all('nivo_lightbox_theme_info');

    // Allow theme information to be altered
    drupal_alter('nivo_lightbox_theme_info', $themes);

    // Find the currently selected theme
    $current_theme = $display['settings']['nivo_lightbox_theme'];

    // Themes could be lost because of enabled/disabled modules that defines
    // their themes in code.
    if (isset($themes[$display['settings']['nivo_lightbox_theme']])) {
      // Add the theme's resources
      foreach (array('js', 'css') as $type) {
        if (!empty($themes[$display['settings']['nivo_lightbox_theme']]['resources'][$type])) {
          foreach ($themes[$display['settings']['nivo_lightbox_theme']]['resources'][$type] as $file_path) {
            $element[$delta]['#attached'][$type][] = $file_path;
          }
        }
      }
    }
  }

  return $element;
}
