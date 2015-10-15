<?php
/**
 * @file
 * crm_core_petition.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function crm_core_petition_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'crm_core_activity-cmcp_petition_signature-field_cmcp_petition_page'
  $field_instances['crm_core_activity-cmcp_petition_signature-field_cmcp_petition_page'] = array(
    'bundle' => 'cmcp_petition_signature',
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
        'weight' => 5,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcp_petition_page',
    'label' => 'Petition',
    'required' => 0,
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
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcp_petition_signature-field_cmcp_petition_targets'
  $field_instances['crm_core_activity-cmcp_petition_signature-field_cmcp_petition_targets'] = array(
    'bundle' => 'cmcp_petition_signature',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Please enter the targets of any online petitions here. This will be used to store information about messages sent to the targets of specific petition campaigns.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcp_petition_targets',
    'label' => 'Petition Targets',
    'required' => 0,
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
      'type' => 'entityreference_autocomplete_tags',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcp_petition_signature-field_cmcp_pm_content'
  $field_instances['crm_core_activity-cmcp_petition_signature-field_cmcp_pm_content'] = array(
    'bundle' => 'cmcp_petition_signature',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter your personal message here.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcp_pm_content',
    'label' => 'Personal message',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcp_petition_signature-field_cmcp_public_display'
  $field_instances['crm_core_activity-cmcp_petition_signature-field_cmcp_public_display'] = array(
    'bundle' => 'cmcp_petition_signature',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Click here to display your signature publicly.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcp_public_display',
    'label' => 'View publicly',
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
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcp_petition_signature-field_cmcp_received_date'
  $field_instances['crm_core_activity-cmcp_petition_signature-field_cmcp_received_date'] = array(
    'bundle' => 'cmcp_petition_signature',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
        ),
        'type' => 'date_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcp_received_date',
    'label' => 'Date of Signature',
    'required' => 0,
    'settings' => array(
      'default_value' => 'now',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'increment' => 15,
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'crm_core_activity-cmcp_petition_signature-field_cmcp_source'
  $field_instances['crm_core_activity-cmcp_petition_signature-field_cmcp_source'] = array(
    'bundle' => 'cmcp_petition_signature',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'crm_core_activity',
    'field_name' => 'field_cmcp_source',
    'label' => 'Petition Source',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-body'
  $field_instances['node-cmcp_petition-body'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter a description of the situation and explain why you are launching this petition.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Preamble',
    'required' => 0,
    'settings' => array(
      'display_summary' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-field_cmcp_cta'
  $field_instances['node-cmcp_petition-field_cmcp_cta'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Explain what you are asking people to do.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
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
    'field_name' => 'field_cmcp_cta',
    'label' => 'Call to Action',
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
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-field_cmcp_personal_message'
  $field_instances['node-cmcp_petition-field_cmcp_personal_message'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter any default text for the personalized message here. Leave blank for none.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
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
    'field_name' => 'field_cmcp_personal_message',
    'label' => 'Personal message',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-field_cmcp_pm_customize'
  $field_instances['node-cmcp_petition-field_cmcp_pm_customize'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this box if you want to allow signatories to include their own personalized message as part of their signature. In order for this to work, please ensure the personalized message field is included in any attached contact forms.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
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
    'field_name' => 'field_cmcp_pm_customize',
    'label' => 'Allow users to include a personalized message?',
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
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-field_cmcp_target_email_subject'
  $field_instances['node-cmcp_petition-field_cmcp_target_email_subject'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter a subject for the target email.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
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
    'field_name' => 'field_cmcp_target_email_subject',
    'label' => 'Target email subject',
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
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-field_cmcp_target_emails'
  $field_instances['node-cmcp_petition-field_cmcp_target_emails'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Check this box to send targeted emails upon receipt of signatures.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
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
    'field_name' => 'field_cmcp_target_emails',
    'label' => 'Send targeted emails?',
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
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-cmcp_petition-field_cmcp_target_ids'
  $field_instances['node-cmcp_petition-field_cmcp_target_ids'] = array(
    'bundle' => 'cmcp_petition',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter a list of contacts that will receive targeted emails. Each contact must have a valid email address.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
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
    'field_name' => 'field_cmcp_target_ids',
    'label' => 'Email targets.',
    'required' => 0,
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
      'type' => 'entityreference_autocomplete_tags',
      'weight' => 9,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Allow users to include a personalized message?');
  t('Call to Action');
  t('Check this box if you want to allow signatories to include their own personalized message as part of their signature. In order for this to work, please ensure the personalized message field is included in any attached contact forms.');
  t('Check this box to send targeted emails upon receipt of signatures.');
  t('Click here to display your signature publicly.');
  t('Date of Signature');
  t('Email targets.');
  t('Enter a description of the situation and explain why you are launching this petition.');
  t('Enter a list of contacts that will receive targeted emails. Each contact must have a valid email address.');
  t('Enter a subject for the target email.');
  t('Enter any default text for the personalized message here. Leave blank for none.');
  t('Enter your personal message here.');
  t('Explain what you are asking people to do.');
  t('Personal message');
  t('Petition');
  t('Petition Source');
  t('Petition Targets');
  t('Please enter the targets of any online petitions here. This will be used to store information about messages sent to the targets of specific petition campaigns.');
  t('Preamble');
  t('Send targeted emails?');
  t('Target email subject');
  t('View publicly');

  return $field_instances;
}
