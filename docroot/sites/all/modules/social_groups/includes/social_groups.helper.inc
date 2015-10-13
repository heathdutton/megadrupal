<?php

/**
 * Helper functions. Get a group.
 * 
 * @param string $id
 * @param array $group_info
 * @return object 
 */
function social_groups_get_instance($id, $group_info) {
  $instances = &drupal_static(__FUNCTION__);
 
  if (!isset($instances[$id])) {
    ctools_include('plugins');
    $plugin = ctools_get_plugins('social_groups', 'plugins', $id);
    $class = ctools_plugin_get_class($plugin, 'handler');
    $instances[$id] = new $class($group_info);
 
    // Check that plugin class has ingerited proper 'SocialGroupPlugin' class.
    if (!is_subclass_of($instances[$id], 'SocialGroupPlugin')) {
      $instances[$id] = NULL;
    }
  }
 
  return $instances[$id];
}

/**
 * Validate callback.
 */
function social_groups_group_form_validate($form, $form_state) {
  // Load all plugins type "plugins".
  ctools_include('plugins');
  $plugins = ctools_get_plugins('social_groups', 'plugins');

  foreach ($plugins as $id => $plugin) {
    if (isset($form_state['values'][$id])) {
      if ($group = social_groups_get_instance($id, $plugin['group'])) {
        $result = $group->setGroup($form_state['values'][$id]);
        if ($result) {
          form_set_error($id . '][' . $result['field'], $result['message']);
        }
      }
    }
  }
  
}