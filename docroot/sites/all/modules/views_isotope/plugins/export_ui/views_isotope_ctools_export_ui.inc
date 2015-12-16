<?php
/**
 * @file
 * Define this Export UI plugin.
 */

$plugin = array(
  // As defined in hook_schema().
  'schema' => 'isotope_configurations',
  // Define a permission users must have to access these pages.
  'access' => 'administer isotope',
  // Define the menu item.
  'menu' => array(
    'menu item' => 'isotope',
    'menu title' => 'Isotope Configurations',
    'menu description' => 'Administer Isotope configurations.',
  ),
  // Define user interface texts.
  'title singular' => t('config'),
  'title plural' => t('configs'),
  'title singular proper' => t('Isotope Configuration'),
  'title plural proper' => t('Isotope Configurations'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'views_isotope_ctools_export_ui_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),
);

/**
 * Define the configuration add/edit form.
 */
function views_isotope_ctools_export_ui_form(&$form, &$form_state) {
  $config = $form_state['item'];

  $ajax_layout_mode = '';

  $libraries = views_isotope_additional_libraries_list();
  $detected_libraries = views_isotope_check_additional_libraries();

  $layouts_included = array(
    'masonry' => t('masonry'),
    'fitRows' => t('fitRows'),
    'vertical' => t('vertical'),
  );

  $layouts_not_included = array();
  foreach ($libraries as $lib_name => $library) {
    if ($library['type'] == 'layout') {
      $layouts_not_included[$lib_name] = $lib_name;
    }
  }

  $form['layoutMode'] = array(
    '#type' => 'radios',
    '#options' => array_merge($layouts_included, $layouts_not_included),
    '#title' => t('Isotope Layout Mode'),
    '#description' => t('If desired options are disabled, you may need to install additional JS libraries. Refer to installation instructions.'),
    '#default_value' => ($config->layoutMode) ? $config->layoutMode : 'masonry',
    '#required' => FALSE,
  );

  // Disable the option for libraries that are not included.
  foreach ($layouts_not_included as $key => $value) {
    if (!in_array($key, $detected_libraries)) {
      $form['layoutMode'][$key] = array('#disabled' => TRUE);
    }
  }

  $plugins = array();
  foreach ($libraries as $lib_name => $library) {
    if ($library['type'] == 'plugin') {
      $plugins[$lib_name] = $lib_name;
    }
  }

  $form['plugins'] = array(
    '#type' => 'checkboxes',
    '#options' => $plugins,
    '#title' => t('Additional Plugins'),
    '#description' => t('If desired options are disabled, you may need to install additional JS libraries.'),
    '#default_value' => ($config->plugins) ? $config->plugins : array(),
    '#required' => FALSE,
    '#tree' => TRUE,
  );

  // Disable the option for libraries that are not included.
  foreach ($plugins as $key => $value) {
    if (!in_array($key, $detected_libraries)) {
      $form['plugins'][$key] = array('#disabled' => TRUE);
    }
  }

  $form['transitionDuration'] = array(
    '#type' => 'textfield',
    '#title' => t('Transition Duration'),
    '#description' => t('To disable all transitions, set transitionDuration: 0.'),
    '#default_value' => $config->transitionDuration,
    '#required' => FALSE,
  );

  if ($ajax_layout_mode == 'masonry') {
    $form['isFitWidth'] = array(
      '#type' => 'select',
      '#options' => array(
        0 => 'False',
        1 => 'True',
      ),
      '#title' => t('isFitWidth'),
      '#description' => t("Sets the width of the container to fit the available number of columns, based the size of container's parent element. When enabled, you can center the container with CSS."),
      '#default_value' => ($config->isFitWidth) ? $config->isFitWidth : 0,
      '#required' => FALSE,
    );
  }

  if ($ajax_layout_mode == 'packery') {
    $form['isHorizontal'] = array(
      '#type' => 'select',
      '#options' => array(
        0 => 'False',
        1 => 'True',
      ),
      '#title' => t('isHorizontal'),
      '#description' => t('Arranges items horizontally instead of vertically.'),
      '#default_value' => ($config->isHorizontal) ? $config->isHorizontal : 0,
      '#required' => FALSE,
    );
  }

  if ($ajax_layout_mode == 'masonry' || $ajax_layout_mode == 'packery' || $ajax_layout_mode == 'masonryHorizontal') {
    $form['stamp'] = array(
      '#type' => 'textfield',
      '#title' => t('Stamp Selector'),
      '#description' => t('Specifies elements that are stamped within the layout. These are special layout elements which will not be laid out. Rather, Isotope will layout item elements below stamped elements.'),
      '#default_value' => $config->stamp,
      '#required' => FALSE,
    );
  }

  if ($ajax_layout_mode == 'vertical') {
    $form['horizontalAlignment'] = array(
      '#type' => 'textfield',
      '#title' => t('Horizontal Alignment (decimal number 0 to 1)'),
      '#description' => t('Aligns items horizontally. 0 will align the origin edge. 1 will align the opposite edge. 0.5 will align center.'),
      '#default_value' => $config->horizontalAlignment,
      '#required' => FALSE,
    );
  }

  if ($ajax_layout_mode == 'horizontal') {
    $form['verticalAlignment'] = array(
      '#type' => 'textfield',
      '#title' => t('Vertical Alignment (decimal number 0 to 1)'),
      '#description' => t('Aligns items vertically. 0 will align the origin edge. 1 will align the opposite edge. 0.5 will align center.'),
      '#default_value' => $config->verticalAlignment,
      '#required' => FALSE,
    );
  }

  foreach ($libraries as $lib_name => $library) {
    if (!in_array($lib_name, $detected_libraries)) {
      $lib_link = l($lib_name, $library['url']);
      $lib_path = libraries_get_path($lib_name);
      $lib_path = !empty($lib_path) ? $lib_path : libraries_get_path('isotope');
      if (empty($lib_path)) {
        if ($library['type'] == 'plugin') {
          $lib_path = 'libraries/' . $lib_name;
        }
        if ($library['type'] == 'layout') {
          $lib_path = 'libraries/isotope';
        }
      }
      $lib_path .= '/' . $library['filename'];
      $item = '<strong>' . $lib_link . '</strong><br />';
      $item .= '<em>' . t('Install into: ') . $lib_path . '</em><br />';
      $item .= $library['description'];
      $items[] = $item;
    }
  }
  if (!empty($items)) {
    $form['instructions'] = array(
      '#title' => t('Installing Additional Plugins'),
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => $items,
    );
  }
}
