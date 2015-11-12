<?php

/**
 * @file
 * Provides failover feature.
 * Works with Cloud, Cluster and Scripting module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */


define('CLOUD_FAILOVER_DRUPAL_USERS_TABLE', 'users'                                    );
define('CLOUD_FAILOVER_PREFIX'            , 'cloud_'                                   );
define('CLOUD_FAILOVER_MODULE_NAME'       , 'cloud_failover'                           );
define('CLOUD_FAILOVER_ACTIONS_TABLE'     , CLOUD_FAILOVER_PREFIX . 'failover_actions' );

define('CLOUD_FAILOVER_INFO_TABLE'        , CLOUD_FAILOVER_PREFIX . 'failover_info'    );
define('CLOUD_FAILOVER_COMMANDS_TABLE'    , CLOUD_FAILOVER_PREFIX . 'scripting');
define('CLOUD_FAILOVER_SCENARIO_TABLE'    , CLOUD_FAILOVER_PREFIX . 'failover_scenario');
define('CLOUD_FAILOVER_PROTOCOL_TABLE'    , CLOUD_FAILOVER_PREFIX . 'failover_protocol');
define('CLOUD_FAILOVER_SCENARIO_DEPLOYMENT_SERVER_TABLE', CLOUD_FAILOVER_PREFIX . 'failover_scenario_cluster_server'         );
define('CLOUD_FAILOVER_HOST_MONITORING_NAGIOS_LOGS'      , CLOUD_FAILOVER_PREFIX . 'failover_host_monitoring_nagios_logs'   );
define('CLOUD_FAILOVER_SERVICE_MONITORING_NAGIOS_LOGS'   , CLOUD_FAILOVER_PREFIX . 'failover_service_monitoring_nagios_logs');

define('CLOUD_FAILOVER_PAGER_LIMIT'                           , 50                         );
define('CLOUD_FAILOVER_TIME_CHECK'                            , '120'                      ); //Time in seconds
define('CLOUD_FAILOVER_FLAG_NO_FAILOVER'                      , 'Disabled'                 );
define('CLOUD_FAILOVER_FLAG_INITIATED'                        , 'fo_initiated'             );
define('CLOUD_FAILOVER_FLAG_SCRIPT_EXEC_IN_PROGRESS'          , 'sc_execution'             );
define('CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT'            , 'eip_assignment'           );



//define('CLOUD_FAILOVER_FLAG_ELASTIC_IP_ASSIGNMENT_IN_PROGRESS', 'eip_assignment');
define('CLOUD_FAILOVER_FLAG_READY_FOR_CHECK'                  , 'Enabled'       );

define('CLOUD_FAILOVER_SERVICE_FAILURE'                       , '16'            );
define('CLOUD_FAILOVER_SERVICE_OK'                            , ''              );



define('CLOUD_FAILOVER_PATH', 'design/failover');
