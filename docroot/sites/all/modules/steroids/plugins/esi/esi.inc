<?php

$plugin = array(
    'name' => 'ESI',
    'module' => 'steroids',
    'hook_menu' => 'steroids_esi_menu',
    'admin_settings_form' => 'steroids_esi_admin_settings_form',
    'hook_theme' => 'steroids_esi_theme',
    'hook_preprocess_block' => 'steroids_esi_preprocess_block',
);

/**
 * Delegation of hook_menu().
 */
function steroids_esi_menu(&$items) {
  $items['steroids/callback/esi/block/%'] = array(
      'page callback' => 'steroids_esi_callback_block_handler',
      'page arguments' => array(4),
      'access arguments' => array('access content'),
      'type' => MENU_CALLBACK,
      'file' => 'plugins/esi/esi.inc',
  );
}

/**
 * Steroids menu callback handler
 */
function steroids_esi_callback_block_handler($bid) {
  print _steroids_esi_get_block($bid);
}

/**
 * Admin settings form
 */
function steroids_esi_admin_settings_form(&$form_state) {
  $settings = variable_get('steroids_esi', array());

  $form['steroids_esi'] = array(
      '#type' => 'fieldset',
      '#title' => t('ESI Blocks'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      '#tree' => TRUE,
      '#theme' => 'steroids_esi_table',
  );

  $form['steroids_esi']['help'] = array(
      '#type' => 'item',
      '#markup' => t('This is a list of all blocks exposed by modules'),
  );

  foreach (module_implements('block_info') as $module) {
    $module_blocks = module_invoke($module, 'block_info');
    if ($module_blocks) {
      foreach ($module_blocks as $delta => $info) {
        $name = check_plain($info['info']);
        $bid = "{$module}_{$delta}";
        $form['steroids_esi']['blocks'][$bid] = array();
        $form['steroids_esi']['blocks'][$bid]['name'] = array(
            '#type' => 'item',
            '#markup' => $name,
        );
        $form['steroids_esi']['blocks'][$bid]['status'] = array(
            '#type' => 'checkbox',
            '#title' => t('Enabled'),
            '#default_value' => (isset($settings['blocks'][$bid]['status'])) ? $settings['blocks'][$bid]['status'] : FALSE,
        );
        $form['steroids_esi']['blocks'][$bid]['ttl'] = array(
            '#type' => 'select',
            '#options' => _steroids_esi_ttl_periods(),
            '#default_value' => (isset($settings['blocks'][$bid]['ttl'])) ? $settings['blocks'][$bid]['ttl'] : 0,
        );
        $form['steroids_esi']['blocks'][$bid]['path'] = array(
            '#type' => 'textfield',
            '#default_value' => (isset($settings['blocks'][$bid]['path'])) ? $settings['blocks'][$bid]['path'] : '',
        );
      }
    }
  }

  return $form;
}

/**
 * Delegation of hook_theme().
 */
function steroids_esi_theme($existing, $type, $theme, $path) {
  $items = array();

  $items['steroids_esi_table'] = array(
    'file' => 'esi.theme.inc',
    'path' => drupal_get_path('module', 'steroids') . '/plugins/esi',
    'render element' => 'form',
  );

  return $items;
}

/**
 * Implements hook_preprocess_block()
 *
 */
function steroids_esi_preprocess_block(&$vars) {
  $enabled_blocks = _steroids_esi_get_enabled_blocks();

  if ((in_array($vars['block']->module . '_' . $vars['block']->delta, $enabled_blocks)) && ((!isset($vars['block']->esi_processed)) || (!$vars['block']->esi_processed))) {
    $settings = _steroids_esi_get_block_settings($vars['block']->module, $vars['block']->delta);
    $vars['block']->path = $settings['path'];
    $vars['block']->ttl = $settings['ttl'];
    $vars['content'] = steroids_esi_block_wrap($vars);
  }
}

/**
 * Wrap the block content with ESI markup.
 */
function steroids_esi_block_wrap($vars) {
  global $theme_key, $base_url;

  $block = $vars['block'];

  if (empty($block->path)) {
    $bid = "{$theme_key}:{$block->region}:{$block->module}:{$block->delta}";
    $block->path = $base_url . '/steroids/callback/esi/block/'. $bid;
  }

  $output  = '';
  $output .= '<esi:include src="'. $block->path .'" onerror="continue"/>';
  $output .= '<esi:remove>';
  $output .= t('This is an ESI BLOCK placeholder.');
  $output .= '</esi:remove>';

  return $output;
}

/**
 * Get block
 */
function _steroids_esi_get_block($bid) {
  if (!substr_count($bid, ':') == 3) {
    return FALSE;
  }
  list($theme, $region, $module, $delta) = explode(':', $bid);

  drupal_theme_initialize();

  global $theme_key;
  $theme_key = $theme;

  $block = block_load($module, $delta);
  $block->context = $region;
  $block->status = 1;
  $block->esi_processed = TRUE;
  $block->subject = '';
  if (!empty($block->title)) {
    // Check plain here to allow module generated titles to keep any markup.
    $block->subject = $block->title == '<none>' ? '' : check_plain($block->title);
  }

  $config = _steroids_esi_get_block_settings($block->module, $block->delta);

  // Set cache control for this ESI block.
  drupal_add_http_header('Cache-Control', 'public, max-age=' . $config['ttl']);

  // Render block.
  $block_content = _block_render_blocks(array($block));
  $build = $block_content[$module . '_' . $delta]->content;

  return drupal_render($build);
}

/**
 * Helper function to get the block with ESI support enabled.
 */
function _steroids_esi_get_enabled_blocks($rebuild = FALSE) {
  static $enabled_blocks = array();

  if (!empty($enabled_blocks) && !$rebuild) {
    return $enabled_blocks;
  }

  $settings = variable_get('steroids_esi', array());

  if (isset($settings['blocks'])) {
    foreach ((array) $settings['blocks'] as $key => $value) {
      if ($value['status'] == 1) {
        array_push($enabled_blocks, $key);
      }
    }
  }

  return $enabled_blocks;
}

/**
 * Helper function to get the settings of a given block.
 */
function _steroids_esi_get_block_settings($module, $delta, $config = NULL) {
  $settings = variable_get('steroids_esi');

  if (isset($settings['blocks'][$module . '_' . $delta])) {
    return $settings['blocks'][$module . '_' . $delta];
  }

  return FALSE;
}

/**
 * Helper function to set the ttl periods option list.
 *
 * @return array
 *   An assoc array with periods in seconds.
 */
function _steroids_esi_ttl_periods() {
  return drupal_map_assoc(array(0, 60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 86400, 86400 * 2, 86400 * 5, 86400 * 7, 86400 * 14, 86400 * 21, 86400 * 28), 'format_interval');
}
