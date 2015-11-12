<?php

/**
 * @file
 * Provides server template functionality for each cloud sub-system.
 * Works with Cloud module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('CLOUD_SERVER_TEMPLATES_PREFIX'                      , 'cloud_'                                                             );
define('CLOUD_SERVER_TEMPLATES_NONE'                        , '- none -'                                                            );
define('CLOUD_SERVER_TEMPLATES_TABLE'                       , CLOUD_SERVER_TEMPLATES_PREFIX . 'server_templates'                    );
define('CLOUD_SERVER_TEMPLATES_SCRIPTS_TABLE'               , CLOUD_SERVER_TEMPLATES_PREFIX . 'server_templates_scripts'            );
define('CLOUD_SERVER_TEMPLATES_ALERTS_TABLE'                , CLOUD_SERVER_TEMPLATES_PREFIX . 'server_templates_alerts'             );

define('CLOUD_SERVER_TEMPLATES_INSTANCE_INFO_TABLE'         , CLOUD_SERVER_TEMPLATES_PREFIX . 'server_templates_instance_info'      );
define('CLOUD_SERVER_TEMPLATES_FAILOVER_SCENARIOS_TABLE'    , CLOUD_SERVER_TEMPLATES_PREFIX . 'server_templates_failover_scenarios' );

define('CLOUD_SERVER_TEMPLATES_INSTANCE_TYPE_TABLE'         , CLOUD_SERVER_TEMPLATES_PREFIX . 'instance_types'                      );
define('CLOUD_SERVER_TEMPLATES_PATH'                        , 'design/server_templates'                                             );
define('CLOUD_SERVER_TEMPLATES_PAGER_LIMIT'                 , 50                                                                    );
define('CLOUD_SERVER_TEMPLATES_SCRIPT_ROOT'                 , drupal_get_path('module', 'cloud')
                                                              . CLOUD_PATH_SEPARATOR
                                                              . 'scripting/'                                                        );
// Not empty.
define('CLOUD_SERVER_TEMPLATES_VALID_NOT_EMPTY'    , '/[^\s]+/m'                                                                    );
