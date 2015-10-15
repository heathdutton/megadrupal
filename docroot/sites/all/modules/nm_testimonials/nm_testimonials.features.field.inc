<?php
/**
 * @file
 * nm_testimonials.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function nm_testimonials_field_default_fields() {
  $fields = array();

  // Exported field: 'node-nm_testimonial-field_nm_email'.
  $fields['node-nm_testimonial-field_nm_email'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_nm_email',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'email',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'email',
    ),
    'field_instance' => array(
      'bundle' => 'nm_testimonial',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Please enter your email address.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '3',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_nm_email',
      'label' => 'Email',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'email',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'email_textfield',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-nm_testimonial-field_nm_name'.
  $fields['node-nm_testimonial-field_nm_name'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_nm_name',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'nm_testimonial',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Please enter your Name.  Your email address will not be publicly displayed.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_plain',
          'weight' => '3',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_nm_name',
      'label' => 'Name',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'node-nm_testimonial-field_nm_rate'.
  $fields['node-nm_testimonial-field_nm_rate'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_nm_rate',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'fivestar',
      'settings' => array(
        'axis' => 'rate',
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'fivestar',
    ),
    'field_instance' => array(
      'bundle' => 'nm_testimonial',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'fivestar',
          'settings' => array(
            'expose' => TRUE,
            'style' => 'average',
            'text' => 'none',
            'widget' => array(
              'fivestar_widget' => 'sites/all/modules/contrib/fivestar/widgets/oxygen/oxygen.css',
            ),
          ),
          'type' => 'fivestar_formatter_default',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'fivestar',
          'settings' => array(
            'expose' => TRUE,
            'style' => 'user',
            'text' => 'none',
            'widget' => array(
              'fivestar_widget' => 'sites/all/modules/contrib/fivestar/widgets/oxygen/oxygen.css',
            ),
          ),
          'type' => 'fivestar_formatter_default',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_nm_rate',
      'label' => 'Rate',
      'required' => 1,
      'settings' => array(
        'allow_clear' => 0,
        'stars' => '5',
        'target' => 'none',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'fivestar',
        'settings' => array(
          'widget' => array(
            'fivestar_widget' => 'sites/all/modules/contrib/fivestar/widgets/basic/basic.css',
          ),
        ),
        'type' => 'stars',
        'weight' => '3',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Email');
  t('Name');
  t('Please enter your Name.');
  t('Please enter your email address.  Your email address will not be publicly displayed.');
  t('Rate');

  return $fields;
}
