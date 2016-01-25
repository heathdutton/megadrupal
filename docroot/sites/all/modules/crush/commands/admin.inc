<?php

function crush_command_admin($parsed_command) {
  if (!empty($parsed_command['args'])) {
    $module = array_shift($parsed_command['args']);
    $info = system_get_info('module', $module);
    $admin_tasks = system_get_module_admin_tasks($module, $info);
    if (empty($admin_tasks)) {
      return array(t('No relevant administrative links found.'));
    }
    // Sort links by title.
    uasort($admin_tasks, 'drupal_sort_title');
    // Move 'Configure permissions' links to the bottom of each section.
    $permission_key = "admin/people/permissions#module-$module";
    if (isset($admin_tasks[$permission_key])) {
      $permission_task = $admin_tasks[$permission_key];
      unset($admin_tasks[$permission_key]);
      $admin_tasks[$permission_key] = $permission_task;
    }
    $messages[] = t('Which administrative link would you like?');
    $index = 1;
    $link_map = array();
    foreach ($admin_tasks as $link => $data) {
      $messages[] = t('@index @title (@link)', array('@index' => $index, '@title' => $data['title'], '@link' => $link));
      $link_map[$index] = $link;
      $index++;
    }
    return array('map' => $link_map, 'messages' => $messages);
  }
}

