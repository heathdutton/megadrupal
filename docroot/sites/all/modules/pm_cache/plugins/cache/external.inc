<?php

/**
 * @file
 * Provides a simple time-based caching option for panel panes.
 */

// Plugin definition
$plugin = array(
  'title' => t("External conditions"),
  'description' => t('Time-based caching based on external conditions like user role or page path.'),
  'cache get' => 'pm_cache_external_cache_get_cache',
  'cache set' => 'pm_cache_external_cache_set_cache',
  'cache clear' => 'pm_cache_external_cache_clear_cache',
  'settings form' => 'pm_cache_external_cache_settings_form',
  'defaults' => array(
    'lifetime' => 600,
    'user' => array(
      'use' => FALSE,
      'rids_include' => array(),
      'rids_exclude' => array(),
      'uids_exclude' => '',
    ),
    'og' => array(
      'use' => FALSE,
      'context' => '',
    ),
    'path' => array(
      'q' => FALSE,
      'front' => FALSE,
      'pager' => FALSE,
    ),
  ),
);

/**
 * Get cached content.
 */
function pm_cache_external_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  $cid = pm_cache_external_cache_get_id($conf, $display, $args, $contexts, $pane);
  if (!$cid) {
    return FALSE;
  }

  $cache = cache_get($cid, 'cache');
  if (!$cache) {
    return FALSE;
  }

  if ((time() - $cache->created) > $conf['lifetime']) {
    return FALSE;
  }

  return $cache->data;
}

/**
 * Set cached content.
 */
function pm_cache_external_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {
  $cid = pm_cache_external_cache_get_id($conf, $display, $args, $contexts, $pane);
  if (!$cid) {
    return FALSE;
  }

  cache_set($cid, $content);
}

/**
 * Clear cached content.
 *
 * Cache clears are always for an entire display, regardless of arguments.
 */
