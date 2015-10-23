<?php
/**
 * @file
 * Administration pages for the Yandex.Share module.
 */

/**
 * Returns settings form.
 */
function yashare_admin_settings() {
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'yashare') . '/js/yashare.admin.js'),
    ),
  );
  $form['settings']['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );
  $form['settings']['general']['yashare_init_type'] = array(
    '#type' => 'radios',
    '#title' => t('Initialization type'),
    '#description' => t('Some configuration parameters are not supported by "Automatic" initialization type.'),
    '#options' => array(
      'automatic' => t('Automatic'),
      'standard' => t('Standard'),
    ),
    '#default_value' => variable_get('yashare_init_type', 'automatic'),
  );
  $form['settings']['general']['yashare_l10n'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => yashare_get_languages(),
    '#default_value' => yashare_get_language(),
  );

  $options = array(
    'default_button' => yashare_admin_widget('default', 'button'),
    'default_link' => yashare_admin_widget('default', 'link'),
    'default_icon' => yashare_admin_widget('default', 'icon'),
    'default_none' => yashare_admin_widget('default', 'none'),
    'dark_button' => yashare_admin_widget('dark', 'button'),
    'dark_link' => yashare_admin_widget('dark', 'link'),
    'dark_icon' => yashare_admin_widget('dark', 'icon'),
    'dark_none' => yashare_admin_widget('dark', 'none'),
    'counter_button' => yashare_admin_widget('counter', 'button'),
    'counter_small' => yashare_admin_widget('counter', 'small'),
  );
  $form['settings']['general']['yashare_theme_type'] = array(
    '#type' => 'radios',
    '#title' => t('Style'),
    '#options' => $options,
    '#default_value' => variable_get('yashare_theme', 'default') . '_' . variable_get('yashare_type', 'button'),
  );

  $form['settings']['general']['yashare_block_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Block title'),
    '#default_value' => variable_get('yashare_block_title', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="yashare_theme_type"]' => array(
          array('value' => 'default_button'),
          array('value' => 'default_link'),
          array('value' => 'dark_button'),
          array('value' => 'dark_link'),
        ),
      ),
    ),
  );
  $form['settings']['general']['yashare_border'] = array(
    '#type' => 'checkbox',
    '#title' => t('Block border'),
    '#default_value' => variable_get('yashare_border', FALSE),
    '#states' => array(
      'invisible' => array(
        ':input[name="yashare_theme_type"]' => array(
          array('value' => 'counter_button'),
          array('value' => 'counter_small'),
        ),
      ),
    ),
  );
  $form['settings']['general']['yashare_linkunderline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Underline block link'),
    '#default_value' => variable_get('yashare_linkunderline', FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="yashare_theme_type"]' => array(
          array('value' => 'default_link'),
          array('value' => 'dark_link'),
        ),
      ),
    ),
  );
  $form['settings']['general']['yashare_linkicon'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Yandex.Share icon to the link'),
    '#default_value' => variable_get('yashare_linkicon', FALSE),
    '#states' => array(
      'visible' => array(
        ':input[name="yashare_theme_type"]' => array(
          array('value' => 'default_link'),
          array('value' => 'dark_link'),
        ),
      ),
    ),
  );
  $form['settings']['general']['yashare_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pass site logo as image to share'),
    '#default_value' => variable_get('yashare_image', FALSE),
  );

  $services = yashare_services_list();
  // Block settings.
  $form['settings']['block_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block services'),
    '#description' => t('Choose services to show in block.'),
  );

  $header = array(
    'name' => t('Name'),
    'dark' => t('Dark theme'),
    'counter' => t('Counters theme'),
  );
  $options = array();
  $weight = -30;
  $enabled_services = yashare_get_enabled_services('block');
  $form['settings']['block_settings']['yashare_block_services'] = array();
  foreach (array_keys($enabled_services + $services) as $codename) {
    $options[$codename] = array(
      'name' => $services[$codename]['name'],
      'dark' => $services[$codename]['dark'] ? t('Yes') : t('No'),
      'counter' => $services[$codename]['counter'] ? t('Yes') : t('No'),
      '#attributes' => array('class' => array('draggable')),
    );
    $form['settings']['block_settings']['yashare_block_services'][$codename . '_block_weight'] = array(
      '#tree' => FALSE,
      '#type' => 'weight',
      '#delta' => 30,
      '#default_value' => $weight++,
      '#attributes' => array('class' => array('yashare-block-services-weight')),
    );
  }
  $form['settings']['block_settings']['yashare_block_services'] += array(
    '#type' => 'tableselect',
    '#title' => t('Block services'),
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $enabled_services,
    '#pre_render' => array('yashare_block_services_pre_render'),
  );

  // Popup settings.
  $form['settings']['popup_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Popup settings'),
    '#description' => t('Popup settings are ignored if you are using Counters theme.'),
  );
  $form['settings']['popup_settings']['yashare_popup_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Popup title'),
    '#default_value' => variable_get('yashare_popup_title', ''),
  );
  $form['settings']['popup_settings']['yashare_popup_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show link for copy'),
    '#default_value' => variable_get('yashare_popup_link', FALSE),
  );
  $form['settings']['popup_settings']['yashare_popup_codeforblog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show code for blog'),
    '#default_value' => variable_get('yashare_popup_codeforblog', FALSE),
  );
  $form['settings']['popup_settings']['yashare_popup_vdirection'] = array(
    '#type' => 'select',
    '#title' => t('Popup direction'),
    '#options' => array(
      0 => t('Auto'),
      'up' => t('Up'),
      'down' => t('Down'),
    ),
    '#default_value' => variable_get('yashare_popup_vdirection', 0),
  );

  $header = array(
    'name' => t('Name'),
    'dark' => t('Dark theme'),
  );
  $options = array();
  $weight = -30;
  $enabled_services = yashare_get_enabled_services('popup');
  $form['settings']['popup_settings']['yashare_popup_services'] = array();
  foreach (array_keys($enabled_services + $services) as $codename) {
    $options[$codename] = array(
      'name' => $services[$codename]['name'],
      'dark' => $services[$codename]['dark'] ? t('Yes') : t('No'),
      '#attributes' => array('class' => array('draggable')),
    );
    $form['settings']['popup_settings']['yashare_popup_services'][$codename . '_popup_weight'] = array(
      '#tree' => FALSE,
      '#type' => 'weight',
      '#delta' => 30,
      '#default_value' => $weight++,
      '#attributes' => array('class' => array('yashare-popup-services-weight')),
    );
  }
  $form['settings']['popup_settings']['yashare_popup_services'] += array(
    '#type' => 'tableselect',
    '#title' => t('Popup services'),
    '#header' => $header,
    '#options' => $options,
    '#default_value' => $enabled_services,
    '#pre_render' => array('yashare_popup_services_pre_render'),
  );

  $form['#validate'] = array('yashare_admin_settings_validate');
  return system_settings_form($form);
}

