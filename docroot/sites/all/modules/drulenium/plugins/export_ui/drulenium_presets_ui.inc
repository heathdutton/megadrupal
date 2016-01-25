<?php

/**
 * @file drulenium_ctools_export_ui.inc
 *
 * Ctools export-ui plugin for drulenium module
 */

$plugin = array(
    'schema' => 'drulenium_preset',
    'menu' => array(
        'menu prefix' => 'admin/structure/drulenium',
        'menu item' => 'presets',
        'menu title' => 'Presets',
        'menu description' => 'Add, edit or delete presets.',
    ),
    'title' => t('Drupal presets'),

    'title singular' => t('preset'),
    'title plural' => t('presets'),
    'title singular proper' => t('Preset'),
    'title plural proper' => t('Presets'),

    'handler' => array(
        'class' => 'drulenium_presets_ui',
        'parent' => 'ctools_export_ui',
    ),
);

/**
 * submit handler
*/
function drulenium_presets_ui_form_submit(&$form, &$form_state) {
  //dpm($form_state);
  $type = $form_state['values']['test_plugin_type']['plugin_type'];
  $form_state['item']->plugin_type = $type;
  $form_state['item']->drulenium_attrs = $form_state['values']['drulenium_settings_' . $type]['drulenium_attrs_' . $type];
  $form_state['item']->settings = $form_state['values']['drulenium_settings_' . $type]['settings_' . $type];
}

/**
 * The preset add/edit form
 */
function drulenium_presets_ui_form(&$form, &$form_state) {

  ctools_include('dependent');
  $export = $form_state['item'];

  $form['description'] = array(
      '#title' => t('description'),
      '#type' => 'textfield',
      '#default_value' => ! empty($export->description) ? $export->description : '',
      '#description' => t('Description for this preset.')
  );
  
  $form['category'] = array(
      '#title' => t('Category'),
      '#type' => 'textfield',
      '#default_value' => ! empty($export->category) ? $export->category : '',
      '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".')
  );

  $all_plugins = drulenium_test_plugin_load();
  $options = array();
  foreach ( $all_plugins as $plugin ) {
    $options[$plugin['name']] = $plugin['description'];
  }

  $form['test_plugin_type'] = array(
      '#type' => 'fieldset',
      '#title' => t('Drulenium plugin type'),
      '#collapsible' => TRUE,
      '#tree' => TRUE
  );

  $form['test_plugin_type']['plugin_type'] = array(
      '#title' => t('type'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => !empty($export->plugin_type) ? $export->plugin_type : 'drulenium_facepile',
      '#description' => t('Description for this preset.')
  );

  // output all form settings here but show/hide using
  // ctools 'dependent' plugin


  foreach ( $all_plugins as $type => $plugin ) {

    // fb settings
    $id = 'drulenium-settings-' . $type;
    $wrapper_id = $id . '-wrapper';
    $form['drulenium_settings_' . $type] = array(
        '#type' => 'fieldset',
        '#title' => t('%description  - settings', array(
            '%description' => $plugin['description']
        )),
        '#input' => TRUE,
        '#process' => array(
            'ctools_dependent_process'
        ),
        '#dependency' => array(
            'radio:test_plugin_type[plugin_type]' => array(
                $type
            )
        ),
        '#id' => $id,
        '#prefix' => '<div id="' . $wrapper_id . '">',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#tree' => TRUE
    );

    // Drulenium attrs
    $form['drulenium_settings_' . $type]['drulenium_attrs_' . $type] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#title' => 'Drulenium plugin attributes'
    );

    $form['drulenium_settings_' . $type]['drulenium_attrs_' . $type] += drulenium_test_plugin_fb_settings_form($type, $export->drulenium_attrs);

    // drupal settings
    $form['drulenium_settings_' . $type]['settings_' . $type] = array(
        '#type' => 'fieldset',
        '#title' => 'Test settings',
        '#collapsible' => TRUE
    );

    //$form['drulenium_settings_' . $type]['settings_' . $type] += drulenium_test_plugin_drupal_settings_form($type, $export->settings);


    $drulenium_attrs_form = drulenium_test_plugin_drupal_settings_form($type, $export->settings);

    foreach ( $drulenium_attrs_form as $k => $v ) {
      $form['drulenium_settings_' . $type]['settings_' . $type][$k] = $v;
    }

    $form['drulenium_settings_' . $type]['settings_' . $type] += _drulenium_presets_ui_create_block_form($export);
    //dsm($export);
  }
}

function _drulenium_presets_ui_create_block_form($export) {
  $form = array();
  /* $form['block'] = array(
   '#type' => 'checkbox',
      '#title' => t('Create a block'),
      '#description' => t('Create a drupal block that contains this plugin.  You have to enable the block manually to show up.'),

      '#default_value' => isset($export->settings['block']) ? $export->settings['block'] : 0
  );*/
  $form['pages'] = array(
      '#type' => 'vertical_tabs',
      '#attached' => array(
          'js' => array(drupal_get_path('module', 'block') . '/block.js'),
      ),
  );

  // Per-path pages.
  $form['pages']['path'] = array(
      '#type' => 'fieldset',
      '#title' => t('Pages'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'pages',
      '#weight' => 0,
  );
  $form['pages']['path']['custom'] = array(
      '#type' => 'textarea',
      '#title' => t('Test on specific pages'),
      '#description' => "Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are blog for the blog page and blog/* for every personal blog.",
      '#default_value' => isset($export->settings['pages']['path']['custom']) ? $export->settings['pages']['path']['custom'] : '',
  );

  // Per-node-type testing.
  $form['pages']['node'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content Types'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'pages',
      '#weight' => 20,
  );
  $node_types = array();
  $result = db_query(
      'SELECT type, name FROM {node_type}');
  foreach ($result as $row) {
    $node_types[$row->type] = $row->name;
  }
  $form['pages']['node']['custom'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Test for specific content types'),
      '#options' => $node_types,
      '#description' => t('Test on pages that display content of the given type(s).'),
      '#default_value' => isset($export->settings['pages']['node']['custom']) ? $export->settings['pages']['node']['custom'] : array(),
  );

  // Per-term-type testing.
  $form['pages']['term'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'pages',
      '#weight' => 20,
  );
  $node_types = array();
  $result = db_query(
      'SELECT vid, name FROM {taxonomy_vocabulary}');
  foreach ($result as $row) {
    $node_types[$row->vid] = $row->name;
  }
  $form['pages']['term']['custom'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Test for specific Taxonomy Vocabularies'),
      '#options' => $node_types,
      '#description' => t('Test on taxonomy term pages of the given Vocabulary type(s)'),
      '#default_value' => isset($export->settings['pages']['term']['custom']) ? $export->settings['pages']['term']['custom'] : array(),
  );

  $user_types = user_roles(FALSE);
  $form['test_as_roles'] = array(
      '#type' => 'radios',
      '#title' => t('Test as specific user roles'),
      '#options' => $user_types,
      '#description' => t('Test as users that belong to the above user role.'),
      '#default_value' => isset($export->settings['test_as_roles']) ? $export->settings['test_as_roles'] : 1,
  );
  return $form;
}

