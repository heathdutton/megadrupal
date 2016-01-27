Installing the AWS SDK for PHP v2
---------------------------------------------

1. Read the "Installation and Usage For Site Builders" for https://drupal.org/project/composer_manager

2. Enable AWS SDK for PHP v2
   $ drush en awssdk2

3. Install/Update dependencies
   $ drush composer-manager install / update

4. Run the compatibility test script.
   $ php pathToVendorDirectory/aws/aws-sdk-php/compatibility-test.php
   (Default pathToVendorDirectory is sites/all/vendor)

Providing own AWS SDK for PHP service plugin
---------------------------------------------

1. Implements hook_ctools_plugin_directory().

/**
 * Implements hook_ctools_plugin_directory().
 */
function aws_s3_ctools_plugin_directory($module, $plugin) {
  if ($module == 'awssdk2' && $plugin == 'service') {
    return 'plugins/awssdk';
  }
}

2. Create a file called s3.inc inside the directory from step1 with at least this content:

<?php
$plugin = array(
  'title' => t('S3'),
);

* Extended version of this file

<?php
$plugin = array(
  'title' => t('S3'),
  'supported_regions' => 'aws_s3_supported_regions',// See http://docs.aws.amazon.com/general/latest/gr/rande.html
  'service_config' => 'aws_s3_config_form',
);

function aws_s3_supported_regions() {
  $supported = array('IRELAND', 'OREGON');
  return $supported;
}

function aws_s3_config_form(&$config, &$form_state) {
  $item = &$form_state['item'];
  $config['my_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $item->config['my_value'],
  );
}

3. Loading own configuration
  $data = awssdk_service_load('s3');

  debug($data->config['my_value']);
  
4. Using AWS SDK for PHP v2 API

$client = awssdk_get_client('s3');
if (!empty($client)) {
  debug($client);
}