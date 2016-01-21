<?php

/**
 * @file
 * Provides cluster feature which enables to bundle server templates.
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('CLOUD_CLUSTER_PREFIX'      , 'cloud_'                                );
define('CLOUD_CLUSTER_PATH'        , 'design/cluster'                        );
define('CLOUD_CLUSTER_DISPLAY_NAME', 'Cluster'                               );

define('CLOUD_CLUSTER_TABLE'       , CLOUD_CLUSTER_PREFIX . 'cluster'        );
define('CLOUD_CLUSTER_SERVER_TABLE', CLOUD_CLUSTER_PREFIX . 'cluster_servers');
define('CLOUD_CLUSTER_PAGER_LIMIT', 50                                      );
