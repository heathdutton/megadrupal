<?php

/**
 * @file
 * An array of fields and field instances used by the content type.
 * declared by commune module.
 */

/**
 * Returns a structured array defining the fields created by this content type.
 *
 * @return array An associative array specifying the fields we wish to add to our
 *         new node type.
 */
function _commune_create_fields() {
  $t = get_t ();
  $name = 'commune_post';
    
  $fields['field_commune_url'] = array (
      'field_name' => 'field_commune_url',
      'cardinality' => 1,
      'locked' => TRUE,
      'type' => 'link_field',
      'bundles' => array (
          'node' => array (
              $name 
          ) 
      ) 
  );

  $fields['field_commune_upload'] = array (
      'field_name' => 'field_commune_upload',
      'cardinality' => 1,
      'type' => 'file',
      'locked' => TRUE,
      'bundles' => array (
          'node' => array (
              $name 
          ) 
      ) 
  );
  
  $fields['field_commune_post_markup'] = array (
      'field_name' => 'field_commune_post_markup',
      'cardinality' => 1,
      'type' => 'text',
      'length' => 512,
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
function _commune_create_instances() {
  $t = get_t ();
  $name = 'commune_post';
  
  $instance['field_commune_upload'] = array (
      'field_name' => 'field_commune_upload',
      'label' => $t ('File Uploads'),
      'type' => 'file',
      'required' => FALSE,
      'widget' => array (
          'type' => 'file_generic' 
      ),
      'display' => array (
          'default' => array (
              'label' => 'hidden',
              'type' => 'commune_embed_file',
          ),
      ),
      'entity_type' => 'node',
      'bundle' => $name,
      'deleted' => '0' ,
      'settings' => array('no_ui', TRUE),
  );


  $instance['field_commune_url'] = array (
      'field_name' => 'field_commune_url',
      'label' => $t ('Share Links'),
      'type' => 'link_field',
      'required' => FALSE,
      'widget' => array (
          'type' => 'link_field' 
      ),
      'display' => array (
          'default' => array (
              'label' => 'hidden',
              'type' => 'commune_embed',
          ), 
      ),
      'entity_type' => 'node',
      'bundle' => $name,
      'deleted' => '0' ,
      'settings' => array('no_ui', TRUE),
  );
  
  return $instance;
}

/**
 * Returns a structured array defining the fields created by this content type.
 *
 * @return array An associative array specifying the fields we wish to add to our
 *         new node type.
 */
function _commune_update_fields() {
}

/**
 * Returns a structured array defining the instances for this content type.
 *
 * @return array An associative array specifying the instances we wish to add to our new
 *         node type.
 */
function _commune_update_instances() {
}

