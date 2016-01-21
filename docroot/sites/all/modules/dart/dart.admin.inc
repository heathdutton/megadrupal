<?php

/**
 * @file
 * Admin form handling for the DART module.
 */

/**
 * Form builder; Configure dart settings for this site.
 *
 * @ingroup forms
 */
function dart_admin_settings($form, &$form_state) {
  ctools_include('plugins');
  ctools_plugin_load_class('ctools', 'export_ui', 'dart_ctools_export_ui', 'handler');

  $form = array(
    '#submit' => array('dart_admin_settings_submit'),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'dart') . '/js/dart.admin.js',
      ),
    ),
  );

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Default tag settings.
  $form['tag_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Tag Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#weight' => '-10',
  );
  $form['tag_settings']['prefix_site_zone_settings'] = dart_prefix_site_zone_form(TRUE, array(
    'prefix' => variable_get('dart_global_prefix', ''),
    'site' => variable_get('dart_global_site', ''),
    'zone' => variable_get('dart_global_zone', ''),
  ));
  $form['tag_settings']['prefix_site_zone_settings']['prefix']['#parents'] = array('dart_global_prefix');
  $form['tag_settings']['prefix_site_zone_settings']['site']['#parents'] = array('dart_global_site');
  $form['tag_settings']['prefix_site_zone_settings']['site']['#description'] = t('The value to use when no "site" is specified on a given page.');
  $form['tag_settings']['prefix_site_zone_settings']['zone']['#parents'] = array('dart_global_zone');
  $form['tag_settings']['prefix_site_zone_settings']['zone']['#description'] = t('The value to use when no "zone" is specified on a given page.');

  $form['tag_settings']['dart_load_last'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load ads last'),
    '#default_value' => variable_get('dart_load_last', '0'),
    '#description' => t('Wait until the entire page has loaded before loading ads. This will allow your pages to load faster but may cause empty spaces to appear on your pages before the ads load. <strong>You must use this option if you are loading ad tags asyncronously.</strong>'),
  );

  if (!libraries_detect('writeCapture') && !libraries_detect('postscribe')) {
    $form['tag_settings']['dart_load_last']['#disabled'] = TRUE;
    $form['tag_settings']['dart_load_last']['#default_value'] = 0;
    $form['tag_settings']['dart_load_last']['#prefix'] = '<p><strong>' .
      t('This option requires !writecapture or !postscribe to be included using the !libraries module (&gt;= 7.x-2.0). Please see README.txt for this module',
        array(
          '!postscribe' => l('postscribe', 'https://github.com/krux/postscribe'),
          '!writecapture' => l('writeCapture','https://github.com/iamnoah/writeCapture'),
          '!libraries' => l('Libraries', 'http://drupal.org/project/libraries'))) .
      '</strong></p>';
  }


  $form['tag_settings']['dart_use_network_id'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a Network ID'),
    '#default_value' => variable_get('dart_use_network_id', ''),
    '#required' => FALSE,
    '#description' => t('Only check this if you have been told by Google that you need to include an network id in your tags.'),
  );
  $form['tag_settings']['dart_global_network_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Network ID'),
    '#default_value' => variable_get('dart_global_network_id', ''),
    '#required' => FALSE,
    '#size' => 10,
    '#description' => t('When your site begins the process of migrating to the new GPT style tags, Google requires that you include a Network ID in order for your existing tags to continue working. If you have already completed your migration than you should consider using the <a href="http://drupal.org/project/dfp">DFP module</a> instead.'),
    '#states' => array(
      'visible' => array(
        'input[name="dart_use_network_id"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Global display options.
  $form['display_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Display Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#weight' => '-9',
  );
  $form['display_options']['dart_global_slug'] = array(
    '#type' => 'textfield',
    '#title' => t('Global Slug'),
    '#default_value' => variable_get('dart_global_slug', '<none>'),
    '#required' => FALSE,
    '#description' => t('Slug all ad tags with this label. Use @tag for no slug. Example: Advertisement', array('@tag' => '<none>')),
  );
  $form['display_options']['dart_noscript'] = array(
    '#type' => 'checkbox',
    '#title' => t('display @tag tags', array('@tag' => '<noscript>')),
    '#default_value' => variable_get('dart_noscript', '1'),
    '#description' => t('Display @tag tags along with the standard javascript DART tags.', array('@tag' => '<noscript>')),
  );

  // Background ad options.
  $form['bg_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background Ad Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#weight' => '-8',
  );
  $form['bg_options']['dart_enable_bg_ads'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable background ads'),
    '#default_value' => variable_get('dart_enable_bg_ads', 0),
    '#description' => t('Many sites change the background image of their entire page to an advertisement by allowing DART to serve the background image.'),
  );
  $form['bg_options']['dart_bg_variables'] = array(
     '#type' => 'markup',
     '#tree' => FALSE,
     '#prefix' => '<div id="dart-bg-varaiables-wrapper">',
     '#suffix' => '</div>',
     '#theme' => 'dart_bg_variables_form',
  );
  $form['bg_options']['dart_bg_variables']['dart_bg_var_bgimg'] = array(
    '#type' => 'textfield',
     '#title' => t('Background image'),
     '#default_value' => variable_get('dart_bg_var_bgimg', ''),
  );
  $form['bg_options']['dart_bg_variables']['dart_bg_var_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => variable_get('dart_bg_var_bgcolor', ''),
  );
  $form['bg_options']['dart_bg_variables']['dart_bg_var_bgrepeat'] = array(
    '#type' => 'textfield',
    '#title' => t('Background repeat'),
    '#default_value' => variable_get('dart_bg_var_bgrepeat', ''),
  );
  $form['bg_options']['dart_bg_variables']['dart_bg_var_clickurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Click thru'),
    '#default_value' => variable_get('dart_bg_var_clickurl', ''),
  );
  $form['bg_options']['dart_bg_var_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS selector'),
    '#default_value' => variable_get('dart_bg_var_selector', 'body'),
    '#description' => t('Enter the CSS selector to which this background image should be targeted.'),
  );

  // Taxonomy options.
  if (module_exists('taxonomy')) {
    $form['taxonomy_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy Options'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#group' => 'settings',
      '#weight' => '-7',
    );
    $form['taxonomy_options']['dart_include_taxonomy_terms'] = array(
      '#type' => 'checkbox',
      '#title' => t('Where possible include taxonomy terms in DART tags as key|val pairs'),
      '#default_value' => variable_get('dart_include_taxonomy_terms', 0),
    );
    $form['taxonomy_options']['dart_term_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Taxonomy term key'),
      '#default_value' => variable_get('dart_term_key', 'term'),
      '#description' => t('Specify what key should be used with each term value in the tag. For example "term" will result in term=termvalue;</br>Leave this field empty to use the vocabulary name of the given tag. Spaces will be removed automatically.'),
      '#states' => array(
        'visible' => array(
          'input[name="dart_include_taxonomy_terms"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['taxonomy_options']['dart_term_formatter'] = array(
      '#type' => 'select',
      '#options' => _dart_term_formatters(),
      '#title' => t('Taxonomy term formatter'),
      '#default_value' => variable_get('dart_term_formatter', '_dart_term_formatter_tid'),
      '#description' => t('How should the taxonomy terms be formatted when included in a DART tag.'),
      '#states' => array(
        'visible' => array(
          'input[name="dart_include_taxonomy_terms"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['taxonomy_options']['dart_enable_ad_categories'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable DART Ad Categories'),
      '#prefix' => '<hr />',
      '#default_value' => variable_get('dart_enable_ad_categories', 0),
      '#description' => t('Example: if you have an "animals" vocabulary and you want to target the same ads to "dogs", "cats" and "hamsters" you can edit each of those terms and set the DART Ad Category to "pets". Whenever the taxonomy terms are included as targeting values, anything tagged "cats" will target "pets" instead.'),
    );

    $bundles = field_info_bundles('taxonomy_term');
    $options = array();
    foreach ($bundles as $key => $value) {
      if ($key != 'dart_ad_categories') {
        $options[$key] = $value['label'];
      }
    }
    $form['taxonomy_options']['dart_enable_ad_categories_bundles'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#prefix' => '<div style="margin-left: 20px">',
      '#suffix' => '</div>',
      '#default_value' => variable_get('dart_enable_ad_categories_bundles', array()),
      '#description' => t('Select the vocabularies on which DART Ad Categories should be enabled.'),
      '#states' => array(
        'visible' => array(
          'input[name="dart_enable_ad_categories"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  // Gloabal key/val pairs
  $form['global_key_vals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Key|Value Pairs'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#weight' => '-7',
  );
  $form['global_key_vals']['dart_special_key_vals'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Special key|val pairs'),
    '#default_value' => variable_get('dart_special_key_vals', array('tile' => 'tile', 'ord' => 'ord')),
    '#options' => array(
      'tile' => t('tile (value auto-increments on each tag)'),
      'ord' => t('ord (value changes on each page request)'),
    ),
    '#description' => t('Include these special values in all of your ad tags.'),
    '#weight' => '-10',
  );
  $form['global_key_vals']['dart_special_tile_init'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial value of tile'),
    '#default_value' => variable_get('dart_special_tile_init', '0'),
    '#size' => '1',
    '#weight' => '-5',
    '#states' => array(
      'visible' => array(
        'input[name="dart_special_key_vals[tile]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Container for the key_value forms.
  $form['global_key_vals']['dart_kvs'] = array(
    '#tree' => FALSE,
  );
  $existing_key_vals =  _dart_key_vals_get_existing($form_state, variable_get('dart_key_vals', array()));
  _dart_key_vals_form($form['global_key_vals']['dart_kvs'], $existing_key_vals);

  // Javascript.
  $form['javascript'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inject Javascript'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
    '#weight' => '20',
  );
  $form['javascript']['dart_js'] = array(
    '#type' => 'textarea',
    '#title' => t('Inject javascript'),
    '#description' => t('Inject this javascript into the @tag on each page request. You do not need to add code to create a random number for use as the ORD if you checked "Include ord value" above. This javascript will be injected above the key|value pairs you enter in the field below, so you can declare variables here that you will include as part of a key|value pair there.', array('@tag' => '<head>')),
    '#default_value' => variable_get('dart_js', ''),
    '#rows' => 5,
  );

  // Ad Test Settngs.
  $form['default_ad_test_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ad Test Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'Enter the values that DART tags should use for prefix, site & zone when you add the ?adtest=true query string to any url. Additionally note that you can override these values at any time by adding ?adtest=true &amp;prefix=testprefix &amp;site=testsite &amp;zone=testzone to any url. <div><h3>Examples:</h3><ul><li>mysite.com?adtest=true</li><li>mysite.com?adtest=true&prefix=testprefix</li><li>mysite.com?adtest=true&prefix=testprefix&zone=testzone</li></ul></div>',
    '#group' => 'settings',
    '#weight' => '50',
  );

  $form['default_ad_test_settings']['prefix_site_zone_settings'] = dart_prefix_site_zone_form(TRUE, array(
    'prefix' => variable_get('dart_test_prefix', ''),
    'site' => variable_get('dart_test_site', ''),
    'zone' => variable_get('dart_test_zone', ''),
  ));
  $form['default_ad_test_settings']['prefix_site_zone_settings']['prefix']['#parents'] = array('dart_test_prefix');
  $form['default_ad_test_settings']['prefix_site_zone_settings']['prefix']['#description'] = t('The prefix to use when the query string ?adtest=true is present.');
  $form['default_ad_test_settings']['prefix_site_zone_settings']['site']['#parents'] = array('dart_test_site');
  $form['default_ad_test_settings']['prefix_site_zone_settings']['site']['#description'] = t('The site to use when the query string ?adtest=true is present.');
  $form['default_ad_test_settings']['prefix_site_zone_settings']['zone']['#parents'] = array('dart_test_zone');
  $form['default_ad_test_settings']['prefix_site_zone_settings']['zone']['#description'] = t('The zone to use when the query string ?adtest=true is present.');

  return system_settings_form($form);
}

/**
 * Form validation function.
 */
function dart_admin_settings_validate($form, &$form_state) {
  ctools_include('plugins');
  ctools_plugin_load_class('ctools', 'export_ui', 'dart_ctools_export_ui', 'handler');

  // Validate "key_vals" before its name is changed.
  _dart_key_val_form_validate($form, $form_state);

  // Rename the key_vals value to dart_key_vals to avoid namespace conflicts.
  $form_state['values']['dart_key_vals'] = $form_state['values']['key_vals'];
  unset($form_state['values']['key_vals']);
}

/**
 * Form submit function.
 */
function dart_admin_settings_submit($form, &$form_state) {
  // Attach (or delete) an instance of the dart_ad_categories term_reference
  // field for each vocabulary that should (or should not) have DART Ad
  // Categories enabled.
  foreach ($form_state['values']['dart_enable_ad_categories_bundles'] as $bundle => $enable) {
    $existing_instance = field_read_instance('taxonomy_term', 'field_dart_ad_categories', $bundle);
    $enable = $enable && !$existing_instance && $form_state['values']['dart_enable_ad_categories'];
    if ($enable) {
      $instance = array(
        'field_name' => 'field_dart_ad_categories',
        'entity_type' => 'taxonomy_term',
        'label' => t('DART Ad Category'),
        'bundle' => $bundle,
        'required' => FALSE,
        'widget' => array(
          'type' => 'options_select'
        ),
      );
      field_create_instance($instance);
    }
    elseif (!$enable && $existing_instance) {
      // Delete this field instance, but be certain not to delete the field.
      field_delete_instance($existing_instance, FALSE);
    }
  }
}

function dart_prefix_site_zone_form($required, $default_values = array()) {
  $form = array(
    '#theme' => 'dart_prefix_site_zone_form',
    '#description' => t('Enter the values that DART tags should use for prefix, site & zone. Example: groups.drupal.org might use gdo.group/general; drupal.org might use drupal.default (notice there is no zone);'),
  );
  $form['prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#default_value' => isset($default_values['prefix']) ? $default_values['prefix'] : '',
    '#required' => $required,
    '#size' => 10,
    '#attributes' => array('style' => 'width:auto'),
    '#description' => t('Typically this is a short version of your organization\'s name (provided by Doubleclick).'),
  );
  $form['site'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#default_value' => isset($default_values['site']) ? $default_values['site'] : '',
    '#required' => $required,
    '#size' => 20,
    '#attributes' => array('style' => 'width:auto'),
    '#description' => t('Typically this is the primary section of your site.'),
  );
  $form['zone'] = array(
    '#type' => 'textfield',
    '#title' => t('Zone'),
    '#default_value' => isset($default_values['zone']) ? $default_values['zone'] : '',
    '#required' => $required,
    '#size' => 20,
    '#attributes' => array('style' => 'width:auto'),
    '#description' => t('Typically this is hte subsection of your site.'),
  );
  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('dart_tag', 'node', 'menu', 'term', 'user'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
    '#dialog' => TRUE,
    '#weight' => 100,
  );

  return $form;
}

/**
 * Theme the prefix.site/zone form.
 */
function theme_dart_prefix_site_zone_form($variables) {
  $form = &$variables['form'];

  // Store field descriptions so they can be displayed later.
  $descriptions = array();
  foreach (element_children($form) as $child) {
    $field = &$form[$child];
    if (!empty($field['#description'])) {
      $descriptions[] = $field['#title'] . ": " . $field['#description'];
      unset($field['#description']);
    }
  }

  // Remove titles.
  unset($form['prefix']['#title'], $form['site']['#title'], $form['zone']['#title']);

  $replacements = array(
    '!prefix' => drupal_render($form['prefix']),
    '!site' => drupal_render($form['site']),
    '!zone' => drupal_render($form['zone']),
    '!tokens' => drupal_render($form['tokens']),
  );

  $form['dart_prefix_site_zone'] = array(
    '#prefix' => '<div class="form-item"><label>prefix . site / zone</label>',
    '#markup' => '<div class="container-inline">' . t('!prefix . !site / !zone !tokens', $replacements) . '</div>',
    "#suffix" => '<div class="description">' . $form['#description'] . theme('item_list', array('items' => $descriptions)) . '</div></div>',
  );

  return drupal_render($form['dart_prefix_site_zone']);
}

/**
 * Theme the prefix.site/zone form.
 */
function theme_dart_bg_variables_form($variables) {
  $form = &$variables['form'];
  $rows = array();
  $headers = array(
    t('Attribute'),
    t('Variable name'),
  );

  $caption = t('Use this table to specify the variables that DART will serve when there is a background ad running.');

  foreach (element_children($form) as $key) {
    $title = $form[$key]['#title'];
    unset($form[$key]['#title']);
    $rendered_field = drupal_render($form[$key]);

    $rows[] = array(
      'data' => array($title, $rendered_field),
   );
  }

  return theme('table', array('header' => $headers, 'rows' => $rows, 'caption' => $caption));
}