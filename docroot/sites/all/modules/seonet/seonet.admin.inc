<?php

/**
 * @file seonet.admin.inc
 *
 * Admin settings for engines.
 */

/**
 * Main settings to enable/disable engines.
 */
function seonet_admin_main() {

  $form = array();

  $form['seonet_sape'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable sape.ru support'),
    '#default_value' => variable_get('seonet_sape', SEONET_SAPE_DISABLE),
  );

  $form['seonet_trustlink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable trustlink.ru support'),
    '#default_value' => variable_get('seonet_trustlink', SEONET_TRUSTLINK_DISABLE),
  );

  $form['seonet_linkfeed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable linkfeed.ru support'),
    '#default_value' => variable_get('seonet_linkfeed', SEONET_LINKFEED_DISABLE),
  );

  $form['adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adv']['seonet_css'] = array(
    '#type' => 'textfield',
    '#title' => t("Module's CSS replacement"),
    '#default_value' => variable_get('seonet_css', ''),
    '#description' => t('Replacement for blocks system identifier. Allowed only English lowercase characters. Default: %name.', array('%name' => 'seonet')),
  );

  $form['#validate'] = array('seonet_admin_main_validate');
  return system_settings_form($form);
}

/**
 * Helper function: validates value for CSS replacement.
 */
function seonet_admin_main_validate($form, &$form_state) {

  $keys = array(
    'seonet_css',
    'seonet_sape_css',
    'seonet_trustlink_css',
    'seonet_linkfeed_css',
  );

  foreach ($keys as $key) {
    if (isset($form_state['values'][$key])) {
      $field = array('field' => $key, 'value' => trim($form_state['values'][$key]));
      break;
    }
  }

  if (!empty($field['value'])) {
    if (!preg_match('!^[a-z]+$!', $field['value'])) {
      form_set_error($field['field'], t('The identifier must contain only English lowercase letters.'));
    }
  }
}

/**
 * Settings for sape.ru engine.
 */
function seonet_admin_sape() {

  $form = array();

  $form['seonet_sape_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Sape ID'),
    '#default_value' => variable_get('seonet_sape_id', SEONET_DEFAULT_USERCODE),
    '#description' => t('Sape ID (<a href="@url" target="_blank">sign up</a>)', array('@url' => 'http://www.sape.ru/r.eb8b2671fe.php')),
    '#required' => TRUE,
  );

  $form['seonet_sape_multi'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable multisite'),
    '#default_value' => variable_get('seonet_sape_multi', SEONET_SAPE_MULTI_ENABLE),
  );

  $opts = drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  $form['seonet_sape_blocks'] = array(
    '#type' => 'select',
    '#title' => t('Number of sape blocks'),
    '#description' => t('Configure number of links in block at <a href="@url" target="_blank">block admin</a>', array('@url' => url('admin/build/block'))),
    '#default_value' => variable_get('seonet_sape_blocks', SEONET_DEFAULT_BLOCKS_COUNT),
    '#options' => $opts,
  );

  $form['seonet_sape_gip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable hypercontext for node'),
    '#default_value' => variable_get('seonet_sape_gip', SEONET_SAPE_GIP_DISABLE),
  );

  $form['seonet_sape_force_show_code'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force show code'),
    '#default_value' => variable_get('seonet_sape_force_show_code', SEONET_SAPE_FORCE_SHOW_CODE_ENABLE),
  );

  $form['adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adv']['seonet_sape_css'] = array(
    '#type' => 'textfield',
    '#title' => t("Engine's CSS replacement"),
    '#default_value' => variable_get('seonet_sape_css', ''),
    '#description' => t('Replacement for blocks system identifier. Allowed only English lowercase characters. Default: %name.', array('%name' => '0')),
  );

  $opts = array(
    '' => t('Default'),
    'file_get_contents' => 'file_get_contents',
    'curl' => 'curl',
    'socket' => 'socket',
  );

  $form['adv']['seonet_sape_fetch'] = array(
    '#type' => 'select',
    '#title' => t('Remote fetch type'),
    '#default_value' => variable_get('seonet_sape_fetch', ''),
    '#options' => $opts,
  );

  $form['adv']['seonet_sape_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#default_value' => variable_get('seonet_sape_host', ''),
    '#description' => t('Leave blank for auto-detection. Detected: @hostname', array('@hostname' => $_SERVER['HTTP_HOST'])),
  );

  $form['adv']['seonet_sape_cache_clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear page cache'),
    '#description' => t('Clear cached pages when new links received.'),
    '#default_value' => variable_get('seonet_sape_cache_clear', SEONET_SAPE_CACHE_CLEAR_DISABLE),
  );

  // Cache system detection.
  $cache_systems = array('none' => t('None'));
  if (function_exists('eaccelerator_get')) {
    $cache_systems['eaccelerator'] = 'eaccelerator';
  }
  if (function_exists('apc_fetch')) {
    $cache_systems['apc'] = 'apc';
  }
  if (function_exists('xcache_get')) {
    $cache_systems['xcache'] = 'xcache';
  }
  if (class_exists('Memcache')) {
    $cache_systems['memcache'] = 'memcache';
  }

  $form['adv']['seonet_sape_cache'] = array(
    '#type' => 'select',
    '#title' => t('Cache engine'),
    '#default_value' => variable_get('seonet_sape_cache', SEONET_SAPE_CACHE_ENGINE_NONE),
    '#options' => $cache_systems,
  );

  $form['adv']['seonet_sape_mc'] = array(
    '#type' => 'textfield',
    '#title' => t('Memcached server'),
    '#description' => t('Should be localhost:11211 or unix socket unix://socket.sock'),
    '#default_value' => variable_get('seonet_sape_mc', ''),
  );

  if (variable_get('seonet_sape_id', SEONET_DEFAULT_USERCODE) !== SEONET_DEFAULT_USERCODE) {
    $sape = _seonet_sape();
    $items = array();
    if ($sape->_error) {
      $items[] = 'Sape: ' . $sape->_error;
    }
    else {
      $items[] = 'Sape: ok';
    }

    $sape = _seonet_sape(TRUE);
    if ($sape->_error) {
      $items[] = 'Sape context: ' . $sape->_error;
    }
    else {
      $items[] = 'Sape context: ok';
    }

    $form['example'] = array(
      '#type' => 'fieldset',
      '#title' => t('Debug information'),
    );

    $form['example']['debug'] = array(
      '#markup' => theme('item_list', array('items' => $items)),
    );
  }

  $form['#validate'] = array('seonet_admin_main_validate');
  return system_settings_form($form);
}

