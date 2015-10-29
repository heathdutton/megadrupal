<?php

/**
 * @file
 * Defines constants for cloud_alerts.*
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('CLOUD_ALERTS_PREFIX'      , 'cloud_'                          );
define('CLOUD_ALERTS_TABLE'       , CLOUD_ALERTS_PREFIX . 'alerts'    );
define('CLOUD_ALERTS_PATH'        , 'design/alerts'                   );
define('CLOUD_ALERTS_VALID_NUMBER', '/^[-+]?\\b[0-9]*\\.?[0-9]+\\b$/' ); // Numbers [0-9] only. int or float value  say 9,23.45 etc..
define('CLOUD_ALERTS_PAGER_LIMIT', 50                                );
