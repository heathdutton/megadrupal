<?php

/**
 * @file
 * A Ctools Export UI plugin for DART Tags.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'dart_tags',
  'access' => 'administer DART tags',
  'create access' => 'administer DART tags',
  'delete access' => 'administer DART tags',

  // Define the menu item.
  'menu' => array(
    'menu item' => 'dart_tags',
    'menu prefix' => 'admin/structure',
    'menu title' => 'DART Tags',
    'menu description' => 'Administer DART tags.',
  ),
  // Define user interface texts.
  'title singular' => t('tag'),
  'title plural' => t('tags'),
  'title singular proper' => t('DART tag'),
  'title plural proper' => t('DART tags'),
  // Define the class to use as a handler for DART tags.
  'handler' => array(
    'class' => 'dart_tag_ui',
    'parent' => 'ctools_export_ui',
  ),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'dart_tag_form',
    'validate' => 'dart_tag_form_validate',
  ),
);

/**
 * Form builder; Create an ad tag.
 */
function dart_tag_form(&$form, &$form_state) {
  $tag = $form_state['item'];

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Tag settings.
  $form['tag_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tag Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
  );
  $form['tag_settings']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A human readable name for this ad tag.'),
    '#required' => TRUE,
    '#maxlength' => 128,
    '#default_value' => $tag->name,
  );
  $form['tag_settings']['machinename'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine Name'),
    '#maxlength' => 32,
    '#description' => t('Only use letters, numbers and underscores. Example: top-banner'),
    '#machine_name' => array(
      'exists' => 'dart_tag_name_exists',
      'source' => array('tag_settings', 'name'),
    ),
  ) + $form['info']['machinename'];
  unset($form['info']);
  $form['tag_settings']['pos'] = array(
    '#type' => 'textfield',
    '#title' => t('Position (pos)'),
    '#description' => t('Example: rt_rail_box1'),
    '#required' => TRUE,
    '#maxlength' => 32,
    '#default_value' => $tag->pos,
  );
  $form['tag_settings']['sz'] = array(
    '#type' => 'textfield',
    '#title' => t('Size (sz)'),
    '#description' => t('Example: 300x600,300x250'),
    '#required' => TRUE,
    '#maxlength' => 64,
    '#default_value' => $tag->sz,
  );

  // Tag settings overrides.
  $form['overrides'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global DART Setting Overrides'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Override the !settings for this ad tag.', array('!settings' => l(t('global dart settings'), '/admin/configure/dart'))),
    '#group' => 'settings',
  );
  $form['overrides']['site'] = array(
    '#type' => 'textfield',
    '#title' => t('Site'),
    '#description' => t('In rare circumstances you may want to override the <a href="!settings">default DART site</a>: %default', array('!settings' => '/admin/configure/dart', '%default' => variable_get('dart_global_site', t('No default value has been set.')))),
    '#required' => FALSE,
    '#default_value' => $tag->settings['overrides']['site'],
  );
  $form['overrides']['zone'] = array(
    '#type' => 'textfield',
    '#title' => t('Zone'),
    '#description' => t('In rare circumstances you may want to override the <a href="!settings">default DART zone</a>: %default', array('!settings' => '/admin/configure/dart', '%default' => variable_get('dart_global_zone', t('No default value has been set.')))),
    '#required' => FALSE,
    '#default_value' => $tag->settings['overrides']['zone'],
  );
  $form['overrides']['slug'] = array(
    '#type' => 'textfield',
    '#title' => t('Slug'),
    '#description' => t('Override the default slug for this ad tag. Use !none for no slug. Leave this field empty to use the default slug. Example: Advertisement', array('!none' => check_plain('<none>'))),
    '#required' => FALSE,
    '#default_value' => $tag->settings['overrides']['slug'],
  );

  // Display options.
  $form['display_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
  );
  $form['display_options']['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create a block for this tag'),
    '#description' => t('Create a block (configurable at admin/build/block) in which this ad tag is displayed.'),
    '#default_value' => $tag->block,
  );
  $form['display_options']['scriptless'] = array(
    '#type' => 'checkbox',
    '#title' => t('Render this tag without javascript'),
    '#description' => t('Use this option for DART tags included in emails.'),
    '#default_value' => $tag->settings['options']['scriptless'],
  );
  $form['display_options']['method'] = array(
    '#type' => 'radios',
    '#title' => t('Output Method'),
    '#description' => t('If you are unsure, use "adj" and/or contact doubleclick.'),
    '#options' => array(
      'adj' => t('&lt;script&gt; (adj)'),
      'adi' => t('&lt;iframe&gt; (adi)'),
    ),
    '#default_value' => !empty($tag->settings['options']['method']) ? $tag->settings['options']['method'] : 'adj',
    '#weight' => -1,
  );

  // Key/val pairs.
  $form['key_vals_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('Key|Value Pairs'),
    '#attributes' => array('#id' => 'key_vals'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'settings',
  );
  $existing_key_vals =  _dart_key_vals_get_existing($form_state, isset($tag->settings['key_vals']) ? $tag->settings['key_vals'] : array());
  _dart_key_vals_form($form['key_vals_group'], $existing_key_vals);
  $form['key_vals_group']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('dart_tag', 'node', 'menu', 'term', 'user'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
    '#dialog' => TRUE,
  );
}

/**
 * Form validation for the dart_tag form.
 */
function dart_tag_form_validate($form, &$form_state) {
  _dart_key_val_form_validate($form, $form_state);
}

/**
 * Check if the given machinename is unique in the dart_tags table.
 */
function dart_tag_name_exists($machinename) {
  $select = db_select('dart_tags', 'dt');
  $select->addExpression('COUNT(*)');
  $select->condition('dt.machinename', $machinename);

  return $select->execute()->fetchField();
}