/**
 * Settings for trustlink.ru engine.
 */
function seonet_admin_trustlink() {
  $form = array();

  $form['seonet_trustlink_id'] = array(
    '#type' => 'textfield',
    '#title' => t('TrustLink ID'),
    '#default_value' => variable_get('seonet_trustlink_id', SEONET_DEFAULT_USERCODE),
    '#description' => t('TrustLink ID (<a href="@url" target="_blank">sign up</a>)', array('@url' => 'http://trustlink.ru/registration/102726')),
    '#required' => TRUE,
  );

  $form['seonet_trustlink_orientation'] = array(
    '#type' => 'select',
    '#title' => t('Choose block direction'),
    '#default_value' => variable_get('seonet_trustlink_orientation', SEONET_TRUSTLINK_ORIENTATION_VERTICAL),
    '#options' => array(
      'vertical' => t('Vertical'),
      'horizontal' => t('Horizontal'),
    ),
  );

  $form['seonet_trustlink_multi'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable multisite'),
    '#default_value' => variable_get('seonet_trustlink_multi', SEONET_TRUSTLINK_MULTI_ENABLE),
  );

  $form['adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adv']['seonet_trustlink_css'] = array(
    '#type' => 'textfield',
    '#title' => t("Engine's CSS replacement"),
    '#default_value' => variable_get('seonet_trustlink_css', ''),
    '#description' => t('Replacement for blocks system identifier. Allowed only English lowercase characters. Default: %name.', array('%name' => 'trustlink')),
  );

  $opts = array(
    '' => t('Default'),
    'file_get_contents' => 'file_get_contents',
    'curl' => 'curl',
    'socket' => 'socket',
  );

  $form['adv']['seonet_trustlink_fetch'] = array(
    '#type' => 'select',
    '#title' => t('Remote fetch type'),
    '#default_value' => variable_get('seonet_trustlink_fetch', ''),
    '#options' => $opts,
  );

  $form['adv']['seonet_trustlink_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#default_value' => variable_get('seonet_trustlink_host', ''),
    '#description' => t('Leave blank for auto-detection. Detected: @hostname', array('@hostname' => $_SERVER['HTTP_HOST'])),
  );

  $form['adv']['seonet_trustlink_cache_clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear page cache'),
    '#description' => t('Clear cached pages when new links received.'),
    '#default_value' => variable_get('seonet_trustlink_cache_clear', SEONET_TRUSTLINK_CACHE_CLEAR_DISABLE),
  );

  $form['adv']['seonet_trustlink_verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose mode'),
    '#default_value' => variable_get('seonet_trustlink_verbose', SEONET_TRUSTLINK_VERBOSE_DISABLE),
  );

  // Add debug information about connection with Trustlink service.
  if (variable_get('seonet_trustlink_id', SEONET_DEFAULT_USERCODE) != SEONET_DEFAULT_USERCODE) {
    $tl = _seonet_trustlink();
    $items = array();
    if ($tl->tl_error) {
      $items[] = 'TrustLink: ' . str_replace(array('<!--', '-->'), array('<span class="error">', '</span>'), $tl->tl_error);
    }
    else {
      $items[] = 'TrustLink: ok';
    }

    $form['example'] = array(
      '#type' => 'fieldset',
      '#title' => t('Debug information'),
    );

    $form['example']['debug'] = array(
      '#markup' => theme('item_list', array('items' => $items)),
    );
  }

  // Add form's validate function.
  $form['#validate'][] = 'seonet_admin_main_validate';

  return system_settings_form($form);
}

