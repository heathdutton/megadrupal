<?php

/**
 * @file
 *  Provides interface and logic to enable/disable tools from interacting with
 *  Drupal's hook system. It also manages whether tools can have dependencies
 *  between each other and external modules.
 *
 * @todo Prevent tools from being disabled when modules require them.
 * @todo Automatically flush certain caches when certain tools are enabled/disabled.
 * @todo Garbage collection when tools are disabled.
 * @todo Reset hap_hooks and hap_hooks_used variables when cache is cleared.
 * @todo Emergency reset to rebuild all tools and flush caches. (All of the above must first be implemented).
 */

/**
 * Implementation of hook_hap_info().
 */
function hap_installer_hap_info() {
  return array(
    'name' => 'Installer',
    'description' => 'This is the highly attractive installer made for highly attractive people.',
    'hooks' => array('menu', 'theme', 'modules_enabled'),
    'always enabled' => TRUE,
  );
}

/**
 * Implements hook_menu().
 */
function hap_installer_menu() {
  $items['admin/config/hap/installer'] = array(
    'title' => '★ Highly Attractive Installer',
    'description' => 'Configure which tools you want to enable to interact with hooks.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('hap_installer_form'),
    'access arguments' => array('access administration pages'),
    'file' => 'installer.inc',
    'file path' => hap_path(),
  );

  return $items;
}

/**
 * Implements hook_theme().
 */
function hap_installer_theme($existing, $type, $theme, $path) {
  return array(
    // Register Installer's table theme on behalf of installer.inc
    'hap_installer_tools' => array(
      'render element' => 'element',
      'path' => hap_path('theme'),
      'file' => 'installer.inc',
    ),
  );
}

/**
 * Implementation of hook_modules_enabled().
 */
function hap_installer_modules_enabled($modules) {
  foreach ($modules as $module) {
    $hap_list = hap_list();
    $file = drupal_get_path('module', $module) . '/' . $module . '.info';
    $info = drupal_parse_info_file($file);
    if (isset($info['hap']) && is_array($info['hap'])) {
      foreach ($info['hap'] as $tool) {
        if (isset($hap_list[$tool]) && $hap_list[$tool] == FALSE) {
          hap_turn_on($tool);
        }
      }
    }
  }
}


/******************************************************************************
 * Helper functions
 *****************************************************************************/

/**
 * Returns the machine names of each enabled tool.
 */
function hap_installer_enabled() {
  return array_keys(array_filter(hap_list()));
}


/**
 * Re-initializes 'hap_enabled' with tools that should always be enabled.
 */
function hap_installer_enabled_revert_to_default() {
  $enabled = array();
  foreach (hap_info() as $tool => $info) {
    $enabled[$tool] = $info['always enabled'];
  }
  variable_set('hap_enabled', $enabled);
  return $enabled;
}


/**
 * Enable/disables tools and runs maintenance callbacks.
 */
function hap_installer_status_set($tools, $status) {
  $list = hap_list();

  if (is_string($tools)) {
    $tools = array($tools);
  }

  foreach ($tools as $tool) {
    $list[$tool] = $status;
  }

  variable_set('hap_enabled', $list);
  hap_installer_rebuild_hooks();
  // flush caches
}



/**
 * Rebuilds hap hook information based on whichever tools are currently
 * enabled.
 */
function hap_installer_rebuild_hooks() {
  $info = hap_info();
  $hooks = $hooks_used = array();
  foreach (hap_installer_enabled() as $tool)  {
    foreach ($info[$tool]['hooks'] as $hook) {
      // Builds hook-to-tools relationship.
      $hooks[$hook][] = $tool;

      // Track all utilized hooks, prevent duplicate entries.
      $hooks_used[$hook] = $hook;
    }
  }

  variable_set('hap_hooks', $hooks);
  variable_set('hap_hooks_used', $hooks_used);
}

function _hap_installer_are_tools_enabled($tool = '', $form_state = NULL) {
  return isset($form_state['values']['tools'][$tool]) ? $form_state['values']['tools'][$tool] : FALSE;
}

function _hap_installer_are_modules_enabled($module = '', $form_state = NULL) {
  return module_exists($module);
}

/**
 * Helper function to prepare info for hap_installer_status_set().
 * @see _hap_installer_form_ajax_callback()
 */