/**
 * Ajax callback for adding key vals to the key vals form.
 */
function dart_more_key_vals_js($form, $form_state) {
  switch ($form['#form_id']) {
    case 'dart_admin_settings':
      return $form['global_key_vals']['dart_kvs']['key_vals'];
    case 'ctools_export_ui_edit_item_form':
      return $form['key_vals_group']['key_vals'];
  }
}

/**
 * Form submit functions for adding more key val forms.
 */
function dart_more_key_vals_submit($form, &$form_state) {
  $form_state['key_vals'] = $form_state['input']['key_vals'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Form builder; Create the key|val pairs subforms.
 */
function _dart_key_vals_form(&$form, $existing_key_vals = array()) {
  // Display settings.
  $form['key_vals'] = array(
    '#type' => 'markup',
    '#tree' => FALSE,
    '#prefix' => '<div id="dart-key-vals-wrapper">',
    '#suffix' => '</div>',
    '#theme' => 'dart_key_vals_form',
    // '#element_validate' => array('dfp_targeting_form_validate'),
  );

  // Add existing targets to the form unless they are empty.
  foreach ($existing_key_vals as $key => $data) {
    _dart_key_val_form($form['key_vals'], $key, $data);
  }
  // Add one blank set of target fields.
  _dart_key_val_form($form['key_vals'], count($existing_key_vals));

  $form['dart_more_key_vals'] = array(
    '#type' => 'submit',
    '#value' => t('Add another key|val pair'),
    '#submit' => array('dart_more_key_vals_submit'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'dart_more_key_vals_js',
      'wrapper' => 'dart-key-vals-wrapper',
      'effect' => 'fade',
      'file' => 'plugins/export_ui/dart_ctools_export_ui.inc',
    ),
  );
  $form['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('dart_tag', 'node', 'term', 'user'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
    '#dialog' => TRUE,
  );
}

/**
 * Form builder; Create the key|value pair subform.
 */
function _dart_key_val_form(&$form, $delta, $defaults = array()) {
  $form[$delta] = array(
    '#tree' => TRUE,
  );

  $form[$delta]['key'] = array(
    '#type' => 'textfield',
    '#size' => '35',
    '#title' => t('Key'),
    '#default_value' => isset($defaults['key']) ? $defaults['key'] : '',
    '#parents' => array('key_vals', $delta, 'key'),
  );

  $form[$delta]['val'] = array(
    '#type' => 'textfield',
    '#size' => '35',
    '#title' => t('Value'),
    '#default_value' => isset($defaults['val']) ? $defaults['val'] : '',
    '#parents' => array('key_vals', $delta, 'val'),
  );
  $form[$delta]['eval'] = array(
    '#type' => 'checkbox',
    '#title' => t('Evaluate as javascript'),
    '#default_value' => isset($defaults['eval']) ? $defaults['eval'] : '',
    '#parents' => array('key_vals', $delta, 'eval'),
  );
}

/**
 * Form validation for any form that includes fields created by
 * dart_key_val_form().
 */
function _dart_key_val_form_validate($form, &$form_state) {
  // Check that each key|val pair has both a key and val.
  foreach ($form_state['values']['key_vals'] as $i => $key_vals) {
    // Remove any empty key|val pairs (even if "Evaluate as js" is checked").
    if (empty($key_vals['key']) && empty($key_vals['val'])) {
      unset($form_state['values']['key_vals'][$i]);
      continue;
    }
    if (!empty($key_vals['key']) && empty($key_vals['val'])) {
      form_set_error('key_vals][' . $i . '][val', t('If you enter a key you must also provide a val.'));
    }
    if (!empty($key_vals['val']) && empty($key_vals['key'])) {
      form_set_error('key_vals][' . $i . '][key', t('If you enter a val you must also provide a key.'));
    }
  }

  // Re index the key_vals array in case some were removed.
  $form_state['values']['key_vals'] = array_values($form_state['values']['key_vals']);
}

/**
 * Returns the current key|vals. The default value will be used unless an
 * "input" exists in the form_state variable, in which case that will be used.
 */
function _dart_key_vals_get_existing($form_state, $default = array()) {
  $key_vals = array();
  if (isset($form_state['input']['key_vals'])) {
    $key_vals = $form_state['input']['key_vals'];
  }
  elseif (!empty($default)) {
    $key_vals = $default;
  }

  return $key_vals;
}

/**
 * Theme the dart tag form for key value pairs.
 */
function theme_dart_key_vals_form($variables) {
  $form = &$variables['form'];

  $rows = array();
  $headers = array(
    t('Key'),
    t('Value'),
    t('Evaluate as javascript'),
  );

  $more_button = '';
// $more_button = drupal_render($form['dart_more_key_vals']);
// unset($form['dart_more_key_vals']);

  foreach (element_children($form) as $key) {
    // No need to print the field title every time.
    unset($form[$key]['key']['#title'], $form[$key]['val']['#title'], $form[$key]['eval']['#title']);

    // Build the table row.
    $row = array(
      'data' => array(
        array(
          'data' => drupal_render($form[$key]['key']),
          'class' => 'key_vals-key',
        ),
        array(
          'data' => drupal_render($form[$key]['val']),
          'class' => 'key_vals-val',
        ),
        array(
          'data' => drupal_render($form[$key]['eval']),
          'class' => 'key_vals-eval',
          'style' => 'text-align: center',
        ),
      ),
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  $output = theme('table', array('header' => $headers, 'rows' => $rows)) . $more_button;
  // $output .= drupal_render_children($form);

  return $output;
}