/**
 * Settings for linkfeed.ru engine.
 */
function seonet_admin_linkfeed() {
  $form = array();

  $form['seonet_linkfeed_id'] = array(
    '#type' => 'textfield',
    '#title' => t('LinkFeed ID'),
    '#default_value' => variable_get('seonet_linkfeed_id', SEONET_DEFAULT_USERCODE),
    '#description' => t('LinkFeed ID (<a href="@url" target="_blank">sign up</a>)', array('@url' => 'http://www.linkfeed.ru/reg/9351')),
    '#required' => TRUE,
  );

  $form['seonet_linkfeed_multi'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable multisite'),
    '#default_value' => variable_get('seonet_linkfeed_multi', SEONET_LINKFEED_MULTI_ENABLE),
  );

  $opts = drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
  $form['seonet_linkfeed_block'] = array(
    '#type' => 'select',
    '#title' => t('Number of links in the block'),
    '#options' => $opts,
    '#default_value' => variable_get('seonet_linkfeed_block', SEONET_DEFAULT_BLOCKS_COUNT),
  );

  $form['adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['adv']['seonet_linkfeed_css'] = array(
    '#type' => 'textfield',
    '#title' => t("Engine's CSS replacement"),
    '#default_value' => variable_get('seonet_linkfeed_css', ''),
    '#description' => t('Replacement for blocks system identifier. Allowed only English lowercase characters. Default: %name.', array('%name' => 'linkfeed')),
  );

  $form['#validate'] = array('seonet_admin_main_validate');

  $opts = array(
    '' => t('Default'),
    'file_get_contents' => 'file_get_contents',
    'curl' => 'curl',
    'socket' => 'socket',
  );

  $form['adv']['seonet_linkfeed_fetch'] = array(
    '#type' => 'select',
    '#title' => t('Remote fetch type'),
    '#default_value' => variable_get('seonet_linkfeed_fetch', ''),
    '#options' => $opts,
  );

  $form['adv']['seonet_linkfeed_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#default_value' => variable_get('seonet_linkfeed_host', ''),
    '#description' => t('Leave blank for auto-detection. Detected: @hostname', array('@hostname' => $_SERVER['HTTP_HOST'])),
  );

  $form['adv']['seonet_linkfeed_cache_clear'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear page cache'),
    '#description' => t('Clear cached pages when new links received.'),
    '#default_value' => variable_get('seonet_linkfeed_cache_clear', SEONET_LINKFEED_CACHE_CLEAR_DISABLE),
  );

  $form['adv']['seonet_linkfeed_verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose mode'),
    '#default_value' => variable_get('seonet_linkfeed_verbose', SEONET_LINKFEED_VERBOSE_DISABLE),
  );

  if (variable_get('seonet_linkfeed_id', SEONET_DEFAULT_USERCODE) !== SEONET_DEFAULT_USERCODE) {
    $lc = _seonet_linkfeed();
    $items = array();
    if ($lc->lc_error) {
      $items[] = 'LinkFeed: ' . str_replace(
        array('<!--', '-->'),
        array('<span class="error">', '</span>'),
        $lc->lc_error
      );
    }
    else {
      $items[] = 'LinkFeed: ok';
    }

    $form['example'] = array(
      '#type' => 'fieldset',
      '#title' => t('Debug information'),
    );

    $form['example']['debug'] = array(
      '#markup' => theme('item_list', array('items' => $items)),
    );
  }

  return system_settings_form($form);
}
