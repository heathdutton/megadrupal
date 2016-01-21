<?php
/**
 * @file
 * Fields for the `Youtube` content type.
 *
 * An array of fields and field instances used by the content type.
 */

/**
 * Returns a structured array defining the fields created by this content type.
 *
 * @return array
 *   An associative array specifying the fields we wish to add to our
 *   new node type.
 */
function social_content_youtube_create_fields() {

  return array(
    'field_youtube_picture' => array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'uri_scheme' => 'public',
        'default_image' => 0,
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_field_youtube_picture' => array(
                'fid' => 'field_youtube_picture_fid',
                'alt' => 'field_youtube_picture_alt',
                'title' => 'field_youtube_picture_title',
                'width' => 'field_youtube_picture_width',
                'height' => 'field_youtube_picture_height',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_field_youtube_picture' => array(
                'fid' => 'field_youtube_picture_fid',
                'alt' => 'field_youtube_picture_alt',
                'title' => 'field_youtube_picture_title',
                'width' => 'field_youtube_picture_width',
                'height' => 'field_youtube_picture_height',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(
        'fid' => array(
          'table' => 'file_managed',
          'columns' => array(
            'fid' => 'fid',
          ),
        ),
      ),
      'indexes' => array(
        'fid' => array(
          'fid',
        ),
      ),
      'field_name' => 'field_youtube_picture',
      'type' => 'image',
      'module' => 'image',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'fid' => array(
          'description' => 'The {file_managed}.fid being referenced in this field.',
          'type' => 'int',
          'not null' => FALSE,
          'unsigned' => TRUE,
        ),
        'alt' => array(
          'description' => "Alternative image text, for the image's 'alt' attribute.",
          'type' => 'varchar',
          'length' => 512,
          'not null' => FALSE,
        ),
        'title' => array(
          'description' => "Image title text, for the image's 'title' attribute.",
          'type' => 'varchar',
          'length' => 1024,
          'not null' => FALSE,
        ),
        'width' => array(
          'description' => 'The width of the image in pixels.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
        'height' => array(
          'description' => 'The height of the image in pixels.',
          'type' => 'int',
          'unsigned' => TRUE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          'youtube',
        ),
      ),
    ),
    'field_youtube_link' => array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'attributes' => array(
          'target' => 'default',
          'class' => '',
          'rel' => '',
        ),
        'url' => 0,
        'title' => 'optional',
        'title_value' => '',
        'title_maxlength' => 128,
        'enable_tokens' => 1,
        'display' => array(
          'url_cutoff' => 80,
        ),
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_field_youtube_link' => array(
                'url' => 'field_youtube_link_url',
                'title' => 'field_youtube_link_title',
                'attributes' => 'field_youtube_link_attributes',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_field_youtube_link' => array(
                'url' => 'field_youtube_link_url',
                'title' => 'field_youtube_link_title',
                'attributes' => 'field_youtube_link_attributes',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'field_name' => 'field_youtube_link',
      'type' => 'link_field',
      'module' => 'link',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'url' => array(
          'type' => 'varchar',
          'length' => 2048,
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
        'attributes' => array(
          'type' => 'text',
          'size' => 'medium',
          'not null' => FALSE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          'youtube',
        ),
      ),
    ),
    'field_youtube_embed' => array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'attributes' => array(
          'target' => 'default',
          'class' => '',
          'rel' => '',
        ),
        'url' => 0,
        'title' => 'optional',
        'title_value' => '',
        'title_maxlength' => 128,
        'enable_tokens' => 1,
        'display' => array(
          'url_cutoff' => 80,
        ),
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_field_youtube_embed' => array(
                'url' => 'field_youtube_embed_url',
                'title' => 'field_youtube_embed_title',
                'attributes' => 'field_youtube_embed_attributes',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_field_youtube_embed' => array(
                'url' => 'field_youtube_embed_url',
                'title' => 'field_youtube_embed_title',
                'attributes' => 'field_youtube_embed_attributes',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'field_name' => 'field_youtube_embed',
      'type' => 'link_field',
      'module' => 'link',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'url' => array(
          'type' => 'varchar',
          'length' => 2048,
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
        'title' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
          'sortable' => TRUE,
        ),
        'attributes' => array(
          'type' => 'text',
          'size' => 'medium',
          'not null' => FALSE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          'youtube',
        ),
      ),
    ),
    'field_youtube_external_id' => array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(
        'max_length' => '255',
      ),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_field_youtube_external_id' => array(
                'value' => 'field_youtube_external_id_value',
                'format' => 'field_youtube_external_id_format',
              ),
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_field_youtube_external_id' => array(
                'value' => 'field_youtube_external_id_value',
                'format' => 'field_youtube_external_id_format',
              ),
            ),
          ),
        ),
      ),
      'foreign keys' => array(
        'format' => array(
          'table' => 'filter_format',
          'columns' => array(
            'format' => 'format',
          ),
        ),
      ),
      'indexes' => array(
        'format' => array(
          'format',
        ),
      ),
      'field_name' => 'field_youtube_external_id',
      'type' => 'text',
      'module' => 'text',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array(
        'value' => array(
          'type' => 'varchar',
          'length' => '255',
          'not null' => FALSE,
        ),
        'format' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        ),
      ),
      'bundles' => array(
        'node' => array(
          'youtube',
        ),
      ),
    ),
  );
}

