<?php

/**
 * Menu callback; mojeID admin settings form.
 */
function mojeid_admin_settings() {
  module_load_include('inc', 'mojeid');

  $form['#attached'] = array(
    'css' => array(
      'type' => 'file',
      'data' => drupal_get_path('module', 'mojeid') . '/mojeid.css',
    ),
  );

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
  );

  $form['basic']['mojeid_server'] = array(
    '#type' => 'radios',
    '#title' => t('Server'),
    '#options' => array(
      MOJEID_SERVER => t('Production server (@uri)', array('@uri' => MOJEID_SERVER)),
      MOJEID_SERVER_TEST => t('Testing server (@uri)', array('@uri' => MOJEID_SERVER_TEST)),
    ),
    '#description' => t("Select testing server if you want to develop some extension or if you want to test the real communictation with the mojeID provider e.g. during account registration. Note that you will probably need to contact mojeID provider to validate your domain URI to access all of the mojeID testing services."),
    '#default_value' => variable_get('mojeid_server', MOJEID_SERVER),
  );

  $form['basic']['mojeid_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Extensive logging'),
    '#description' => t("If enabled, all requests and responses to or from the mojeID will be logged. Enable only for testing or development purposes."),
    '#default_value' => variable_get('mojeid_logging', FALSE),
  );

  $form['basic']['mojeid_supress_openid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Supress OpenID'),
    '#description' => t('Suppress general usage of OpenID on this site, only mojeID will be allowed.'),
    '#default_value' => variable_get('mojeid_supress_openid', FALSE),
  );


  $form['buttons'] = array(
    '#type' => 'fieldset',
    '#title' => t('Buttons and appearance'),
  );

  $form['buttons']['mojeid_login_method'] = array(
    '#type' => 'radios',
    '#title' => t('Login method'),
    '#options' => array(
      'username' => t('Require username'),
      'direct' => t('Direct login'),
    ),
    '#default_value' => variable_get('mojeid_login_method', 'username'),
    '#description' => t('Selects mojeID login method for the login form. The <em>Direct login</em> method will display just login button without any input field. The <em>Require username</em> method will display standard username input field.'),
  );

  $form['buttons']['mojeid_direct_login_button'] = array(
    '#type' => 'radios',
    '#title' => t('Direct login button'),
    '#options' => array(
      'basic' => t('Basic form button'),
      'image' => t('Image button'),
    ),
    '#default_value' => variable_get('mojeid_direct_login_button', 'basic'),
  );

  $form['buttons']['mojeid_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Buttons theme'),
    '#options' => array(
      'silver_155x24' => t('Silver 155x24'),
      'black_155x24' => t('Black 155x24'),
      'silver_162x32' => t('Silver 162x32'),
      'black_162x32' => t('Black 162x32'),
    ),
    '#default_value' => variable_get('mojeid_theme', 'silver_155x24'),
  );

  $form['banners'] = array(
    '#type' => 'fieldset',
    '#title' => t('Banners for mojeID login promotion'),
  );

  $form['banners']['mojeid_promo_icons'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Where to display'),
    '#options' => array(
      'user_login_block' => t('User login block'),
      'user_login' => t('User login form'),
      'user_register' => t('User register form'),
      'openid_user_add' => t('OpenID form under user account'),
    ),
    '#default_value' => variable_get('mojeid_promo_icons', array()),
    '#description' => t('Display Powered by mojeID or mojeID Login banners on the checked forms.'),
  );

  $icons = array('A1', 'A2', 'A3', 'A4', 'B1', 'B2', 'B3', 'B4');
  $icons_dir = drupal_get_path('module', 'mojeid') . '/img/powered_small/';
  foreach ($icons as $name) {
    $localized_filepath = mojeid_file_language_variant($icons_dir . $name . '.png');
    $promo_icons[$name] = $name . '<br />' . theme('image', array('path' => $localized_filepath, 'title' => $name, 'alt' => $name));
  }

  $form['banners']['mojeid_banner_type'] = array(
    '#type' => 'radios',
    '#title' => t('Banner type'),
    '#options' => $promo_icons,
    '#default_value' => variable_get('mojeid_banner_type', 'A1'),
  );

  $form['banners']['mojeid_banner_size'] = array(
    '#type' => 'radios',
    '#title' => t('Banner size'),
    '#options' => array(
      'small' => t('Small'),
      'big' => t('Big'),
      'giant' => t('Giant'),
    ),
    '#default_value' => variable_get('mojeid_banner_size', 'big'),
  );

  if (module_exists('openid_client_ax')) {
    $schema = openid_client_ax_schema_definitions();
    $supported_elements = mojeid_supported_elements();
    foreach ($supported_elements as $uri) {
      if (isset($schema[$uri])) {
        $short_name = openid_client_ax_attribute_name($uri);
        $section = $schema[$uri]['section'];
        $options[$section][$short_name] = $schema[$uri]['label'] . ' <small>(' . $uri . ')</small>';
      }
      else {
        drupal_set_message(t("Element %uri should be supported but it wasn't found in the schema definition.", array('%uri' => $uri)), 'warning');
      }
    }

    $form['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['advanced']['mojeid_required'] = array(
      '#type' => 'fieldset',
      '#title' => t('mojeID required attributes'),
      '#description' => t('Checked mojeID attributes will be required when user is loging-in. Do it only if you really need it.'),
      '#tree' => TRUE
    );

    $required_defaults = variable_get('mojeid_required', array());
    foreach ($options as $section => $section_options) {
      $form['advanced']['mojeid_required'][] = array(
        '#type' => 'checkboxes',
        '#title' => t('Section @section', array('@section' => $section)),
        '#options' => $section_options,
        '#default_value' => array_intersect_key($required_defaults, $section_options),
      );
    }
  }

  $form['#submit'][] = 'mojeid_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for the mojeID admin settings form.
 *
 * @see mojeid_admin_settings()
 */
function mojeid_admin_settings_submit($form, &$form_state) {
  $values =& $form_state['values'];

  // Flatten AX schema required settings checkboxes.
  if (isset($values['mojeid_required']) && is_array($values['mojeid_required'])) {
    $flattened = array();
    foreach ($values['mojeid_required'] as $section) {
      $flattened += $section;
    }
    $values['mojeid_required'] = $flattened;
  }
}
