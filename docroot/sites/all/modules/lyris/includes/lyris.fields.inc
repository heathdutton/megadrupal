<?php
/**
 * @file
 * Define Lyris field api fields.
 */
/**
 * Implements hook_lyris_fields().
 *
 * http://api.drupal.org/api/drupal/modules!field!field.module/group/field/7
 *
 * @see lyris_fields()
 */
function lyris_lyris_fields() {
  $fields = array();

  // User's username for Lyris.
  $fields['lyris_username'] = array(
    'field' => array(
      'type' => 'text',
      'entity_types' => array('user'),
      'locked' => TRUE,
      'settings' => array(
        'maxlength' => 64,
      ),
    ),
    'instance' => array(
      'user' => array(
        'access' => 'access lyris api',
        'user' => array(
          'label' => 'Lyris Username',
          'widget' => array('type' => 'textfield'),
          'display' => array(
            'default' => array(
              'label' => 'inline',
              'type' => 'hidden',
            ),
          ),
        ),
      ),
    ),
  );

  // User's password for Lyris.
  $fields['lyris_userpass'] = array(
    'field' => array(
      'type' => 'password_field',
      'entity_types' => array('user'),
      //'locked' => TRUE,
      'settings' => array(
        'maxlength' => 64,
      ),
    ),
    'instance' => array(
      'user' => array(
        'access' => 'access lyris api',
        'user' => array(
          'label' => 'Lyris Password',
          'widget' => array('type' => 'password_field'),
          'display' => array(
            'default' => array(
              'label' => 'inline',
              'type' => 'hidden',
            ),
          ),
        ),
      ),
    ),
  );

  // Content title field
  // NOT CURRENTLY USED
  $fields['lyris_content_title'] = array(
    'field' => array(
      'type' => 'text',
      'entity_types' => array('lyris_content'),
      'cardinality' => 1,
      'locked' => FALSE,
      'settings' => array(
        'maxlength' => 128,
      ),
    ),
    'instance' => array(
      'lyris_content' => array(
        'access' => 'create lyris lists',
        'all' => array(
          'label' => 'Title',
          'field_name' => 'Title',
          'description' => 'Enter a title for this content or issue.',
          'required' => TRUE,
          'widget' => array(
            'type' => 'textfield',
            'weight' => -10,
          ),
          'settings' => array(),
          'display' => array(
            'default' => array(
              'label' => 'hidden',
              'type' => 'plain'
            ),
          ),
        ),
      ),
    ),
  );

  // Content body field
  $fields['lyris_content_docparts'] = array(
    'field' => array(
      'type' => 'text_with_summary',
      'entity_types' => array('lyris_content'),
      'cardinality' => 1,
      'locked' => FALSE,
      'settings' => array(),
    ),
    'instance' => array(
      'lyris_content' => array(
        'all' => array(
          'label' => 'Body',
          'description' => 'Create content to be delivered to your mailing list subscribers.',
          'widget' => array(
            'type' => 'textarea',
            'weight' => -9,
          ),
          'display' => array(
            'default' => array(
              'label' => 'hidden',
            ),
          ),
        ),
      ),
    ),
  );

  return $fields;
}
