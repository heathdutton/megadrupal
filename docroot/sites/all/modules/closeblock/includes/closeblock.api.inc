<?php

/**
 * Default theme's settings
 */
function closeblock_default_settings() {
  $defaults = array(
    'closeblock_active' => CLOSEBLOCK_ACTIVE,
    'closeblock_custom' => CLOSEBLOCK_CUSTOM,
    'closeblock_type' => CLOSEBLOCK_TYPE,
    'closeblock_speed' => CLOSEBLOCK_SPEED,
    'closeblock_save' => CLOSEBLOCK_SAVE,
    'closeblock_count' => CLOSEBLOCK_COUNT,
    'closeblock_frequency' => CLOSEBLOCK_FREQUENCY,
  );
  return $defaults;
}

/**
 * Load settings
 */
function closeblock_initialization() {
  if (user_access('closeblock')) {
    global $theme, $user;
    
    $current_theme = $theme ? $theme : variable_get('theme_default', 'garland');
    
    $params = array(
      'uid' => $user->uid,
      'theme' => $current_theme,
    );
    
    $blocks = closeblock_closing_info_get($params);
    
    drupal_static('closeblock_blocks', $blocks);
    
    //load the settings
    $current_theme = $theme ? $theme : variable_get('theme_default', 'garland');
    $theme_settings = variable_get(str_replace('/', '_', 'theme_' . $current_theme . '_settings'), array());
    $theme_settings = array_merge(closeblock_default_settings(), $theme_settings, array());
    
    drupal_static('closeblock_theme_settings', $theme_settings);
  }
}

/**
 * Write block settings to Drupal.settings.closeblock
 */
function closeblock_ajax_settings($data, $block) {
  if (user_access('closeblock') && !empty($data['content'])) {
    $theme_settings = drupal_static('closeblock_theme_settings');
    $blocks = drupal_static('closeblock_blocks');
    
    $settings = variable_get('closeblock_settings', array());
    
    $params = (array) $block;
    
    $block_id = theme('closeblock_block_id', $params);
    
    if (empty($settings[$block_id]['closeblock_active'])) {
      return;
    }
    
    if (!empty($settings[$block_id]['closeblock_custom'])) {
      $theme_settings = array_merge($theme_settings, $settings[$block_id], array());
    }
    
    $closed = !empty($blocks[$block_id]) && !empty($theme_settings['closeblock_save']);
    
    if (!empty($theme_settings['closeblock_count']) && $theme_settings['closeblock_count'] > 1) {
      if (empty($blocks[$block_id]['count']) || $blocks[$block_id]['count'] < $theme_settings['closeblock_count']) {
        if (empty($theme_settings['closeblock_frequency'])) {
          $closed = 0;
        }
        else {
          $current_time = time();
          $time = strtotime($theme_settings['closeblock_frequency']) - $current_time;
          
          if (empty($blocks[$block_id]['timestamp']) || ($blocks[$block_id]['timestamp'] + $time < $current_time)) {
            $closed = 0;
          }
        }
      }
    }
    
    $types = array(1 => '', 2 => 'slideUp', 3 => 'fadeOut');
    
    drupal_add_js(array('closeblock' => array(
      $block_id => array(
        'closed' => $closed,
        'speed' => $theme_settings['closeblock_speed'],
        'save' => $theme_settings['closeblock_save'],
        'type' => $types[$theme_settings['closeblock_type']],
        'module' => $params['module'],
        'delta' => $params['delta'],
      )
    )), array('type' => 'setting', 'scope' => JS_DEFAULT));
    
    $path = drupal_get_path('module', 'closeblock');
    
    drupal_add_js($path .'/theme/js/closeblock.js');
    drupal_add_css($path .'/theme/css/closeblock.css');
  }
}

/**
 * Set info.
 */
function closeblock_closing_info_set($params) {
  if (isset($params['uid']) && !empty($params['uid'])) {
    $info = closeblock_closing_info_get($params);
  
    $params['count'] = 1;
    $params['timestamp'] = time();
    
    if ($info) {
      $block_id = theme('closeblock_block_id', $params);
      $params['count'] = $info[$block_id]['count'] + 1;
      drupal_write_record('closeblock', $params, array('module', 'delta', 'theme', 'uid'));
    }
    else {
      drupal_write_record('closeblock', $params);
    }
  }
  else {
    closeblock_closing_info_anonymous_set($params);
  }
}

/**
 * Get info.
 */
function closeblock_closing_info_get($params) {
  if (isset($params['uid']) && empty($params['uid'])) {
    return closeblock_closing_info_anonymous_get($params);
  }
  
  if (empty($params) || !is_array($params)) {
    return;
  }
  
  $result = closeblock_db_load($params);
  
  $blocks = array();
  
  if (!empty($result)) {
    foreach ($result as $block) {
      $params = (array) $block;
      $block_id = theme('closeblock_block_id', $params);
      $blocks[$block_id] = $params;
    }
  }
  
  return $blocks;
}

/**
 * Set info for anonymous users.
 */
function closeblock_closing_info_anonymous_set($params) {
  if (!empty($params['theme']) && !empty($params['module']) && !empty($params['delta'])) {
    $block_id = theme('closeblock_block_id', $params);
    
    $count = 1;
    if (!empty($_SESSION['closeblock'][$block_id]['count'])) {
      $count += $_SESSION['closeblock'][$block_id]['count'];
    }
    
    $_SESSION['closeblock'][$block_id] = array(
      'count' => $count,
      'timestamp' => time(),
      'module' => $params['module'],
      'delta' => $params['delta'],
    );
  }
}

/**
 * Get info for anonymous users.
 */
function closeblock_closing_info_anonymous_get($params) {
  if (!empty($params['theme']) && !empty($params['module']) && !empty($params['delta'])) {
    $block_id = theme('closeblock_block_id', $params);
    
    if (!empty($_SESSION['closeblock'][$block_id])) {
      return $_SESSION['closeblock'][$block_id];
    }
  }
  else if (!empty($_SESSION['closeblock'])) {
    return $_SESSION['closeblock'];
  }
  
  return array();
}

/**
 * Ajax callback function.
 */
function closeblock_ajax_callback($module, $delta) {
  global $theme, $user;
  $current_theme = $theme ? $theme : variable_get('theme_default', 'garland');
  $params = array(
    'uid' => $user->uid,
    'theme' => $current_theme,
    'module' => $module,
    'delta' => $delta,
  );
  closeblock_closing_info_set($params);
}
