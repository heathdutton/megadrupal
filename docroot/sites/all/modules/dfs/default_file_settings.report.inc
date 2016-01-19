<?php

/**
 * @file
 * Report include for displaying a compliance report.
 */

/**
 * Callback function to generate a report of all file field settings.
 */
function default_file_settings_status() {

  // Make sure the installation API is available.
  if (!defined('REQUIREMENT_ERROR')) {
    include_once DRUPAL_ROOT . '/includes/install.inc';
  }

  // Define an empty array of field requirements.
  $output = '';

  // Get all the file/image fields.
  $field_lookup = field_info_field_map();

  // Loop over fields and evaluate the fields, adding them to the report.
  foreach ($field_lookup as $field_name => $field_map_details) {
    if ($field_map_details['type'] == 'file' || $field_map_details['type'] == 'image') {

      foreach ($field_map_details['bundles'] as $entity => $bundles) {
        foreach ($bundles as $bundle) {
          $requirements = array();
          $field_info = field_info_instance($entity, $field_name, $bundle);

          unset($field_info['settings']['user_register_form']);
          $field_requirements = array(
            'title' => '<strong>' . $field_name . '</strong> (' . $entity . ' > ' . $bundle . ')',
          );

          default_file_settings_check_requirement(
            $field_requirements,
            'file_extensions',
            $field_info,
            REQUIREMENT_ERROR,
            t('Missing file extension')
          );
          default_file_settings_check_requirement(
            $field_requirements,
            'max_filesize',
            $field_info,
            variable_get('dfs_' . $field_map_details['type'] . '_require_filesize', FALSE) ? REQUIREMENT_ERROR : REQUIREMENT_WARNING,
            t('Missing max file size')
          );
          default_file_settings_check_requirement(
            $field_requirements,
            'file_directory',
            $field_info,
            REQUIREMENT_WARNING,
            t('Missing file directory')
          );
          if ($field_map_details['type'] == 'image') {
            default_file_settings_check_requirement(
              $field_requirements,
              'max_resolution',
              $field_info,
              REQUIREMENT_WARNING,
              t('Missing max resolution')
            );
            default_file_settings_check_requirement(
              $field_requirements,
              'min_resolution',
              $field_info,
              REQUIREMENT_WARNING,
              t('Missing min resolution')
            );
            default_file_settings_check_requirement(
              $field_requirements,
              'default_image',
              $field_info,
              REQUIREMENT_INFO,
              t('Missing default image')
            );
          }

          // This field passes the compliance check.
          if (!isset($field_requirements['severity'])) {
            $field_requirements['severity'] = REQUIREMENT_OK;
            $field_requirements['value'] = t('Complete');
          }

          $empty_list = FALSE;

          // Add a header to the requirements array.
          $header = array(
            'title' => $field_requirements['title'],
            'value' => '',
            'severity' => REQUIREMENT_INFO,
          );

          // General status row.
          $field_requirements['title'] = '<strong>' . t('Status report') . '</strong>';
          $field_requirements['value'] = '<strong>' . $field_requirements['value'] . '</strong>';
          $requirements[] = $field_requirements;

          foreach ($field_info['settings'] as $id => $setting) {
            $field_requirements['title'] = $id;
            // Check each configuration individually.
            if (empty($setting)) {
              $field_requirements['value'] = '';
              $field_requirements['severity'] = REQUIREMENT_WARNING;
              $require_max_filesize = variable_get('dfs_' . $field_map_details['type'] . '_require_filesize', FALSE);
              // Only mark missing values that are required with an 'Error' status.
              if ($id == 'file_extensions' || ($id == 'max_filesize' && $require_max_filesize)) {
                $field_requirements['value'] = '<strong>' . t('Missing') . '</strong>';
                $field_requirements['severity'] = REQUIREMENT_ERROR;
              }
            }
            else {
              $field_requirements['value'] = $setting;
              $field_requirements['severity'] = REQUIREMENT_OK;
            }
            $requirements[] = $field_requirements;
          }

          array_unshift($requirements, $header);

          // Present the report in the standard drupal report format.
          $output .= theme('status_report', array('requirements' => $requirements));
        }
      }
    }
  }

  if (empty($output)) {
    $output = t('There are currently no defined file or image fields.');
  }

  return $output;
}

/**
 * Function to test for default settings compliance.
 */
function default_file_settings_check_requirement(&$field_requirements, $req_setting, $field_info, $severity, $message) {
  if (empty($field_info['settings'][$req_setting])) {
    if (isset($field_requirements['severity'])) {
      switch ($severity) {

        case REQUIREMENT_ERROR:
          $field_requirements['severity'] = $severity;
          break;

        case REQUIREMENT_WARNING:
          if ($field_requirements['severity'] != REQUIREMENT_ERROR) {
            $field_requirements['severity'] = $severity;
          }
          break;
      }
    }
    else {
      $field_requirements['severity'] = $severity;
    }

    if (isset($field_requirements['value'])) {
      $field_requirements['value'] .= '<br />' . $message;
    }
    else {
      $field_requirements['value'] = $message;
    }
  }
}
