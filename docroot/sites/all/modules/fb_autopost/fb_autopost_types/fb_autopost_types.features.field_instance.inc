<?php
/**
 * @file
 * fb_autopost_types.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fb_autopost_types_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'facebook_publication-album-field_facebook_message'
  $field_instances['facebook_publication-album-field_facebook_message'] = array(
    'bundle' => 'album',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The description of the album.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_message',
    'label' => 'Message',
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
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'facebook_publication-album-field_facebook_name'
  $field_instances['facebook_publication-album-field_facebook_name'] = array(
    'bundle' => 'album',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The title of the album',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_name',
    'label' => 'Name',
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'facebook_publication-event-field_facebook_description'
  $field_instances['facebook_publication-event-field_facebook_description'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Event description',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_description',
    'label' => 'Description',
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
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'facebook_publication-event-field_facebook_end_time'
  $field_instances['facebook_publication-event-field_facebook_end_time'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Event end time, in ISO-8601',
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
        'weight' => 2,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_end_time',
    'label' => 'End time',
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
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'facebook_publication-event-field_facebook_location'
  $field_instances['facebook_publication-event-field_facebook_location'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Event location',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_location',
    'label' => 'Location',
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
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'facebook_publication-event-field_facebook_location_id'
  $field_instances['facebook_publication-event-field_facebook_location_id'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Facebook Place ID of the place the Event is taking place',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_location_id',
    'label' => 'Location ID',
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

  // Exported field_instance: 'facebook_publication-event-field_facebook_name'
  $field_instances['facebook_publication-event-field_facebook_name'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Event name.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_name',
    'label' => 'Name',
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

  // Exported field_instance: 'facebook_publication-event-field_facebook_privacy_type'
  $field_instances['facebook_publication-event-field_facebook_privacy_type'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 'OPEN',
      ),
    ),
    'deleted' => 0,
    'description' => 'Event privacy setting',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_privacy_type',
    'label' => 'Privacy type',
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

  // Exported field_instance: 'facebook_publication-event-field_facebook_start_time'
  $field_instances['facebook_publication-event-field_facebook_start_time'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Event start time, in ISO-8601',
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
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_start_time',
    'label' => 'Start time',
    'required' => 1,
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
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'facebook_publication-link-field_facebook_link'
  $field_instances['facebook_publication-link-field_facebook_link'] = array(
    'bundle' => 'link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Link URL',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_link',
    'label' => 'Link',
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
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'facebook_publication-link-field_facebook_message'
  $field_instances['facebook_publication-link-field_facebook_message'] = array(
    'bundle' => 'link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Link message',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_message',
    'label' => 'Message',
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
      'weight' => 44,
    ),
  );

  // Exported field_instance: 'facebook_publication-link-field_facebook_published'
  $field_instances['facebook_publication-link-field_facebook_published'] = array(
    'bundle' => 'link',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Whether a post is published. Default is published.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_published',
    'label' => 'Published',
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
      'weight' => 46,
    ),
  );

  // Exported field_instance: 'facebook_publication-link-field_facebook_scheduled_publish'
  $field_instances['facebook_publication-link-field_facebook_scheduled_publish'] = array(
    'bundle' => 'link',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post. UNIX timestamp.',
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
        'weight' => 3,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_scheduled_publish',
    'label' => 'Scheduled publish time',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 48,
    ),
  );

  // Exported field_instance: 'facebook_publication-note-field_facebook_message'
  $field_instances['facebook_publication-note-field_facebook_message'] = array(
    'bundle' => 'note',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Note content',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_message',
    'label' => 'Message',
    'required' => 1,
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
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'facebook_publication-note-field_facebook_subject'
  $field_instances['facebook_publication-note-field_facebook_subject'] = array(
    'bundle' => 'note',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The subject of the Note',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_subject',
    'label' => 'Subject',
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
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'facebook_publication-photo-field_facebook_feed_targeting'
  $field_instances['facebook_publication-photo-field_facebook_feed_targeting'] = array(
    'bundle' => 'photo',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'JSON object containing targeting parameters 
See [Feed Targeting](#targeting)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_feed_targeting',
    'label' => 'Feed targeting',
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
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'facebook_publication-photo-field_facebook_message'
  $field_instances['facebook_publication-photo-field_facebook_message'] = array(
    'bundle' => 'photo',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Photo description',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_message',
    'label' => 'Message',
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
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'facebook_publication-photo-field_facebook_published'
  $field_instances['facebook_publication-photo-field_facebook_published'] = array(
    'bundle' => 'photo',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Whether a post is published. Default is published.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_published',
    'label' => 'Published',
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

  // Exported field_instance: 'facebook_publication-photo-field_facebook_scheduled_publish'
  $field_instances['facebook_publication-photo-field_facebook_scheduled_publish'] = array(
    'bundle' => 'photo',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post. UNIX timestamp',
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
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_scheduled_publish',
    'label' => 'Scheduled publish time',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'facebook_publication-photo-field_facebook_source'
  $field_instances['facebook_publication-photo-field_facebook_source'] = array(
    'bundle' => 'photo',
    'deleted' => 0,
    'description' => 'Facebook photo image. If the name is not set, then the title of the image will be used if set.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_source',
    'label' => 'Source',
    'required' => 1,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'facebook_publication-photo-field_facebook_targeting'
  $field_instances['facebook_publication-photo-field_facebook_targeting'] = array(
    'bundle' => 'photo',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'JSON object containing countries, cities, regions or locales 
Example: {\'countries\':[\'US\',\'GB\']}',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_targeting',
    'label' => 'Targeting',
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_actions'
  $field_instances['facebook_publication-post-field_facebook_actions'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post actions.

Input the name and link in the following way:
name|link',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_actions',
    'label' => 'Actions',
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
      'weight' => 47,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_caption'
  $field_instances['facebook_publication-post-field_facebook_caption'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post caption (can only be used if link is specified)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_caption',
    'label' => 'Caption',
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
      'weight' => 45,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_description'
  $field_instances['facebook_publication-post-field_facebook_description'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post description (can only be used if link is specified)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_description',
    'label' => 'Description',
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
      'weight' => 46,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_link'
  $field_instances['facebook_publication-post-field_facebook_link'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post URL',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_link',
    'label' => 'Link',
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
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_message'
  $field_instances['facebook_publication-post-field_facebook_message'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post message',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_message',
    'label' => 'Message',
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
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_name'
  $field_instances['facebook_publication-post-field_facebook_name'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post name (can only be used if link is specified)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_name',
    'label' => 'Name',
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
      'weight' => 44,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_picture'
  $field_instances['facebook_publication-post-field_facebook_picture'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Post thumbnail image (can only be used if link is specified)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_picture',
    'label' => 'Picture',
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
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_published'
  $field_instances['facebook_publication-post-field_facebook_published'] = array(
    'bundle' => 'post',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Whether a post is published. Default is published. This field is `not` supported when actions parameter is specified.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_published',
    'label' => 'Published',
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
      'weight' => 49,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_scheduled_publish'
  $field_instances['facebook_publication-post-field_facebook_scheduled_publish'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post.

UNIX timestamp',
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
        'weight' => 9,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_scheduled_publish',
    'label' => 'Scheduled publish time',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 50,
    ),
  );

  // Exported field_instance: 'facebook_publication-post-field_facebook_targeting'
  $field_instances['facebook_publication-post-field_facebook_targeting'] = array(
    'bundle' => 'post',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'JSON object containing countries, cities, regions or locales 
Example: {\'countries\':[\'US\',\'GB\']}',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_targeting',
    'label' => 'Targeting',
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
      'weight' => 48,
    ),
  );

  // Exported field_instance: 'facebook_publication-question-field_facebook_allow_new_options'
  $field_instances['facebook_publication-question-field_facebook_allow_new_options'] = array(
    'bundle' => 'question',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Allows other users to add new options (True by default)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_allow_new_options',
    'label' => 'Allow new options',
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
      'weight' => 43,
    ),
  );

  // Exported field_instance: 'facebook_publication-question-field_facebook_options'
  $field_instances['facebook_publication-question-field_facebook_options'] = array(
    'bundle' => 'question',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Array of answer options (one option per field value)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_options',
    'label' => 'Options',
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
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'facebook_publication-question-field_facebook_published'
  $field_instances['facebook_publication-question-field_facebook_published'] = array(
    'bundle' => 'question',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Whether a post is published. Default is published.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_published',
    'label' => 'Published',
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
      'weight' => 45,
    ),
  );

  // Exported field_instance: 'facebook_publication-question-field_facebook_question'
  $field_instances['facebook_publication-question-field_facebook_question'] = array(
    'bundle' => 'question',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The text of the question',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_question',
    'label' => 'Question',
    'required' => 1,
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
      'weight' => 41,
    ),
  );

  // Exported field_instance: 'facebook_publication-question-field_facebook_scheduled_publish'
  $field_instances['facebook_publication-question-field_facebook_scheduled_publish'] = array(
    'bundle' => 'question',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post. UNIX timestamp',
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
        'weight' => 4,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_scheduled_publish',
    'label' => 'Scheduled publish time',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 47,
    ),
  );

  // Exported field_instance: 'facebook_publication-status-field_facebook_message'
  $field_instances['facebook_publication-status-field_facebook_message'] = array(
    'bundle' => 'status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Status Message content',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_message',
    'label' => 'Message',
    'required' => 1,
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
      'weight' => 42,
    ),
  );

  // Exported field_instance: 'facebook_publication-status-field_facebook_published'
  $field_instances['facebook_publication-status-field_facebook_published'] = array(
    'bundle' => 'status',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Whether a post is published. Default is published.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_published',
    'label' => 'Published',
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
      'weight' => 44,
    ),
  );

  // Exported field_instance: 'facebook_publication-status-field_facebook_scheduled_publish'
  $field_instances['facebook_publication-status-field_facebook_scheduled_publish'] = array(
    'bundle' => 'status',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post. UNIX timestamp',
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
        'weight' => 2,
      ),
    ),
    'entity_type' => 'facebook_publication',
    'field_name' => 'field_facebook_scheduled_publish',
    'label' => 'Scheduled publish time',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 46,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Actions');
  t('Allow new options');
  t('Allows other users to add new options (True by default)');
  t('Array of answer options (one option per field value)');
  t('Caption');
  t('Description');
  t('End time');
  t('Event description');
  t('Event end time, in ISO-8601');
  t('Event location');
  t('Event name.');
  t('Event privacy setting');
  t('Event start time, in ISO-8601');
  t('Facebook Place ID of the place the Event is taking place');
  t('Facebook photo image. If the name is not set, then the title of the image will be used if set.');
  t('Feed targeting');
  t('JSON object containing countries, cities, regions or locales 
Example: {\'countries\':[\'US\',\'GB\']}');
  t('JSON object containing targeting parameters 
See [Feed Targeting](#targeting)');
  t('Link');
  t('Link URL');
  t('Link message');
  t('Location');
  t('Location ID');
  t('Message');
  t('Name');
  t('Note content');
  t('Options');
  t('Photo description');
  t('Picture');
  t('Post URL');
  t('Post actions.

Input the name and link in the following way:
name|link');
  t('Post caption (can only be used if link is specified)');
  t('Post description (can only be used if link is specified)');
  t('Post message');
  t('Post name (can only be used if link is specified)');
  t('Post thumbnail image (can only be used if link is specified)');
  t('Privacy type');
  t('Published');
  t('Question');
  t('Scheduled publish time');
  t('Source');
  t('Start time');
  t('Status Message content');
  t('Subject');
  t('Targeting');
  t('The description of the album.');
  t('The subject of the Note');
  t('The text of the question');
  t('The title of the album');
  t('Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post.

UNIX timestamp');
  t('Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post. UNIX timestamp');
  t('Time when the page post should go live, this should be between 10 mins and 6 months from the time of publishing the post. UNIX timestamp.');
  t('Whether a post is published. Default is published.');
  t('Whether a post is published. Default is published. This field is `not` supported when actions parameter is specified.');

  return $field_instances;
}
