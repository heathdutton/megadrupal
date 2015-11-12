<?php

/**
 * @file
 * bounce_convert.admin.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function bounce_convert_field_default_field_instances() {
  $t = get_t();
  $field_instances = array();

  // Exported field_instance: 'node-bounce_convert-field_bcc_active'
  $field_instances['node-bounce_convert-field_bcc_active'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Only active campaigns will be displayed to visitors.'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
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
    'field_name' => 'field_bcc_active',
    'label' => $t('Active'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => -3,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_background_color'
  $field_instances['node-bounce_convert-field_bcc_background_color'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => array(
      0 => array(
        'value' => '#000000',
      ),
    ),
    'deleted' => 0,
    'description' => $t('The color of the background behind the modal. Should be entered in html hex notation (eg \'#000000\').'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_bcc_background_color',
    'label' => $t('Background color'),
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
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_background_opacity'
  $field_instances['node-bounce_convert-field_bcc_background_opacity'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => array(
      0 => array(
        'value' => '0.5',
      ),
    ),
    'deleted' => 0,
    'description' => $t('The opacity of the background behind the modal. Should be entered as a decimal value (eg 0.85 = 85% opacity).'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_bcc_background_opacity',
    'label' => $t('Background opacity'),
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
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_cookie_expiration'
  $field_instances['node-bounce_convert-field_bcc_cookie_expiration'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Time in seconds. The popup will not be shown again to the same person until this amount of time has passed. Enter 0 to always show the popup (not recommended, except for testing).'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_bcc_cookie_expiration',
    'label' => $t('Cookie expiration'),
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
      'weight' => -1,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_modal_height'
  $field_instances['node-bounce_convert-field_bcc_modal_height'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('The height of the modal popup, in pixels.'),
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
    'field_name' => 'field_bcc_modal_height',
    'label' => $t('Modal height'),
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
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_modal_width'
  $field_instances['node-bounce_convert-field_bcc_modal_width'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('The width of the modal popup, in pixels.'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_bcc_modal_width',
    'label' => $t('Modal width'),
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

  // Exported field_instance: 'node-bounce_convert-field_bcc_pages_list'
  $field_instances['node-bounce_convert-field_bcc_pages_list'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Specify pages by using their paths. Enter one path per line. The <i>\'*\'</i> character is a wildcard. Example paths are <i>blog</i> for the blog page and <i>blog/*</i> for every personal blog.<code> &lt;front&gt; </code> is the front page.'),
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
    'field_name' => 'field_bcc_pages_list',
    'label' => $t('Pages list'),
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
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_pages_option'
  $field_instances['node-bounce_convert-field_bcc_pages_option'] = array(
    'bundle' => 'bounce_convert',
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
        'weight' => 11,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_bcc_pages_option',
    'label' => $t('Show campaign on specific pages'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-bounce_convert-field_bcc_webform'
  $field_instances['node-bounce_convert-field_bcc_webform'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Select the webform that will be the basis for this campaign.'),
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
    'field_name' => 'field_bcc_webform',
    'label' => $t('Select webform'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => -2,
    ),
  );

  // Export field_instance: 'node-bounce_convert-field_bcc_specific_user_roles'
  $field_instances['node-bounce_convert-field_bcc_specific_user_roles'] = array(
    'bundle' => 'bounce_convert',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Show this campaign only for the selected role(s). If you select no roles, the campaign will be shown to all users.'),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 12,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_bcc_specific_user_roles',
    'label' => $t('Show campaign for specific roles'),
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 9,
    ),
  );

  return $field_instances;
}
