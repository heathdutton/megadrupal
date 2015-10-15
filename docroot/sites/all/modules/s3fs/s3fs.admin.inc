<?php

/**
 * @file
 * Administration form setup for S3 File System.
 */

/**
 * Builds the Settings form.
 */
function s3fs_settings() {
  $config = _s3fs_get_config();
  $form = array();
  // I'd like to be able to pull this information directly from the SDK, but
  // I couldn't find a good way to get the human-readable region names.
  $region_map = array(
    '' => 'Default',
    'us-east-1' => 'US Standard (us-east-1)',
    'us-west-1' => 'US West - Northern California  (us-west-1)',
    'us-west-2' => 'US West - Oregon (us-west-2)',
    'us-gov-west-1' => 'USA GovCloud Standard (us-gov-west-1)',
    'eu-west-1' => 'EU - Ireland  (eu-west-1)',
    'eu-central-1' => 'EU - Frankfurt (eu-central-1)',
    'ap-southeast-1' => 'Asia Pacific - Singapore (ap-southeast-1)',
    'ap-southeast-2' => 'Asia Pacific - Sydney (ap-southeast-2)',
    'ap-northeast-1' => 'Asia Pacific - Tokyo (ap-northeast-1)',
    'sa-east-1' => 'South America - Sao Paulo (sa-east-1)',
    'cn-north-1' => 'China - Beijing (cn-north-1)',
  );

  $form['s3fs_credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Amazon Web Services Credentials'),
    '#description' => t(
      "To configure your Amazon Web Services credentials, enter the values in the appropriate fields below.
      You may instead set \$conf['awssdk2_access_key'] and \$conf['awssdk2_secret_key'] in your site's settings.php file.
      Values set in settings.php will override the values in these fields."
    ),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($config['awssdk2_access_key']) || !empty($config['use_instance_profile']),
  );

  $form['s3fs_credentials']['s3fs_awssdk2_access_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon Web Services Access Key'),
    '#default_value' => ($access_key = !empty($config['awssdk2_access_key']) ? $config['awssdk2_access_key'] : ''),
  );
  // Override with awssdk2_access_key from settings.php, if set.
  $form['s3fs_credentials']['s3fs_awssdk2_access_key']['#default_value'] = variable_get('awssdk2_access_key', $access_key);

  $form['s3fs_credentials']['s3fs_awssdk2_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Amazon Web Services Secret Key'),
    '#default_value' => ($secret_key = !empty($config['awssdk2_secret_key']) ? $config['awssdk2_secret_key'] : ''),
  );
  // Override with awssdk2_secret_key from settings.php, if set.
  $form['s3fs_credentials']['s3fs_awssdk2_secret_key']['#default_value'] = variable_get('awssdk2_secret_key', $secret_key);

  $form['s3fs_credentials']['s3fs_use_instance_profile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use EC2 Instance Profile Credentials'),
    '#default_value' => !empty($config['use_instance_profile']) ? $config['use_instance_profile'] : FALSE,
    '#description' => t(
      'If your Drupal site is running on an Amazon EC2 server, you may use the Instance Profile Credentials from that server
      rather than setting your AWS credentials directly.'
    ),
  );
  $form['s3fs_credentials']['s3fs_awssdk2_default_cache_config'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Cache Location'),
    '#description' => t('The default cache location for your EC2 Instance Profile Credentials.'),
    '#default_value' => ($cache_config = !empty($config['awssdk2_default_cache_config']) ? $config['awssdk2_default_cache_config'] : ''),
    '#states' => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-instance-profile]' => array('checked' => TRUE),
      ),
    ),
  );
  // Override with awssdk2_default_cache_config from settings.php, if set.
  $form['s3fs_credentials']['s3fs_awssdk2_default_cache_config']['#default_value'] = variable_get('awssdk2_default_cache_config', $cache_config);

  $form['s3fs_bucket'] = array(
    '#type'           => 'textfield',
    '#title'          => t('S3 Bucket Name'),
    '#default_value'  => !empty($config['bucket']) ? $config['bucket'] : '',
    '#required'       => TRUE,
  );
  $form['s3fs_region'] = array(
    '#type'          => 'select',
    '#options'       => $region_map,
    '#title'         => t('S3 Region'),
    '#description'   => t(
      'The region in which your bucket resides. Be careful to specify this accurately,
      as you are likely to see strange or broken behavior if the region is set wrong.<br>
      Use of the USA GovCloud region requires !SPECIAL_PERMISSION.<br>
      Use of the China - Beijing region requires a !CHINESE_AWS_ACCT.',
      array(
          '!CHINESE_AWS_ACCT' => l('亚马逊 AWS account', 'http://www.amazonaws.cn'),
          '!SPECIAL_PERMISSION' => l('special permission', 'http://aws.amazon.com/govcloud-us/'),
      )
    ),
    '#default_value' => !empty($config['region']) ? $config['region'] : '',
  );
  $form['s3fs_use_cname'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable CNAME'),
    '#description'   => t('Serve files from a custom domain by using an appropriately named bucket, e.g. "mybucket.mydomain.com".'),
    '#default_value' => !empty($config['use_cname']) ? $config['use_cname'] : FALSE,
  );
  $form['s3fs_cname_settings_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('CNAME Settings'),
    '#states' => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-cname]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['s3fs_cname_settings_fieldset']['s3fs_domain'] = array(
    '#type'          => 'textfield',
    '#title'         => t('CDN Domain Name'),
    '#description'   => t('If serving files from CloudFront, the bucket name can differ from the domain name.'),
    '#default_value' => !empty($config['domain']) ? $config['domain'] : '',
  );
  $form['s3fs_cache_control_header'] = array(
    '#type'          => 'textfield',
    '#title'         => t('S3 Object Cache-Control Header'),
    '#description'   => t('The cache control header to set on all S3 objects for CDNs and browsers, e.g. "public, max-age=300".'),
    '#default_value' => !empty($config['cache_control_header']) ? $config['cache_control_header'] : '',
  );
  $form['s3fs_use_https'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Always serve files from S3 via HTTPS'),
    '#description'   => t(
      'Forces S3 File System to always generate HTTPS URLs for files in your bucket,
      e.g. "https://mybucket.s3.amazonaws.com/smiley.jpg".<br>
      Without this setting enabled, URLs for your files will use the same scheme as the page they are served from.'
    ),
    '#default_value' => !empty($config['use_https']) ? $config['use_https'] : FALSE,
  );
  $form['s3fs_use_customhost'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use a Custom Host'),
    '#description'   => t('Connect to an S3-compatible storage service other than Amazon.'),
    '#default_value' => !empty($config['use_customhost']) ? $config['use_customhost'] : FALSE,
  );
  $form['s3fs_costomhost_settings_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Host Settings'),
    '#states' => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-customhost]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['s3fs_costomhost_settings_fieldset']['s3fs_hostname'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Hostname'),
    '#description'   => t('Custom service hostname, e.g. "objects.dreamhost.com".'),
    '#default_value' => !empty($config['hostname']) ? $config['hostname'] : '',
    '#states'        => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-customhost]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['s3fs_ignore_cache'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Ignore the file metadata cache'),
    '#description'   => t(
      "If you need to debug a problem with S3, you may want to temporarily ignore the file metadata cache.
      This will make all file system reads hit S3 instead of the cache.<br>
      <b>This causes s3fs to work extremely slowly, and should never be enabled on a production site.</b>"
    ),
    '#default_value' => !empty($config['ignore_cache']) ? $config['ignore_cache'] : FALSE,
  );
  $form['s3fs_use_s3_for_public'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use S3 for public:// files'),
    '#description'   => t(
      'Enable this option to store all files which would be uploaded to or created in the public://
      file system into your S3 bucket instead.<br><br>
      <b>PLEASE NOTE:</b> If you intend to use Drupal\'s performance options which aggregate your CSS or Javascript
      files, or will be using any other system that writes CSS or Javascript files into your site\'s public:// file system,
      you must perform some additional configuration on your webserver to make those files work correctly when stored in S3.
      Please see the section titled "Aggregated CSS and JS in S3" in the !README for details.',
      array('!README' => l('README', drupal_get_path('module', 's3fs') . '/README.txt'))
    ),
    '#default_value' => !empty($config['use_s3_for_public']) ? $config['use_s3_for_public'] : FALSE,
  );
  $form['s3fs_no_rewrite_cssjs'] = array(
    '#type' => 'checkbox',
    '#title' => t("Don't rewrite CSS/JS file paths"),
    '#description' => t(
      'If this box is checked, s3fs will NOT rewrite the CSS/JS file paths to "/s3fs-(css|js)/...". Instead, they will
      be placed on the page with their regular CDN name. Only enable this option if you <b>know</b> you need it!'
    ),
    '#default_value' => !empty($config['no_rewrite_cssjs']) ? $config['no_rewrite_cssjs'] : FALSE,
    '#states'        => array(
      'visible' => array(
        ':input[id=edit-s3fs-use-s3-for-public]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['s3fs_use_s3_for_private'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use S3 for private:// files'),
    '#description'   => t(
      'Enable this option to store all files which would be uploaded to or created in the private://
      file system in your S3 bucket instead.'
    ),
    '#default_value' => !empty($config['use_s3_for_private']) ? $config['use_s3_for_private'] : FALSE,
  );
  $form['s3fs_root_folder'] = array(
    '#type'           => 'textfield',
    '#title'          => t('Root Folder'),
    '#default_value'  => !empty($config['root_folder']) ? $config['root_folder'] : '',
    '#description'   => t(
      'S3 File System uses the specified folder as the root of the file system within your bucket (if blank, the bucket root is used).
      This is helpful when your bucket is used by multiple sites, or has additional data in it which s3fs should not interfere with.<br>
      The metadata refresh function will not retrieve metadata for any files which are outside the Root Folder.<br>
      This setting is case sensitive. Do not include leading or trailing slashes.<br>
      Changing this setting <b>will not</b> move any files. If you\'ve already uploaded files to S3 through s3fs, you will need to manually move
      them into this folder.'
    ),
  );
  $form['s3fs_presigned_urls'] = array(
    '#type' => 'textarea',
    '#title' => t('Presigned URLs'),
    '#description' => t(
      'A list of timeouts and paths that should be delivered through a presigned url.<br>
      Enter one value per line, in the format timeout|path. e.g. "60|private_files/*". Paths use regex patterns
      as per !link. If no timeout is provided, it defaults to 60 seconds.<br>
      <b>This feature does not work when "Enable CNAME" is used.</b>',
      array('!link' => l('preg_match', 'http://php.net/preg_match'))
    ),
    '#default_value' => !empty($config['presigned_urls']) ? $config['presigned_urls'] : '',
    '#rows' => 5,
  );
  $form['s3fs_saveas'] = array(
    '#type' => 'textarea',
    '#title' => t('Force Save As'),
    '#description' => t(
      'A list of paths for which users will be forced to save the file, rather than displaying it in the browser.<br>
      Enter one value per line. e.g. "video/*". Paths use regex patterns as per !link.<br>
      <b>This feature does not work when "Enable CNAME" is used.</b>',
      array('!link' => l('preg_match', 'http://php.net/preg_match'))
    ),
    '#default_value' => !empty($config['saveas']) ? $config['saveas'] : '',
    '#rows' => 5,
  );
  $form['s3fs_torrents'] = array(
    '#type' => 'textarea',
    '#title' => t('Torrents'),
    '#description' => t(
      'A list of paths that should be delivered via BitTorrent.<br>
      Enter one value per line, e.g. "big_files/*". Paths use regex patterns as per !link.<br>
      <b>Private files and paths which are already set as Presigned URLs or Forced Save As cannot be delivered as torrents.</b>',
      array('!link' => l('preg_match', 'http://php.net/preg_match'))
    ),
    '#default_value' => !empty($config['torrents']) ? $config['torrents'] : '',
    '#rows' => 5,
  );

  $form = system_settings_form($form);
  return $form;
}

