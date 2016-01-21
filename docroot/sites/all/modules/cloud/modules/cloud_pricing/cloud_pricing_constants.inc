<?php

/**
 * @file
 * Provides pricing for hourly rate configuration for Billing module.
 * Works with Cloud and Billing module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('CLOUD_PRICING_PREFIX'         , 'cloud_'                                   );
define('CLOUD_PRICING_PATH'           , 'admin/config/pricing'                   );
define('CLOUD_PRICING_INSTANCES_TABLE', CLOUD_PRICING_PREFIX . 'pricing_instances' );
// Numbers [0-9] only. int or float value  say 9,23.45 etc..
define('CLOUD_PRICING_VALID_NUMBER'   , '/^[-+]?\\b[0-9]*\\.?[0-9]+\\b$/'          );
define('CLOUD_PRICING_PAGER_LIMIT'    , 50                                         );