function pm_cache_external_cache_clear_cache($display) {
  $cid = 'pm_cache_external';

  // This is used in case this is an in-code display, which means did will be something like 'new-1'.
  if (isset($display->owner) && isset($display->owner->id)) {
    $cid .= ':' . $display->owner->id;
  }
  $cid .= ':' . $display->did;

  cache_clear_all($cid, 'cache', TRUE);
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function pm_cache_external_cache_get_id($conf, $display, $args, $contexts, $pane) {
  $id = 'pm_cache_external';

  // If the panel is stored in the database it'll have a numeric did value.
  if (is_numeric($display->did)) {
    $id .= ':' . $display->did;
  }
  // Exported panels won't have a numeric did but may have a usable cache_key.
  elseif (!empty($display->cache_key)) {
    $id .= ':' . str_replace('panel_context:', '', $display->cache_key);
  }
  // Alternatively use the css_id.
  elseif (!empty($display->css_id)) {
    $id .= ':' . $display->css_id;
  }
  // Failover to just appending the did, which may be the completely unusable
  // string 'new'.
  else {
    $id .= ':' . $display->did;
  }

  if ($pane) {
    $id .= ':' . $pane->pid;
  }

  if ($conf['path']['q']) {
    $id .= ':' . check_plain($_GET['q']);
  }
  // It only makes sense to distinguish the front page if the path aren't
  // already distinguished.
  elseif ($conf['path']['front'] && drupal_is_front_page()) {
    $id .= ':front';
  }

  // Distinguish on user data.
  if ($conf['user']['use']) {
    $rids_user = array_keys($GLOBALS['user']->roles);

    // Check for user exclusion.
    if (!empty($conf['user']['uids_exclude'])) {
      $string = ctools_context_keyword_substitute($conf['user']['uids_exclude'], array(), $contexts);
      $uids_exclude = array_map('trim', explode(',', $string));
      if (in_array($GLOBALS['user']->uid, $uids_exclude)) {
        return FALSE;
      }
    }

    // Check for role exclusion.
    $rids_exclude = array_values(array_filter($conf['user']['rids_exclude']));
    foreach ($rids_exclude as $rid) {
      if (in_array($rid, $rids_user)) {
        return FALSE;
      }
    }

    // Check for role inclusion.
    $append = '';
    $rids_include = array_values(array_filter($conf['user']['rids_include']));
    foreach ($rids_include as $rid) {
      if (in_array($rid, $rids_user)) {
        $append .= "$rid";
      }
    }
    if ($append) {
      $id .= ':rid' . $append;
    }
  }

  if (module_exists('locale')) {
    global $language;
    $id .= ':' . $language->language;
  }

  if ($conf['path']['pager']) {
    if((!empty($pane->configuration['use_pager']) && !empty($_GET['page'])) || !$pane && !empty($_GET['page'])) {
      $id .= ':page' . check_plain($_GET['page']);
    }
  }

  return $id;
}

function pm_cache_external_cache_settings_form($conf, $display, $pid) {
  $options = drupal_map_assoc(array(15, 30, 60, 120, 180, 240, 300, 600, 900, 1200, 1800, 3600, 7200, 14400, 28800, 43200, 86400, 172800, 259200, 345600, 604800), 'format_interval');

  $form['lifetime'] = array(
    '#title' => t('Lifetime'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['lifetime'],
  );

  $user_states = array(
    'visible' => array(
      ':input[name="settings[user][use]"]' => array('checked' => TRUE),
    ),
  );
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User data'),
  );
  $form['user']['use'] = array(
    '#title' => t('Use user data'),
    '#type' => 'checkbox',
    '#default_value' => $conf['user']['use'],
  );
  $form['user']['rids_include'] = array(
    '#title' => t('Roles to include'),
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#description' => t('Select what user roles the cache should distinguish between.'),
    '#default_value' => $conf['user']['rids_include'],
    '#states' => $user_states,
  );
  $form['user']['rids_exclude'] = array(
    '#title' => t('Roles to exclude'),
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#description' => t('Select what user roles that should not be cached for. This has precedence over inclusion.'),
    '#default_value' => $conf['user']['rids_exclude'],
    '#states' => $user_states,
  );
  $form['user']['uids_exclude'] = array(
    '#title' => t('User IDs to exclude'),
    '#type' => 'textfield',
    '#description' => t('Enter a comma separated list of user IDs or substitution keys representing user that should not be cached for, e.g. %ints or %keys. This has precedence over inclusion.', array('%ints' => '1, 3, 10', '%keys' => '%node:author:uid, %user:uid')),
    '#default_value' => $conf['user']['uids_exclude'],
    '#states' => $user_states,
  );

  $og_states = array(
    'visible' => array(
      ':input[name="settings[og][use]"]' => array('checked' => TRUE),
    ),
  );
  $form['og'] = array(
    '#type' => 'fieldset',
    '#title' => t('Organic Groups context'),
    '#access' => module_exists('og'),
  );
  $form['og']['use'] = array(
    '#title' => t('Use organic group context'),
    '#type' => 'checkbox',
    '#default_value' => $conf['og']['use'],
  );
  $form['og']['context'] = array(
    '#title' => t('Group context'),
    '#type' => 'textfield',
    '#description' => t('Enter a substitution key for the group context of which members to not cache for, e.g. %node.', array('%node' => '%node')),
    '#default_value' => $conf['og']['context'],
    '#states' => $og_states,
  );

  $form['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path data'),
  );
  $form['path']['q'] = array(
    '#title' => t('Per path'),
    '#type' => 'checkbox',
    '#default_value' => $conf['path']['q'],
    '#description' => t('Select if the cache should distinguish between page paths.'),
  );
  $form['path']['front'] = array(
    '#title' => t('Distinguish front page'),
    '#type' => 'checkbox',
    '#default_value' => $conf['path']['front'],
    '#description' => t('Select if the cache should distinguish the front page.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="settings[path]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['path']['pager'] = array(
    '#title' => t('Distinguish pager page'),
    '#type' => 'checkbox',
    '#default_value' => $conf['path']['pager'],
    '#description' => t('Select if the cache should make an extra exception for each pager page.'),
  );

  return $form;
}