/**
 * Validates the values on the admin form.
 */
function s3fs_settings_validate($form, &$form_state) {
  $config = _s3fs_get_config();
  $values = _s3fs_convert_form_state_to_config($form_state);
  $config = array_merge($config, $values);
  _s3fs_validate_config($config);
}

/**
 * Converts a $form_state array to a configuration settings array.
 */
function _s3fs_convert_form_state_to_config($form_state) {
  $config = array();
  foreach ($form_state['values'] as $name => $value) {
    // If the name starts with 's3fs_', strip that off and save the value.
    if (substr($name, 0, 5) == 's3fs_') {
      $config[substr($name, 5)] = $value;
    }
  }
  return $config;
}

/**
 * Builds the Actions form.
 */
function s3fs_actions() {
  $form = array();

  // Drupal's menu system doesn't let you set the page title for tabs.
  // So we set it here.
  drupal_set_title('S3 File System Actions');

  $form['s3fs_refresh_cache'] = array(
    '#type' => 'fieldset',
    '#description' => t(
      "The file metadata cache keeps track of every file that S3 File System writes to (and deletes from) the S3 bucket,
      so that queries for data about those files (checks for existence, filetype, etc.) don't have to hit S3.
      This speeds up many operations, most noticeably anything related to images and their derivatives."
    ),
    '#title' => t('File Metadata Cache'),
  );
  $refresh_description = t(
    "This button queries S3 for the metadata of <i><b>all</b></i> the files in your site's bucket (unless you use the Root Folder option), and saves it to the
    database. This may take a while for buckets with many thousands of files. <br>
    It should only be necessary to use this button if you've just installed S3 File System and you need to cache all the
    pre-existing files in your bucket, or if you need to restore your metadata cache from scratch for some other reason."
  );
  $form['s3fs_refresh_cache']['refresh'] = array(
    '#type' => 'submit',
    '#suffix' => '<div class="refresh">' . $refresh_description . '</div>',
    '#value' => t('Refresh file metadata cache'),
    '#attached' => array(
      'css' => array(
        // Push the button closer to its own description, and push the disable
        // checkbox away from the description.
        '#edit-refresh {margin-bottom: 0; margin-top: 1em;} div.refresh {margin-bottom: 1em;}' => array('type' => 'inline')
      ),
    ),
    '#submit' => array('_s3fs_refresh_cache_submit'),
  );

  $config = _s3fs_get_config();
  $use_public = !empty($config['use_s3_for_public']);
  $use_private = !empty($config['use_s3_for_private']);

  if ($use_public || $use_private) {
    $form['s3fs_copy_local'] = array(
      '#type' => 'fieldset',
      '#description' => t(
        "Since you have S3 File System configured to take over for the public and/or private file systems, you
        may wish to copy any files which were previously uploaded to your site into your S3 bucket. <br>
        If you have a lot of files, or very large files, you'll want to use <i>drush s3fs-copy-local</i>
        instead of this form, as the limitations imposed by browsers may break very long copy operations."
      ),
      '#title' => t('Copy Local Files to S3'),
    );
    if ($use_public) {
      $form['s3fs_copy_local']['public'] = array(
        '#type' => 'submit',
        '#prefix' => '<br>',
        '#name' => 'public',
        '#value' => t('Copy local public files to S3'),
        '#submit' => array('_s3fs_copy_local_submit'),
      );
    }
    if ($use_private) {
      $form['s3fs_copy_local']['private'] = array(
        '#type' => 'submit',
        '#prefix' => '<br>',
        '#name' => 'private',
        '#value' => t('Copy local private files to S3'),
        '#submit' => array('_s3fs_copy_local_submit'),
      );
    }
  }

  return $form;
}

/**
 * Submit callback for the "Refresh file metadata cache" button.
 */
function _s3fs_refresh_cache_submit($form, &$form_state) {
  $config = _s3fs_get_config();
  _s3fs_refresh_cache($config);
}

/**
 * Submit callback for the "Copy local files to S3" buttons.
 */
function _s3fs_copy_local_submit($form, &$form_state) {
  _s3fs_copy_file_system_to_s3($form_state['triggering_element']['#name']);
}

