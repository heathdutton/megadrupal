<?php

/**
 * @file
 * TODO.
 */

/* Global Admin */

/**
 * Form builder.
 */
function gcc_contextual_block_config($form, $form_state) {

  $form = array();

  $form['basic'] = array(

    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Basic configuration'),
  );

  $themes = list_themes();
  $options = array();
  foreach ($themes as $key => $theme) {
    if ($theme->status) {
      $options[$key] = $theme->info['name'];
    }
  }

  $config = variable_get('gcc_contextual_block_config', array());

  $form['config'] = array('#tree' => TRUE);

  foreach (variable_get('gcc_contextual_block_theme', array()) as $theme) {

    $blocks = _block_rehash($theme);
    $regions = system_region_list($theme, REGIONS_VISIBLE);
    $regions[GCC_CONTEXTUAL_BLOCK_HIDDEN] = t('Hidden');

    $form['config'][$theme] = array(

      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => $themes[$theme]->info['name'],
      '#regions' => $regions,
      '#blocks' => $blocks,
      '#pre_render' => array('gcc_contextual_block_table'),
    );

    foreach ($blocks as $block) {

      $form['config'][$theme][$block['bid']]['config_customize'] = array(

        '#type' => 'checkbox',
        '#title' => '',
        '#default_value' => isset($config[$theme][$block['bid']]['config_customize']) ? $config[$theme][$block['bid']]['config_customize'] : 0,
      );

      $form['config'][$theme][$block['bid']]['region_customize'] = array(

        '#type' => 'checkbox',
        '#title' => '',
        '#default_value' => isset($config[$theme][$block['bid']]['region_customize']) ? $config[$theme][$block['bid']]['region_customize'] : 0,
      );

      $form['config'][$theme][$block['bid']]['regions'] = array(

        '#type' => 'checkboxes',
        '#title' => '',
        '#options' => $regions,
        '#default_value' => isset($config[$theme][$block['bid']]['regions']) ? $config[$theme][$block['bid']]['regions'] : array(),
      );
    }
  }

  $form['basic']['gcc_contextual_block_theme'] = array(

    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('gcc_contextual_block_theme', array()),
    '#title' => t('Themes'),
    '#description' => t('Choose for which theme you want to allow to customize the block disposition and configuration'),
  );

  $form['actions'] = array('#type' => 'actions');

  $form['actions']['submit'] = array(

    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Theme callback.
 */
function gcc_contextual_block_table($element) {

  $regions = $element['#regions'];
  $blocks = $element['#blocks'];

  $header = array(
    t('Block Name'),
    t('Allow configuration customization'),
    t('Allow region customization'),
  );

  foreach ($regions as $region) {
    $header[] = $region;
  }

  $element['table'] = array(

    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array(),
  );

  $rows = array();
  foreach ($blocks as $block) {

    $row = array();
    $row[] = $block['info'];
    $row[] = array('data' => $element[$block['bid']]['config_customize']);
    $row[] = array('data' => $element[$block['bid']]['region_customize']);

    foreach ($regions as $key => $name) {

      $element[$block['bid']]['regions'][$key]['#title'] = '';
      $row[] = array('data' => $element[$block['bid']]['regions'][$key]);
    }

    $rows[] = $row;

    unset($element[$block['bid']]);
  }

  $element['table']['#rows'] = $rows;

  return $element;
}

/**
 * Submit handler.
 */
function gcc_contextual_block_config_submit($form, &$form_state) {

  $form_state['values']['gcc_contextual_block_theme'] = array_filter($form_state['values']['gcc_contextual_block_theme']);
  variable_set('gcc_contextual_block_theme', array_filter($form_state['values']['gcc_contextual_block_theme']));

  $form_state['values'] += array('config' => array());
  $form_state['values']['config'] = array_intersect_key($form_state['values']['config'], $form_state['values']['gcc_contextual_block_theme']);
  variable_set('gcc_contextual_block_config', $form_state['values']['config']);

  drupal_set_message(t('The configuration options have been saved.'));
}

/* Group Admin */

/**
 * Administration form for the block layout of a group.
 */
function gcc_contextual_block_layout_config($form, &$forms_state, $entity_type, $entity, $base_path) {

  $form = array('#tree' => TRUE);

  $form['#entity_type'] = $entity_type;
  $form['#entity'] = $entity;

  $theme = variable_get('theme_default', 'bartik');

  if (module_exists('gcc_theme') && isset($entity->{GCC_THEME_FIELD_THEME}[LANGUAGE_NONE][0]['value'])) {
    $theme = $entity->{GCC_THEME_FIELD_THEME}[LANGUAGE_NONE][0]['value'];
  }

  // Fetching the current config.
  $current_config = array();
  if (isset($entity->{GCC_CONTEXTUAL_BLOCK_FIELD}[LANGUAGE_NONE][0]['config'])) {
    $current_config = $entity->{GCC_CONTEXTUAL_BLOCK_FIELD}[LANGUAGE_NONE][0]['config'];
  }

  // Fetching the block data.
  $blocks_info = array();
  foreach (module_implements('block_info') as $module) {

    $module_blocks = module_invoke($module, 'block_info');
    foreach ($module_blocks as $delta => $block) {
      $blocks_info[$module][$delta] = $block;
    }
  }

  $blocks_bid = array();
  $list = db_select('block', 'b')
    ->fields('b')
    ->condition('b.theme', $theme)
    ->execute()
    ->fetchAllAssoc('bid', PDO::FETCH_ASSOC);

  foreach ($list as $bid => $block) {

    $blocks_bid[$bid] = $block;
    if (isset($blocks_info[$block['module']][$block['delta']]['info'])) {
      $blocks_bid[$bid]['info'] = $blocks_info[$block['module']][$block['delta']]['info'];
    }
    else {
      $blocks_bid[$bid]['info'] = $block['module'] . ' - ' . $block['delta'];
    }
  }

  // Fetch the config data.
  $global_config = variable_get('gcc_contextual_block_config', array());
  foreach ($global_config as $key => $blocks) {

    $global_config[$key] = array();
    foreach ($blocks as $bid => $block) {
      if ($block['config_customize'] || $block['region_customize']) {

        $block['regions'] = array_filter($block['regions']);
        $global_config[$key][$bid] = $block;
      }
    }
  }

  if (!isset($global_config[$theme]) || empty($global_config[$theme])) {

    return array(
      'info' => array(
        '#markup' => t('The website administrator did not authorized you to customize your block layout'),
      ),
    );
  }

  foreach ($global_config[$theme] as $bid => $block_config) {

    $form['blocks'][$bid] = array(

      '#type' => 'fieldset',
      '#title' => $blocks_bid[$bid]['info'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    if ($block_config['region_customize']) {

      $options = array(
        GCC_CONTEXTUAL_BLOCK_DISABLE => t('Use the default configuration'),
      );
      $options += $block_config['regions'];

      if (isset($options[GCC_CONTEXTUAL_BLOCK_HIDDEN])) {
        $options[GCC_CONTEXTUAL_BLOCK_HIDDEN] = t('Hidden');
      }

      $form['blocks'][$bid]['region'] = array(

        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Custom Region'),
        '#default_value' => isset($current_config[$bid]['region']) ? $current_config[$bid]['region'] : '',
      );

      $form['blocks'][$bid]['weight'] = array(

        '#type' => 'weight',
        '#delta' => 50,
        '#title' => t('Weight'),
        '#default_value' => isset($current_config[$bid]['weight']) ? $current_config[$bid]['weight'] : 0,
      );
    }

    if ($block_config['config_customize']) {

      $form['blocks'][$bid]['title'] = array(

        '#type' => 'textfield',
        '#title' => t('Title'),
        '#default_value' => isset($current_config[$bid]['title']) ? $current_config[$bid]['title'] : '',
        '#description' => t('<none>, A custom title or empty to use the default title'),
      );
    }
  }

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler.
 */
function gcc_contextual_block_layout_config_submit($form, &$form_state) {

  $entity = $form['#entity'];

  $entity->{GCC_CONTEXTUAL_BLOCK_FIELD}[LANGUAGE_NONE][0]['config'] = $form_state['values']['blocks'];

  field_attach_presave($form['#entity_type'], $entity);
  field_attach_update($form['#entity_type'], $entity);

  drupal_set_message(t('Configuration saved.'));
}
