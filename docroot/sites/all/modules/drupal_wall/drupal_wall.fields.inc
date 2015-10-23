<?php

/**
 * @file
 * An array of fields and field instances used by the content type.
 * declared by Drupal wall module.
 */

/**
 * Returns a structured array defining the fields created by this content type.
 *
 * @return array An associative array specifying the fields we wish to add to our
 *         new node type.
 */
function _drupal_wall_create_fields() {
  $t = get_t ();
  $name = 'drupal_wall';
  
  $fields['field_drupal_wall_photos'] = array (
      'translatable' => '0',
      'entity_types' => array (),
      'settings' => array (
          'uri_scheme' => 'public',
          'default_image' => 0 
      ),
      'storage' => array (
          'type' => 'field_sql_storage',
          'settings' => array (),
          'module' => 'field_sql_storage',
          'active' => '1',
          'details' => array (
              'sql' => array (
                  'FIELD_LOAD_CURRENT' => array (
                      'field_data_field_drupal_wall_photos' => array (
                          'fid' => 'field_drupal_wall_photos_fid',
                          'alt' => 'field_drupal_wall_photos_alt',
                          'title' => 'field_drupal_wall_photos_title',
                          'width' => 'field_drupal_wall_photos_width',
                          'height' => 'field_drupal_wall_photos_height' 
                      ) 
                  ),
                  'FIELD_LOAD_REVISION' => array (
                      'field_revision_field_drupal_wall_photos' => array (
                          'fid' => 'field_drupal_wall_photos_fid',
                          'alt' => 'field_drupal_wall_photos_alt',
                          'title' => 'field_drupal_wall_photos_title',
                          'width' => 'field_drupal_wall_photos_width',
                          'height' => 'field_drupal_wall_photos_height' 
                      ) 
                  ) 
              ) 
          ) 
      ),
      'foreign keys' => array (
          'fid' => array (
              'table' => 'file_managed',
              'columns' => array (
                  'fid' => 'fid' 
              ) 
          ) 
      ),
      'indexes' => array (
          'fid' => array (
              'fid' 
          ) 
      ),
      'field_name' => 'field_drupal_wall_photos',
      'type' => 'image',
      'module' => 'image',
      'active' => '1',
      'locked' => '0',
      'cardinality' => '1',
      'deleted' => '0',
      'columns' => array (
          'fid' => array (
              'description' => 'The {file_managed}.fid being referenced in this field.',
              'type' => 'int',
              'not null' => FALSE,
              'unsigned' => TRUE 
          ),
          'alt' => array (
              'description' => "Alternative image text, for the image's 'alt' attribute.",
              'type' => 'varchar',
              'length' => 512,
              'not null' => FALSE 
          ),
          'title' => array (
              'description' => "Image title text, for the image's 'title' attribute.",
              'type' => 'varchar',
              'length' => 1024,
              'not null' => FALSE 
          ),
          'width' => array (
              'description' => 'The width of the image in pixels.',
              'type' => 'int',
              'unsigned' => TRUE 
          ),
          'height' => array (
              'description' => 'The height of the image in pixels.',
              'type' => 'int',
              'unsigned' => TRUE 
          ) 
      ),
      'bundles' => array (
          'node' => array (
              $name 
          ) 
      ) 
  );
  
  $fields['field_drupal_wall_videos'] = array (
      'field_name' => 'field_drupal_wall_videos',
      'cardinality' => 1,
      'type' => 'text',
      'bundles' => array (
          'node' => array (
              $name 
          ) 
      ) 
  );
  $fields['field_drupal_wall_image_style'] = array (
      'field_name' => 'field_drupal_wall_image_style',
      'cardinality' => 1,
      'type' => 'text',
      'bundles' => array (
          'node' => array (
              $name 
          ) 
      ) 
  );
  return $fields;
}

/**
 * Returns a structured array defining the instances for this content type.
 *
 * @return array An associative array specifying the instances we wish to add to our new
 *         node type.
 */
function _drupal_wall_create_instances() {
  $t = get_t ();
  $name = 'drupal_wall';
  
  $instance['field_drupal_wall_photos'] = array (
      'label' => 'Share Photo',
      'widget' => array (
          'weight' => '31',
          'type' => 'image_image',
          'module' => 'image',
          'active' => 1,
          'settings' => array (
              'progress_indicator' => 'throbber',
              'preview_image_style' => 'thumbnail' 
          ) 
      ),
      'settings' => array (
          'file_directory' => '',
          'file_extensions' => 'png gif jpg jpeg',
          'max_filesize' => '',
          'alt_field' => 0,
          'title_field' => 0,
          'default_image' => 0,
          'user_register_form' => FALSE 
      ),
      'display' => array (
          'default' => array (
              'label' => 'above',
              'type' => 'image',
              'settings' => array (
                  'image_style' => '',
                  'image_link' => '' 
              ),
              'module' => 'image',
              'weight' => 1 
          ),
          'teaser' => array (
              'type' => 'hidden',
              'label' => 'above',
              'settings' => array (),
              'weight' => 0 
          ) 
      ),
      'description' => '',
      'field_name' => 'field_drupal_wall_photos',
      'entity_type' => 'node',
      'bundle' => $name,
      'deleted' => '0' 
  );
  
  $instance['field_drupal_wall_videos'] = array (
      'field_name' => 'field_drupal_wall_videos',
      'label' => $t ('Share Videos'),
      'type' => 'text',
      'required' => FALSE,
      'widget' => array (
          'type' => 'text_textfield' 
      ),
      'display' => array (
          'joinprofile_field' => array (
              'label' => 'hidden',
              'type' => 'hidden' 
          ) 
      ),
      'entity_type' => 'node',
      'bundle' => $name,
      'deleted' => '0' 
  );
  
  $instance['field_drupal_wall_image_style'] = array (
      'field_name' => 'field_drupal_wall_image_style',
      'label' => $t ('Image Style'),
      'type' => 'text',
      'required' => FALSE,
      'widget' => array (
          'type' => 'text_textfield' 
      ),
      'display' => array (
          'joinprofile_field' => array (
              'label' => 'hidden',
              'type' => 'hidden' 
          ) 
      ),
      'entity_type' => 'node',
      'bundle' => $name,
      'deleted' => '0' 
  );
  return $instance;
}

/**
 * Returns a structured array defining the fields created by this content type.
 *
 * @return array An associative array specifying the fields we wish to add to our
 *         new node type.
 */
function _drupal_wall_update_fields() {
  $name = 'drupal_wall';
  
  return array (
      'field_name' => 'field_drupal_wall_image_style',
      'cardinality' => 1,
      'type' => 'text',
      'bundles' => array (
          'node' => array (
              $name 
          ) 
      ) 
  );
}

/**
 * Returns a structured array defining the instances for this content type.
 *
 * @return array An associative array specifying the instances we wish to add to our new
 *         node type.
 */
function _drupal_wall_update_instances() {
  $name = 'drupal_wall';
  return array (
      'field_name' => 'field_drupal_wall_image_style',
      'label' => t ('Photo Style'),
      'type' => 'text',
      'required' => FALSE,
      'widget' => array (
          'type' => 'text_textfield' 
      ),
      'display' => array (
          'joinprofile_field' => array (
              'label' => 'hidden',
              'type' => 'hidden' 
          ) 
      ),
      'entity_type' => 'node',
      'bundle' => $name,
      'deleted' => '0' 
  );

}

