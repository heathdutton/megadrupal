<?php
/**
 * @file
 */


function nm_core_add_body_field($type, $label = 'Body') {
  // Add or remove the body field, as needed.
  $field = field_info_field('field_nm_body');
  $instance = field_info_instance('node', 'field_nm_body', $type->type);

  if (empty($field)) {
    $field = array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node',
      ),
      'field_name' => 'field_nm_body',
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
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_with_summary',
    );
    $field = field_create_field($field);
  }

  if (empty($instance)) {
    $instance = array(
      'bundle' => $type->type,
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2',
        ),
        'gallery_teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '3',
        ),
        'nm_embed_gallery' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => 600,
          ),
          'type' => 'text_summary_or_trimmed',
          'weight' => '1',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_nm_body',
      'label' => $label,
      'required' => 0,
      'settings' => array(
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '20',
          'summary_rows' => 5,
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '6',
      ),
    );
    
    // let's alter the nm_body field on various content types
    switch ($type->type) {
      case 'nm_announcement':
        // here, we want the default teaser cutoff length to be a bit shorter than normal
        $instance['display']['teaser']['settings']['trim_length'] = 300;
      break;
      case 'nm_gallery':
        // hide the body field on gallery teasers
        $instance['display']['teaser']['type'] = 'hidden';
      break;
      case 'nm_testimonial':
      case 'forum':
        //don't have summary
        $instance['settings']['display_summary'] = 0;
        //require body field
        $instance['required'] = 1;
      break;
    }
    $instance = field_create_instance($instance);
  }
  return $instance;
}


function nm_core_add_reference_field($type, $label = 'Related Content') {
  // Add or remove the body field, as needed.
  $field = field_info_field('field_nm_related_content');
  $instance = field_info_instance('node', 'field_nm_related_content', $type->type);

  if (empty($field)) {
    $field = array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_nm_related_content',
      'foreign keys' => array(
        'node' => array(
          'columns' => array(
            'target_id' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'locked' => '0',
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'direction' => 'ASC',
            'field' => 'field_nm_attach_gallery:target_id',
            'property' => 'title',
            'type' => 'property',
          ),
          'target_bundles' => array(

          ),
        ),
        'handler_submit' => 'Change handler',
        'target_type' => 'node',
      ),
      'translatable' => '0',
      'type' => 'entityreference',
    );
    $field = field_create_field($field);
  }

  if (empty($instance)) {
    $instance = array(
      'bundle' => $type->type,
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
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
      'field_name' => 'field_nm_related_content',
      'label' => $label,
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
          'size' => '60',
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => '32',
      ),
    );
    $instance = field_create_instance($instance);
  }
  return $instance;
}


function nm_core_add_tags_field($type, $label = 'Tags') {
  // Add or remove the body field, as needed.
  $field = field_info_field('field_nm_tags');
  $instance = field_info_instance('node', 'field_nm_tags', $type->type);

  if (empty($field)) {
    $field = array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_nm_tags',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'nm_tags',
            'parent' => 0,
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    );
    $field = field_create_field($field);
  }

  if (empty($instance)) {
    $instance = array(
      'bundle' => $type->type,
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Enter a comma-separated list of words to describe your content.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 10,
        ),
        'teaser' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 10,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_nm_tags',
      'label' => $label,
      'required' => FALSE,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60,
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => 0,
      ),
    );
    $instance = field_create_instance($instance);
  }
  return $instance;
}


function nm_core_add_image_field($type, $label = 'Headline Image') {
  // Add or remove the body field, as needed.
  $field = field_info_field('field_nm_headline_image');
  $instance = field_info_instance('node', 'field_nm_headline_image', $type->type);

  if (empty($field)) {
    $field = array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_nm_headline_image',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'image',
      'settings' => array(
        'default_image' => FALSE,
        'uri_scheme' => 'public',
      ),
      'translatable' => '1',
      'type' => 'image',
    );
    $field = field_create_field($field);
  }

  if (empty($instance)) {

    $instance = array(
      'bundle' => $type->type,
      'deleted' => '0',
      'description' => 'A Headline Image adds interest to your content. Upload an image to go with this content.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => 'nodemaker_headline_image',
          ),
          'type' => 'image',
          'weight' => -1,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => 'content',
            'image_style' => 'nodemaker_headline_image',
          ),
          'type' => 'image',
          'weight' => -1,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_nm_headline_image',
      'label' => $label,
      'required' => FALSE,
      'settings' => array(
        'alt_field' => TRUE,
        'default_image' => 0,
        'file_directory' => 'images/headline',
        'file_extensions' => 'png gif jpg jpeg',
        'imagecrop' => array(
          'nodemaker_headline_image' => 'nodemaker_headline_image',
          'nodemaker_thumbnail' => 'nodemaker_thumbnail',
        ),
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => TRUE,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'nodemaker_thumbnail_small',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => -1,
      ),
    );

    // let's alter the $instance a bit for image styles
    $styles = image_styles();

    $style_default = 'nodemaker_headline_image';
    switch ($type->type) {
      case 'nm_blog':
        $teaser_style = isset($styles['nodemaker_blog_teaser']) ? 'nodemaker_blog_teaser' : $style_default;
        // add the teaser style as the default for teaser display
        $instance['display']['teaser']['settings']['image_style'] = $teaser_style;
        // allow the blog type to use imagecrop in this new style
        $instance['settings']['imagecrop'][$teaser_style] = $teaser_style;
      break;
      case 'nm_announcement':
        //$teaser_style = isset($styles['nodemaker_announcements_teaser']) ? 'nodemaker_announcements_teaser' : $style_default;
        //$instance['display']['teaser']['settings']['image_style'] = $teaser_style;
      break;
      case 'nm_event':
        $teaser_style = isset($styles['nodemaker_event_teaser']) ? 'nodemaker_event_teaser' : $style_default;
        $instance['display']['teaser']['settings']['image_style'] = $teaser_style;
      break;
    }

    // add the instance
    $instance = field_create_instance($instance);
  }
  return $instance;
}