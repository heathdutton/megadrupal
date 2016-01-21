<?php
/**
 * @file
 * Provides Views Support
 */

/**
 * Implements hook_views_data().
 */
function ms_files_views_data() {
  $data = array();

  // Definitions for ms_files_profiles
  $data['ms_files_profiles']['table']['group'] = t('File Download Plans');

  $data['ms_files_profiles']['table']['base'] = array(
    'field' => 'pfid',
    'title' => t('MS Files'),
    'help' => t("Create MS Files Related Views"),
    'weight' => -10,
  );

  //serial
  $data['ms_files_profiles']['pfid'] = array(
    'title' => 'File ID',
    'help' => 'The unique File ID (primary)',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //varchar
  $data['ms_files_profiles']['buynow'] = array(
    'title' => 'Buy Now Button',
    'help' => 'A button with a link to where the user can purchase the file',
    'field' => array(
        'handler' => 'ms_files_buynow_handler',
        'click sortable' => TRUE,
        'field' => 'pfid',
    ),
  );
  //varchar
  $data['ms_files_profiles']['filename'] = array(
    'title' => 'Filename',
    'help' => 'Filename of the file',
    'field' => array(
        'handler' => 'ms_files_filename_handler',
        'click sortable' => TRUE,
        'field' => 'fid',
    ),
  );
  //varchar
  $data['ms_files_profiles']['filesize'] = array(
    'title' => 'File Size',
    'help' => 'Size of the file',
    'field' => array(
        'handler' => 'ms_files_filesize_handler',
        'click sortable' => TRUE,
        'field' => 'fid',
    ),
  );
  //int
  $data['ms_files_profiles']['fid'] = array(
    'title' => 'File Fid',
    'help' => 'The File ID of the Drupal file',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['ms_files_downloads']['table']['join']['files'] = array(
    'left_field' => 'fid',
    'field' => 'fid',
  );
  //varchar
  $data['ms_files_profiles']['name'] = array(
    'title' => 'Title',
    'help' => 'The name of the File',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //text
  $data['ms_files_profiles']['description'] = array(
    'title' => 'Description',
    'help' => 'Description of the File',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //numeric
  $data['ms_files_profiles']['amount'] = array(
    'title' => 'Price',
    'help' => 'The Price of the File',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_float',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_float',
    ),
  );
  //varchar
  $data['ms_files_profiles']['price'] = array(
    'title' => 'Formatted Price',
    'help' => 'Price formatted according to currency code',
    'field' => array(
        'handler' => 'ms_files_price_handler',
        'click sortable' => TRUE,
        'field' => 'amount',
        'additional fields' => array(
          'amount' => array(
            'table' => 'ms_files_profiles',
            'field' => 'amount',
            ),
          ),
    ),
  );
  //int
  $data['ms_files_profiles']['rid'] = array(
    'title' => 'Role ID',
    'help' => 'The Role ID granted upon pruchase',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_profiles']['download_limit'] = array(
    'title' => 'Download Limit',
    'help' => 'The max number of times a file may be downloaded',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_profiles']['address_limit'] = array(
    'title' => 'Address Limit',
    'help' => 'The max number of different IP Addresses that may download the file',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_profiles']['time_limit'] = array(
    'title' => 'Expiration',
    'help' => 'The number of days until the file expires',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //varchar
  $data['ms_files_profiles']['purchase_mail_subject'] = array(
    'title' => 'Purchase Mail Subject',
    'help' => 'Subject for the Purchase Email',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //text
  $data['ms_files_profiles']['purchase_mail_body'] = array(
    'title' => 'Purchase Mail Body',
    'help' => 'Body for the Purchase Email',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //varchar
  $data['ms_files_profiles']['expired_mail_subject'] = array(
    'title' => 'Expired Mail Subject',
    'help' => 'Subject for the Expired Email',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //text
  $data['ms_files_profiles']['expired_mail_body'] = array(
    'title' => 'Expired Mail Body',
    'help' => 'Body for the Expired Email',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //int
  $data['ms_files_profiles']['weight'] = array(
    'title' => 'Weight',
    'help' => 'Used for ordering the files',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Definitions for ms_files_downloads
  $data['ms_files_downloads']['table']['group'] = t('File Downloads');

  //serial
  $data['ms_files_downloads']['fuid'] = array(
    'title' => 'File Download ID',
    'help' => 'The Unique ID of this File Download',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_downloads']['pfid'] = array(
    'title' => 'File ID',
    'help' => 'The File Profile ID',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['ms_files_downloads']['table']['join']['ms_files_profiles'] = array(
    'left_field' => 'pfid',
    'field' => 'pfid',
  );
  //int
  $data['ms_files_downloads']['fid'] = array(
    'title' => 'File ID',
    'help' => 'The Drupal File ID',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['ms_files_downloads']['table']['join']['files'] = array(
    'left_field' => 'fid',
    'field' => 'fid',
  );
  //varchar
  $data['ms_files_downloads']['filename'] = array(
    'title' => 'Filename',
    'help' => 'Filename of the file',
    'field' => array(
        'handler' => 'ms_files_filename_handler',
        'click sortable' => TRUE,
        'field' => 'fid',
    ),
  );
  //varchar
  $data['ms_files_downloads']['filesize'] = array(
    'title' => 'File Size',
    'help' => 'Size of the file',
    'field' => array(
        'handler' => 'ms_files_filesize_handler',
        'click sortable' => TRUE,
        'field' => 'fid',
    ),
  );
  //int
  $data['ms_files_downloads']['uid'] = array(
    'title' => 'User ID',
    'help' => 'The UID of the user who purchased the file',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['ms_files_downloads']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );
  //varchar
  $data['ms_files_downloads']['username'] = array(
    'title' => 'Username',
    'help' => 'The Username',
    'field' => array(
        'handler' => 'ms_files_username_handler',
        'click sortable' => TRUE,
        'field' => 'uid',
    ),
  );
  //int
  $data['ms_files_downloads']['oid'] = array(
    'title' => 'Order ID',
    'help' => 'The Order ID associated with the File Download',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //varchar
  $data['ms_files_downloads']['file_key'] = array(
    'title' => 'Download Key',
    'help' => 'A generated key used for the Download URL',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //int
  $data['ms_files_downloads']['downloads'] = array(
    'title' => 'Downloads',
    'help' => 'The number of times the file has been downloaded',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //text
  $data['ms_files_downloads']['addresses'] = array(
    'title' => 'Addresses',
    'help' => 'The list of IP Addresses that have downloaded the file',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_string',
    ),
  );
  //int
  $data['ms_files_downloads']['download_limit'] = array(
    'title' => 'Download Limit',
    'help' => 'The max number of times the file may be downloaded',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_downloads']['address_limit'] = array(
    'title' => 'Address Limit',
    'help' => 'The max number of IP Addresses that may download the file',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_downloads']['granted'] = array(
    'title' => 'Start Time',
    'help' => 'Timestamp of when the file was purchased',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_downloads']['expiration'] = array(
    'title' => 'Expiration',
    'help' => 'The Expiration Timestamp of the File Download',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );
  //int
  $data['ms_files_downloads']['status'] = array(
    'title' => 'Status',
    'help' => 'The Status of the File Download',
    'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
        'handler' => 'views_handler_argument_numeric',
    ),
  );


  return $data;
}
