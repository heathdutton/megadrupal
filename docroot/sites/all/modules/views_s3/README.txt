Views S3
========

Project URL: http://drupal.org/project/views_s3

DESCRIPTION
-----------
This module is useful for any kind of integration with the S3 hosting system, and uses the official AWS SDK for PHP library from Amazon through the AWS SDK for PHP module. An example use case would be a private/authenticated video delivery network that stores videos using Amazon's S3 hosting solution.

You can also provide signed URL through CloudFront CDN when available.

REQUIREMENTS
------------
Drupal 7.x with Views 3 installed.

You will also need the AWS SDK for PHP module and the actual library to operate with the S3 service (and CloudFront if necessary).

INSTALLING
----------
  1. To install the module copy the 'views_s3' folder to your module directory or install it using drush. See [how to install modules in D7](http://drupal.org/documentation/install/modules-themes/modules-7).
  2. Go to admin/modules. Enable the module.

CONFIGURING AND USING
---------------------
There are a few options that are configured globally in the module configuration page, the rest of configuration parameters are stored either in the view itself or in the AWS SDK module's configuration form.

  1. Go to admin/structure/views/s3 to enable/disable the debug mode. If the debug mode is enabled you will see notifications via watchdog and dpm's (if devel module is enabled).
  2. Go to admin/config/media/awssdk and fill in your AWS information. If you enter your CloudFront key-pair and *.pem information you will be able to sign URLs using Amazon's CDN.
  3. Create a new view listing Amazon S3 files by going to admin/structure/views/add and selecting "Sow S3 sorted by unsorted".
  4. You need to supply a bucket name before getting its contents. Please open Advanced settings in your newly created view and select your bucket in Query settings under Other section.
  5. Operate normally with your view.

FEATURES
--------
This module is aimed to ease the pain of fetching and listing items from Amazon S3 hosting service. These are the fields you can easily list using views:

  * File path.
  * File size.
  * File folder.
  * File extension.
  * File basename.
  * File hash.
  * File storage class. Either "standard" or "reduced redundancy".
  * Owner name.
  * Owner canonical ID.
  * Signed S3 URL.
  * Signed CloudFront URL.
  * Pluggable actions list.

Besides the fields you can list the module allows you to filter the results and sort them based on several criteria. It also provides a contextual filter (formerly known as argument) to fetch data from a file based on its S3 path (URL encoded).

ACTIONS LIST
------------
You can extend the module's features by providing your custom actions to perform on the results of the view.

To do so you just need to implement hook_views_s3_actions hook and return an array containing your actions as follows:
array(
  'action1' => array(
    '#title' => t('Action 1'),
    '#description' => t('Description for action 1. This is for administrative purposes'),
    '#href' => '[path that handles the action 1]',
    '#options' => 'options for the link render array',
  ),
  'action2' => array(
    '#title' => t('Action 2'),
    '#description' => t('Description for action 2. This is for administrative purposes'),
    '#href' => '[path that handles the action 2]',
    '#theme' => 'theme function to render the action',
  ),
);

Each action consists in a machine name (the key of the array) and a render array. If the #theme property is present an output like the following will be generated, to allow you to define your own theming functions:
array(
  '#theme' => $action['#theme'],
  '#action' => $action,
  '#values' => $variables['values'],
)

If #theme is not defined, the module will assume a link render array.

In the path that handles the action you can provide replacement patterns from your views data. For instance "views-s3-actions/%etag/delete/%bucket" will be replaced by "views-s3-actions/KLHAA123123JAN/delete/mybucket" so you can act on the file.

See views_s3_actions implementation for a more detailed example on how to provide your custom actions and how to interact with them.

THEMING
-------
Theming the results is done the same way you theme your regular views.

You can alter the appearance of the actions list by overriding: 

  * theme_views_s3_actions_default_action($variables) to alter the appearance of an action link.
  * theme_views_s3_actions_list($variables) to alter the appearance of the action list.