/**
 * Returns a structured array defining the instances for this content type.
 *
 * @return array
 *   An associative array specifying the instances we wish to add to our new
 *   node type.
 */
function social_content_youtube_create_instances() {

  return array(
    'field_youtube_picture' => array(
      'label' => 'Picture',
      'widget' => array(
        'weight' => '31',
        'type' => 'image_image',
        'module' => 'image',
        'active' => 1,
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
      ),
      'settings' => array(
        'file_directory' => 'social_content/youtube',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'alt_field' => 0,
        'title_field' => 0,
        'default_image' => 0,
        'user_register_form' => FALSE,
      ),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'type' => 'image',
          'settings' => array(
            'image_style' => '',
            'image_link' => '',
          ),
          'module' => 'image',
          'weight' => 1,
        ),
        'teaser' => array(
          'type' => 'hidden',
          'label' => 'above',
          'settings' => array(),
          'weight' => 0,
        ),
      ),
      'required' => 0,
      'description' => '',
      'field_name' => 'field_youtube_picture',
      'entity_type' => 'node',
      'bundle' => 'youtube',
      'deleted' => '0',
    ),
    'field_youtube_link' => array(
      'label' => 'Youtube link',
      'widget' => array(
        'weight' => '33',
        'type' => 'link_field',
        'module' => 'link',
        'active' => 0,
        'settings' => array(),
      ),
      'settings' => array(
        'validate_url' => 0,
        'url' => 0,
        'title' => 'none',
        'title_value' => '',
        'title_maxlength' => '128',
        'display' => array(
          'url_cutoff' => '80',
        ),
        'attributes' => array(
          'target' => '_blank',
          'rel' => '',
          'class' => '',
          'configurable_title' => 0,
          'title' => '',
        ),
        'rel_remove' => 'default',
        'enable_tokens' => 1,
        'user_register_form' => FALSE,
      ),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'type' => 'link_default',
          'settings' => array(),
          'module' => 'link',
          'weight' => 3,
        ),
        'teaser' => array(
          'type' => 'hidden',
          'label' => 'above',
          'settings' => array(),
          'weight' => 0,
        ),
      ),
      'required' => 0,
      'description' => '',
      'default_value' => NULL,
      'field_name' => 'field_youtube_link',
      'entity_type' => 'node',
      'bundle' => 'youtube',
      'deleted' => '0',
    ),
    'field_youtube_embed' => array(
      'label' => 'Youtube Video Embed',
      'widget' => array(
        'weight' => '33',
        'type' => 'link_field',
        'module' => 'link',
        'active' => 0,
        'settings' => array(),
      ),
      'settings' => array(
        'validate_url' => 0,
        'url' => 0,
        'title' => 'none',
        'title_value' => '',
        'title_maxlength' => '128',
        'display' => array(
          'url_cutoff' => '80',
        ),
        'attributes' => array(
          'target' => '_blank',
          'rel' => '',
          'class' => '',
          'configurable_title' => 0,
          'title' => '',
        ),
        'rel_remove' => 'default',
        'enable_tokens' => 1,
        'user_register_form' => FALSE,
      ),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'type' => 'link_default',
          'settings' => array(),
          'module' => 'link',
          'weight' => 3,
        ),
        'teaser' => array(
          'type' => 'hidden',
          'label' => 'above',
          'settings' => array(),
          'weight' => 0,
        ),
      ),
      'required' => 0,
      'description' => '',
      'default_value' => NULL,
      'field_name' => 'field_youtube_embed',
      'entity_type' => 'node',
      'bundle' => 'youtube',
      'deleted' => '0',
    ),
    'field_youtube_external_id' => array(
      'label' => 'External id',
      'widget' => array(
        'weight' => '32',
        'type' => 'text_textfield',
        'module' => 'text',
        'active' => 1,
        'settings' => array(
          'size' => '60',
        ),
      ),
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'type' => 'text_default',
          'settings' => array(),
          'module' => 'text',
          'weight' => 2,
        ),
        'teaser' => array(
          'type' => 'hidden',
          'label' => 'above',
          'settings' => array(),
          'weight' => 0,
        ),
      ),
      'required' => 0,
      'description' => '',
      'default_value' => NULL,
      'field_name' => 'field_youtube_external_id',
      'entity_type' => 'node',
      'bundle' => 'youtube',
      'deleted' => '0',
    ),
  );
}
