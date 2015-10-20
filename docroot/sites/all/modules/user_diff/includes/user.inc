<?php

/**
 * @file
 * Implementation of hook_user_diff() for user.module
 */

/**
 * Implementation of hook_user_diff() for user.module
 */
function user_user_diff($old_user, $new_user) {
  $result = array();
  $result['name'] = array(
    '#name' => t('User Name'),
    '#old' => array($old_user->name),
    '#new' => array($new_user->name),
    '#weight' => -5,
  );

  $result['email'] = array(
    '#name' => t('User Email'),
    '#old' => array($old_user->mail),
    '#new' => array($new_user->mail),
    '#weight' => -5,
  );

  $result['signature'] = array(
    '#name' => t('Signature'),
    '#old' => array($old_user->signature),
    '#new' => array($new_user->signature),
    '#weight' => -5,
  );

  $result['timezone'] = array(
    '#name' => t('Time Zone'),
    '#old' => array($old_user->timezone),
    '#new' => array($new_user->timezone),
    '#weight' => -5,
  );

  $result['language'] = array(
    '#name' => t('Language'),
    '#old' => array($old_user->language),
    '#new' => array($new_user->language),
    '#weight' => -5,
  );

  $result['theme'] = array(
    '#name' => t('Theme'),
    '#old' => array($old_user->theme),
    '#new' => array($new_user->theme),
    '#weight' => -5,
  );
  $old_user->picture = isset($old_user->picture) ? $old_user->picture : new stdClass();
  $old_user->picture->filename = isset($old_user->picture->filename) ? $old_user->picture->filename : NULL;
  $new_user->picture = isset($new_user->picture) ? $new_user->picture : new stdClass();
  $new_user->picture->filename = isset($new_user->picture->filename) ? $new_user->picture->filename : NULL;
  $result['picture'] = array(
    '#name' => t('Picture'),
    '#old' => array($old_user->picture->filename),
    '#new' => array($new_user->picture->filename),
    '#weight' => -5,
  );

  $result['status'] = array(
    '#name' => t('Status'),
    '#old' => array(($old_user->status) ? t('Active') : t('Blocked')),
    '#new' => array(($new_user->status) ? t('Active') : t('Blocked')),
    '#weight' => -5,
  );
  if (variable_get('user_diff_show_editors_ip', FALSE)) {
    $result['ip'] = array(
      '#name' => t('Editor\'s IP'),
      '#old' => array($old_user->ip),
      '#new' => array($new_user->ip),
      '#weight' => -5,
    );
  }
  // @TODO: abstract this to work with all field types and/or split this
  // integration out to be more generic.
  $instances = field_info_instances('user', field_extract_bundle('user', 'user'));
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    $langcode = field_language('user', $new_user, $field_name);
    if (isset($new_user->{$field_name}[$langcode])) {
      foreach (array_keys($new_user->{$field_name}[$langcode]) as $delta) {
        if (isset($new_user->{$field_name}[$langcode][$delta]['value'])) {
          $view_old = isset($old_user->{$field_name}[$langcode][$delta]['value']) ? $old_user->{$field_name}[$langcode][$delta]['value'] : "";
          $view_new = isset($new_user->{$field_name}[$langcode][$delta]['value']) ? $new_user->{$field_name}[$langcode][$delta]['value'] : "";
          $result["{$field_name}_{$delta}"] = array(
            '#name' => t($instance['label']),
            '#old' => explode("\n", $view_old),
            '#new' => explode("\n", $view_new),
          );
        }
        elseif (isset($new_user->{$field_name}[$langcode][$delta]['fid'])) {
          $view_old = isset($old_user->{$field_name}[$langcode][$delta]['fid']) ? $old_user->{$field_name}[$langcode][$delta]['fid'] : "";
          $view_new = isset($new_user->{$field_name}[$langcode][$delta]['fid']) ? $new_user->{$field_name}[$langcode][$delta]['fid'] : "";
          if ($view_old != "" && $view_old = file_load($view_old)) {
            $view_old = $view_old->filename;
          }
          if ($view_new != "" && $view_new = file_load($view_new)) {
            $view_new = $view_new->filename;
          }
          $result["{$field_name}_{$delta}"] = array(
            '#name' => t($instance['label']),
            '#old' => explode("\n", $view_old),
            '#new' => explode("\n", $view_new),
          );
        }
      }
    }
    if (isset($old_user->{$field_name}[$langcode])) {
      foreach (array_keys($old_user->{$field_name}[$langcode]) as $delta) {
        if (isset($old_user->{$field_name}[$langcode][$delta]['value']) && !isset($result["{$field_name}_{$delta}"])) {
          $view_old = isset($old_user->{$field_name}[$langcode][$delta]['value']) ? $old_user->{$field_name}[$langcode][$delta]['value'] : "";
          $view_new = isset($new_user->{$field_name}[$langcode][$delta]['value']) ? $new_user->{$field_name}[$langcode][$delta]['value'] : "";
          $result["{$field_name}_{$delta}"] = array(
            '#name' => t($instance['label']),
            '#old' => explode("\n", $view_old),
            '#new' => explode("\n", $view_new),
          );
        }
        elseif (isset($new_user->{$field_name}[$langcode][$delta]['fid']) && !isset($result["{$field_name}_{$delta}"])) {
          $view_old = isset($old_user->{$field_name}[$langcode][$delta]['fid']) ? $old_user->{$field_name}[$langcode][$delta]['fid'] : "";
          $view_new = isset($new_user->{$field_name}[$langcode][$delta]['fid']) ? $new_user->{$field_name}[$langcode][$delta]['fid'] : "";
          if ($view_old != "" && $view_old = file_load($view_old)) {
            $view_old = $view_old->filename;
          }
          if ($view_new != "" && $view_new = file_load($view_new)) {
            $view_new = $view_new->filename;
          }
          $result["{$field_name}_{$delta}"] = array(
            '#name' => t($instance['label']),
            '#old' => explode("\n", $view_old),
            '#new' => explode("\n", $view_new),
          );
        }
      }
    }
  }
  return $result;
}
