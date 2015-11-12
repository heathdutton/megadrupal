<?php

/**
 * @file
 *   View API implementation
 */

/**
 * Implements hook_views_data().
 */
function views_s3_views_data() {
  $data = array();

  // Base table definition.
  $data['views_s3']['table'] = array(
    'group' => t('S3'),
    'base' => array(
      'title' => t('S3'),
      'help' => t('Information about your S3 files stored in the configured bucket in !link.', array('!link' => t('the configuration page'), 'admin/structure/views/s3')),
      'query class' => 'views_s3_query',
    ),
  );

  // Define views fields and filters
  // Hash
  $data['views_s3']['ETag'] = array(
    'title' => t('Hash'),
    'help' => t('Hash identifying a file.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_s3_handler_filter_equality',
    ),
  );
  
  // CloudFront signed URL
  // Only if there is information about CloudFront
  if (_is_cloudfront_active()) {
    $data['views_s3']['cdn_signed_url'] = array(
      'title' => t('CloudFront URL'),
      'help' => t('A CloudFront signed URL for this file. CAUTION: Displaying this URL to end users will allow them to donwload the file.'),
      'field' => array(
        'handler' => 'views_s3_handler_field_signed_url',
        'parent' => 'views_s3_handler_field',
        'click sortable' => FALSE,
      ),
    );
  }

  // File name
  $data['views_s3']['Key'] = array(
    'title' => t('File name'),
    'help' => t('The name of the file in S3. When used as filter or argument needs to be passed in urlencoded.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_s3_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_s3_handler_filter_equality',
    ),
  );

  // File name
  $data['views_s3']['signed_url'] = array(
    'title' => t('Signed URL'),
    'help' => t('A signed URL for this file. CAUTION: Displaying this URL to end users will allow them to donwload the file.'),
    'field' => array(
      'handler' => 'views_s3_handler_field_signed_url',
      'parent' => 'views_s3_handler_field',
      'click sortable' => FALSE,
    ),
  );
  
  // File name
  $data['views_s3']['basename'] = array(
    'title' => t('Base name'),
    'help' => t('The name of the file in S3 including extension.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
  );
  
  // Path
  $data['views_s3']['dirname'] = array(
    'title' => t('Path'),
    'help' => t('The path of the file in the bucket.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
  );
    
  // Timestamp
  $data['views_s3']['LastModified'] = array(
    'title' => t('Modified'),
    'help' => t('Timestamp when the file was last modified.'),
    'field' => array(
      'handler' => 'views_s3_handler_field_timestamp',
      'click sortable' => TRUE,      
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort_date',
    ),
  );
  
  $data['views_s3']['Size'] = array(
    'title' => t('Size'),
    'help' => t('The size of the file in bytes.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
  );
  
  $data['views_s3']['StorageClass'] = array(
    'title' => t('Storage class'),
    'help' => t('The type of storage used for this file in Amazon S3.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_s3_handler_filter_selectable',
      'select options' => array(
        'STANDARD' => t('Standard'),
        'REDUCED_REDUNDANCY' => t('Reduced redundancy'),
      ),
      'select title' => t('Storage type'),
      'select description' => t('Select the kind of storage class you want to filter by.'),
      'select value' => 'STANDARD',
    ),
  );
  
  $data['views_s3']['OwnerID'] = array(
    'title' => t('Owner ID'),
    'help' => t('Amazon ID for the owner of the file.'),
    'field' => array(
      'handler' => 'views_s3_handler_field_nested',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),    
  );
  
  $data['views_s3']['OwnerDisplayName'] = array(
    'title' => t('Display name'),
    'help' => t('Amazon display name for the owner of the file.'),
    'field' => array(
      'handler' => 'views_s3_handler_field_nested',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
  );
  
  // Extension
  $data['views_s3']['extension'] = array(
    'title' => t('Extension'),
    'help' => t('Extension of the file.'),
    'field' => array(
      'handler' => 'views_s3_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_s3_handler_filter_equality',
    ),
    'sort' => array(
      'handler' => 'views_s3_handler_sort',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function views_s3_views_plugins() {
  $plugins = array(
    'query' => array(
      'views_s3_query' => array(
        'title' => t('S3APIRequest'),
        'help' => t('Uses S3APIRequest for querying information about your files in your S3 bucket.'),
        'handler' => 'views_s3_plugin_query',
      ),
    ),
  );

  return $plugins;
}