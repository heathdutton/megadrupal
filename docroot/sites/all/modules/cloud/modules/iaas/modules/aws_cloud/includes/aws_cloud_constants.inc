<?php

/**
 * @file
 * Constants related to aws_cloud.*
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

// updated by yas 2015/05/06

define('AWS_CLOUD_CONTEXT'                 , 'aws_cloud');
define('AWS_CLOUD_DETAILS_TABLE'           , 'cloud_aws_cloud_details');
define('AWS_CLOUD_AMAZON_INSTANCE_NAME'    , 'Amazon');
define('AWS_CLOUD_OPENSTACK_INSTANCE_NAME' , 'OpenStack');
define('AWS_CLOUD_EUCALYPTUS_INSTANCE_NAME', 'Eucalyptus');
define('AWS_CLOUD_CLOUDN_INSTANCE_NAME'    , 'Cloud n');
define('AWS_CLOUD_LIST_PAGE'               , 'admin/config/clouds');
define('AWS_CLOUD_ADD_PAGE'                , 'admin/config/clouds/add');
define('AWS_CLOUD_MODULE_NAME'             , 'aws_cloud');
define('AWS_CLOUD_OPENSTACK_NOVA_CONTEXT'  , 'openstack_nova');
define('AWS_CLOUD_CLOUDN_CONTEXT'          , 'cloudn');

// AWS Constants
define('AWS_CLOUD_CLOUD_PREFIX', 'cloud_');
define('AWS_CLOUD_BUNDLE_INSTANCES_TABLE', AWS_CLOUD_CLOUD_PREFIX . 'bundle_instances' );
define('AWS_CLOUD_BUNDLE_IMAGE_SCRIPT', 'BUNDLE_IMAGE_SCRIPT');
define('AWS_CLOUD_DB_CONTEXT', 'aws');
define('AWS_CLOUD_CLOUD_NONE', '- none -');
define('AWS_CLOUD_SEED', 'Your favorite word here');
define('AWS_CLOUD_TEMP_DIR', 'public://cloud');
define('AWS_CLOUD_INSTANCES_TABLE', 'instances');
define('AWS_CLOUD_INSTANCES_LOCK_TABLE', 'instances_lock');
define('AWS_CLOUD_IMAGES_TABLE', 'images');
define('AWS_CLOUD_SSH_KEYS_TABLE', 'ssh_keys');
define('AWS_CLOUD_SECURITY_GROUPS_TABLE', 'security_groups');
define('AWS_CLOUD_SECURITY_GROUPS_DETAILS_TABLE' , 'security_groups_details');
define('AWS_CLOUD_ZONE_TABLE', 'zone');
define('AWS_CLOUD_ELASTIC_IP_TABLE', 'elastic_ip');
define('AWS_CLOUD_SNAPSHOTS_TABLE', 'snapshots');
define('AWS_CLOUD_EBS_VOLUMES_TABLE', 'ebs_volumes');
define('AWS_CLOUD_VOLUMES_DEVICE_OPTIONS_TABLE', 'ebs_volumes_device_options');
define('AWS_CLOUD_USER_KEYS_TABLE', 'user_keys');
define('AWS_CLOUD_REGISTER_IMAGE_STATUS_TABLE', 'register_image_status');
define('AWS_CLOUD_SSH_CLIENT_JAR_FILE', 'mindterm.jar');
define('AWS_CLOUD_API_VERSION', '2012-06-01');
define('AWS_CLOUD_HOST_URI', 'ec2.amazonaws.com');
define('AWS_CLOUD_IMAGE_UPLOAD_URL', 'https://s3.amazonaws.com');
define('AWS_CLOUD_IMAGE_REGISTER_URL', 'https://ec2.amazonaws.com');
define('AWS_CLOUD_PAGER_LIMIT', 50);
define('AWS_CLOUD_TIME_LIMIT', 1000);
