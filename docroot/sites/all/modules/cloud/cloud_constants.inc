<?php

/**
 * @file
 * Defines constants for cloud.module
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


/**
 * @file
 * Provides common functionalites for cloud management.
 */


drupal_set_time_limit(5000);

//project name as a prefix to all project specific tables
define('CLOUD_PREFIX'                 , 'cloud_'                        );
define('CLOUD_NONE'                   , '- none -'                      );
define('CLOUD_CLOUDS_TABLE'           , CLOUD_PREFIX . 'clouds'         );
define('CLOUD_TEMP_TABLE'             , CLOUD_PREFIX . 'temp_table'     );
define('CLOUD_INSTANCE_TYPE_TABLE'    , CLOUD_PREFIX . 'instance_types' );


define('CLOUD_PAGER_LIMIT'            , 50                              );

define('CLOUD_SSH_USER_NAME'          , 'root'                          );

define('CLOUD_RRD_FILE'               , 'rrd.php'                       );
define('CLOUD_REMOTE_SNMP_MANAGER'    , 'cloud_snmp_manager.php'        );

// This Module Name list will be mainly used to check whether the Module is enabled or no.

//for linux
define('CLOUD_PATH_SEPARATOR'           , '/'   );
define('CLOUD_PHP_PATH'                 , 'php' );
define('CLOUD_SSH_PATH'                 , 'ssh' );
define('CLOUD_SCP_PATH'                 , 'scp' );
define('CLOUD_HOST_ENTRIES_REFRESH_TIME', 5     );
define('CLOUD_INPUTS_PARAMETER_VALUES_TABLE', CLOUD_PREFIX . 'inputs_parameter_values' );

define('CLOUD_INSTANCE_STATUS_BOOTING'     , 'booting'     );
define('CLOUD_INSTANCE_STATUS_OPERATIONAL', 'operational' );

/**
 * DIGIT [0-9] only without decimal point. int value only
 */
define('CLOUD_VALID_DIGIT', '/^[0-9]+$/');

define('CLOUD_TIME_LIMIT', 1000);