/**
 * Pre-render callback for the block services tableselect.
 */
function yashare_block_services_pre_render($element) {
  return yashare_services_tableselect_pre_render($element, 'block');
}

/**
 * Pre-render callback for the popup services tableselect.
 */
function yashare_popup_services_pre_render($element) {
  return yashare_services_tableselect_pre_render($element, 'popup');
}

/**
 * Pre-render callback helper.
 */
function yashare_services_tableselect_pre_render($element, $type) {
  // Add weight column.
  $element['#header']['weight'] = t('Weight');
  foreach (array_keys($element['#options']) as $codename) {
    $key = $codename . '_' . $type . '_weight';
    $element['#options'][$codename]['weight'] = array(
      'data' => drupal_render($element[$key]),
    );
    unset($element[$key]);
  }

  // Assign id to the table.
  $table_id = 'yashare-' . $type . '-services';
  $element['#attributes'] = array('id' => $table_id);
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'yashare-' . $type . '-services-weight');
  return $element;
}

function yashare_admin_settings_validate($form, &$form_state) {
  $values = &$form_state['values'];

  list($theme, $type) = explode('_', $values['yashare_theme_type']);
  $values['yashare_theme'] = $theme;
  $values['yashare_type'] = $type;
  unset($values['yashare_theme_type']);

  $services = yashare_services_list();
  // Remove weights so they are not saved as variables.
  foreach (array_keys($services) as $codename) {
    $block_services[$codename] = $values[$codename . '_block_weight'];
    $popup_services[$codename] = $values[$codename . '_popup_weight'];
    unset($values[$codename . '_block_weight']);
    unset($values[$codename . '_popup_weight']);
  }
  asort($block_services);
  asort($popup_services);

  $block_services_values = $values['yashare_block_services'];
  $popup_services_values = $values['yashare_popup_services'];
  unset($values['yashare_block_services']);
  unset($values['yashare_popup_services']);

  foreach (array_keys($block_services) as $codename) {
    $values['yashare_block_services'][$codename] = $block_services_values[$codename];
  }
  foreach (array_keys($popup_services) as $codename) {
    $values['yashare_popup_services'][$codename] = $popup_services_values[$codename];
  }
}

/**
 * Render Yandex.Share block for admin settings form.
 */
function yashare_admin_widget($theme, $type) {
  $element = array(
    '#type' => 'yashare_widget',
    '#html_tag' => 'span',
    '#link' => url('<front>', array('absolute' => TRUE)),
    '#title' => variable_get('site_name', 'Drupal'),
    '#yashare_theme' => $theme,
    '#yashare_type' => $type,
  );
  $map = array(
    'default' => t('Default'),
    'dark' => t('Dark'),
    'counter' => t('Counters'),
  );
  return $map[$theme] . ' theme | ' . drupal_render($element);
}
