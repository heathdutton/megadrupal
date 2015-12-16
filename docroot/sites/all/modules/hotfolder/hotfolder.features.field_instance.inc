<?php
/**
 * @file
 * hotfolder.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function hotfolder_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-job_record-field_job_log'
  $field_instances['node-job_record-field_job_log'] = array(
    'bundle' => 'job_record',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A log of job information to date.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_log',
    'label' => 'Job Log',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-job_record-field_job_record_actions'
  $field_instances['node-job_record-field_job_record_actions'] = array(
    'bundle' => 'job_record',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'hotfolder_action_config',
        'settings' => array(),
        'type' => 'hotfolder_action_config_formatter',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_record_actions',
    'label' => 'Configured Actions',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'hotfolder_action_config',
      'settings' => array(),
      'type' => 'hotfolder_action_config_widget',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-job_record-field_job_record_retries'
  $field_instances['node-job_record-field_job_record_retries'] = array(
    'bundle' => 'job_record',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_record_retries',
    'label' => 'Retries',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-job_record-field_job_record_source_file'
  $field_instances['node-job_record-field_job_record_source_file'] = array(
    'bundle' => 'job_record',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_record_source_file',
    'label' => 'Source File',
    'required' => FALSE,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'txt cvs csv xls xlsx xml',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-job_record-field_job_record_stage'
  $field_instances['node-job_record-field_job_record_stage'] = array(
    'bundle' => 'job_record',
    'default_value' => array(
      0 => array(
        'value' => 'prep',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_record_stage',
    'label' => 'Stage',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-job_record-field_job_record_status'
  $field_instances['node-job_record-field_job_record_status'] = array(
    'bundle' => 'job_record',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_record_status',
    'label' => 'Status',
    'required' => FALSE,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-job_record-field_job_record_watch'
  $field_instances['node-job_record-field_job_record_watch'] = array(
    'bundle' => 'job_record',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_job_record_watch',
    'label' => 'Watch',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-job_record-field_orig_watch_config_folder'
  $field_instances['node-job_record-field_orig_watch_config_folder'] = array(
    'bundle' => 'job_record',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Copied from the Watch configuration at the time of Job creation.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_orig_watch_config_folder',
    'label' => 'Original Watch Folder',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-job_record-field_pickup_filename'
  $field_instances['node-job_record-field_pickup_filename'] = array(
    'bundle' => 'job_record',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_pickup_filename',
    'label' => 'Pickup Filename',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-job_record-field_watch_config_delete'
  $field_instances['node-job_record-field_watch_config_delete'] = array(
    'bundle' => 'job_record',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_delete',
    'label' => 'Delete Completed',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 7,
    ),
  );

  // Exported field_instance:
  // 'node-watch_configuration-field_watch_config_actions'
  $field_instances['node-watch_configuration-field_watch_config_actions'] = array(
    'bundle' => 'watch_configuration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'hotfolder_action_config',
        'settings' => array(),
        'type' => 'hotfolder_action_config_formatter',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_actions',
    'label' => 'Configured Actions',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'hotfolder_action_config',
      'settings' => array(),
      'type' => 'hotfolder_action_config_widget',
      'weight' => 8,
    ),
  );

  // Exported field_instance:
  // 'node-watch_configuration-field_watch_config_delete'
  $field_instances['node-watch_configuration-field_watch_config_delete'] = array(
    'bundle' => 'watch_configuration',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Delete completed jobs after they are done. This will also delete the associated job file.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_delete',
    'label' => 'Delete Completed',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'node-watch_configuration-field_watch_config_folder'
  $field_instances['node-watch_configuration-field_watch_config_folder'] = array(
    'bundle' => 'watch_configuration',
    'default_value' => array(
      0 => array(
        'value' => 'hotfolder',
      ),
    ),
    'deleted' => 0,
    'description' => 'Folder to watch, within default sites path. It is the responsibility of the user to make sure the watchfolder is in place on the server. For security reasons this configuration will not automatically create it.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_folder',
    'label' => 'Watch Folder',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'node-watch_configuration-field_watch_config_recurse'
  $field_instances['node-watch_configuration-field_watch_config_recurse'] = array(
    'bundle' => 'watch_configuration',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'A recursive watch will check all folders below the watched folder for new files as well.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_recurse',
    'label' => 'Recursive Watch',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'node-watch_configuration-field_watch_config_scheme'
  $field_instances['node-watch_configuration-field_watch_config_scheme'] = array(
    'bundle' => 'watch_configuration',
    'default_value' => array(
      0 => array(
        'tid' => 8,
      ),
    ),
    'deleted' => 0,
    'description' => 'The schema to use with this watch folder. Add additional streams as needed to identify the correct path from root for your folder.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_scheme',
    'label' => 'Stream',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'node-watch_configuration-field_watch_config_src_pattern'
  $field_instances['node-watch_configuration-field_watch_config_src_pattern'] = array(
    'bundle' => 'watch_configuration',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'This is a regular expression pattern.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_watch_config_src_pattern',
    'label' => 'Source Filename Pattern',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'taxonomy_term-hotfolder_streams-field_stream_folder'
  $field_instances['taxonomy_term-hotfolder_streams-field_stream_folder'] = array(
    'bundle' => 'hotfolder_streams',
    'default_value' => array(
      0 => array(
        'value' => '/',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_stream_folder',
    'label' => 'Stream folder',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A log of job information to date.');
  t('A recursive watch will check all folders below the watched folder for new files as well.');
  t('Configured Actions');
  t('Copied from the Watch configuration at the time of Job creation.');
  t('Delete Completed');
  t('Delete completed jobs after they are done. This will also delete the associated job file.');
  t('Folder to watch, within default sites path. It is the responsibility of the user to make sure the watchfolder is in place on the server. For security reasons this configuration will not automatically create it.');
  t('Job Log');
  t('Original Watch Folder');
  t('Pickup Filename');
  t('Recursive Watch');
  t('Retries');
  t('Source File');
  t('Source Filename Pattern');
  t('Stage');
  t('Status');
  t('Stream');
  t('Stream folder');
  t('The schema to use with this watch folder. Add additional streams as needed to identify the correct path from root for your folder.');
  t('This is a regular expression pattern.');
  t('Watch');
  t('Watch Folder');

  return $field_instances;
}