function _hap_installer_config_change($config) {
  $current = hap_installer_enabled();
  $change = array();
  $status = FALSE;

  // Compare current and configured for extracting tools to disable.
  foreach ($current as $tool) {
    if (!$config[$tool]) {
      $change[] = $tool;
    }
  }

  // If nothing has changed we look for differences in current and config to
  // determine tools that are being enabled.
  if (empty($change)) {
    $conf = array_keys(array_filter($config));
    $change = array_diff($conf, $current);
    $status = TRUE;
  }
  return array($status, $change);
}


/******************************************************************************
 * Form handlers
 *****************************************************************************/

function hap_installer_form($form, $form_state) {
  $tools = hap_info();
  $form_state['values'] = isset($form_state['values']) ? $form_state['values'] : array('tools' => variable_get('hap_enabled', array()));
  $form['tools'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#theme' => 'hap_installer_tools',
    '#header' => array(
      array('data' => t('Enabled'), 'class' => array('checkbox')),
      t('Name'),
      t('Version'),
      t('Description'),
      array('data' => t('Operations'), 'colspan' => 2),
    ),
    '#prefix' => '<div id="hap-installer-tools">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(hap_path('css') . '/installer.css'),
    ),
  );

  foreach ($tools as $name => $info) {
    $form['tools'][$name] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($form_state['values']['tools'][$name]) ? $form_state['values']['tools'][$name] : NULL,
      '#installer_name' => $info['name'],
      '#installer_description' => isset($info['description']) ? $info['description'] : '',
      '#installer_version' => $info['version'],
      '#installer_configure' => isset($info['configure']) ? $info['configure']: '',
      '#installer_help' => isset($info['help']) ? $info['help']: '',
      '#ajax' => array(
        'callback' => '_hap_installer_form_ajax_callback',
        'wrapper' => 'hap-installer-tools',
      ),
    );

    $info['machine_name'] = $name;

    // Add additional descriptions regarding dependencies for modules and tools
    _hap_installer_process_tool_description($form['tools'][$name], $form_state, $info);

    // process checkboxes to add #disabled = TRUE where appropriate.
    _hap_installer_process_tool_status($form['tools'][$name], $form_state, $info);
  }

  return $form;
}


/**
 * Ajax callback and pseudo-submit handler.
 */
function _hap_installer_form_ajax_callback($form, &$form_state) {
  list($status, $change) = _hap_installer_config_change($form_state['values']['tools']);
  hap_installer_status_set($change, $status);
  return $form['tools'];
}


function _hap_installer_process_tool_description(&$element, &$form_state, $info) {
  $description = '';
  foreach (array('modules', 'tools') as $type) {
    $func = '_hap_installer_are_' . $type . '_enabled';
    $dependencies = array();

    foreach ($info['dependencies'][$type] as $dependency) {
      $state = ($func($dependency, $form_state)) ? 'enabled' : 'disabled';
      $dependencies[] = $dependency . ' (<span class="admin-' . $state . '">' . $state . '</span>)';
    }

    if (!empty($dependencies)) {
      $description .= '<div>' . $type . ' required: ' . implode(', ', $dependencies) . '</div>';
    }
  }
  $element['#installer_description'] .= $description;
}

/**
 * Processes a checkbox and sets its state, depending on module/tool
 * dependencies or hook_hap_info() configurations.
 */
function _hap_installer_process_tool_status(&$element, &$form_state, $info) {
  // Enable checkbox and disable interaction with it if a tool has declared
  // itself to be "always enabled".
  if (isset($info['always enabled']) && $info['always enabled']) {
    $element['#value'] = TRUE;
    $element['#disabled'] = TRUE;
  }

  else {
    // Check if other enabled tools depend on THIS tool, that way we can
    // disable interaction to this checkbox.
    if ($element['#default_value']) {
      $tools = hap_info();
      $enabled = $form_state['values']['tools'];
      foreach ($enabled as $tool => $status) {
        if (!$status) continue;
        if (in_array($info['machine_name'], $tools[$tool]['dependencies']['tools'])) {
          $element['#disabled'] = TRUE;
          break;
        }
      }
    }

    // We need to check if a tool has a dependency with other modules and prevent
    // them from getting installed if the dependency is not itself enabled.
    else {
      foreach (array('modules', 'tools') as $type) {
        $func = '_hap_installer_are_' . $type . '_enabled';
        foreach ($info['dependencies'][$type] as $member) {
          if (!$func($member, $form_state)) {
            $element['#disabled'] = TRUE;
            break 2;
          }
        }
      }
    }
  }
}
