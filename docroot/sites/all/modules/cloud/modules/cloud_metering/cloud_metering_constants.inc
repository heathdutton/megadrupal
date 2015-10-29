<?php

/**
 * @file
 * Contains the constants used by the Cloud Metering module
 * Works with Cloud Metering module
 *
 * Copyright (c) 2011 DOCOMO Innovations, Inc.
 *
 */

// Generic constants
define('CLOUD_METERING_MODULE_NAME'                     , 'cloud_metering');
define('CLOUD_METERING_MODULE_DISPLAY_NAME'             , 'Cloud Metering');

// Database related constants
define('CLOUD_METERING_PREFIX'                          , 'cloud_');
define('CLOUD_METERING_THRESHOLDS_TABLE'                , CLOUD_METERING_PREFIX . 'metering_thresholds');
define('CLOUD_METERING_MAILS_TABLE'                     , CLOUD_METERING_PREFIX . 'metering_mails');
define('CLOUD_METERING_USER_BUDGET_TABLE'               , CLOUD_METERING_PREFIX . 'metering_budget_users');
define('CLOUD_METERING_BUDGET_GROUP_TABLE'              , CLOUD_METERING_PREFIX . 'metering_budget_groups');
define('CLOUD_METERING_INSTANCES_BUDGET_TABLE'          , CLOUD_METERING_PREFIX . 'metering_instances_budget');
define('CLOUD_METERING_TEMPLATES_BUDGET_TABLE'          , CLOUD_METERING_PREFIX . 'metering_templates_budget');
define('CLOUD_METERING_QUEUE_TABLE'                     , CLOUD_METERING_PREFIX . 'metering_queue');

// threshold mail template
define('CLOUD_METERING_DEFAULT_THRESHOLD_MAIL_SUBJECT'  , variable_get('cloud_metering_mail_subject', 'The instance [cloud_metering_instance:instance-name] has crossed the cost threshold!'));
define('CLOUD_METERING_DEFAULT_THRESHOLD_MAIL_BODY'     , variable_get('cloud_metering_mail_template', "Dear [cloud_metering_instance:username],\n\nThe cost for your cloud instance named [cloud_metering_instance:instance-name] has crossed the cost threshold set by you. You should either increase the budget for the instance or terminate it. Please login to your account and take the necessary steps.\n\n-- [site:name]"));

// terminate mail
define('CLOUD_METERING_DEFAULT_TERMINATE_MAIL_SUBJECT'  , variable_get('cloud_metering_terminate_mail_subject', 'The instance [cloud_metering_instance:instance-name] has crossed the allocated budget!'));
define('CLOUD_METERING_DEFAULT_TERMINATE_MAIL_BODY'     , variable_get('cloud_metering_terminate_mail_template', "Dear [cloud_metering_instance:username],\n\nThe cloud instance named [cloud_metering_instance:instance-name] has crossed the budget allocated to it. The instance will automatically terminate after the volumes are backed up.\n\n-- [site:name]"));

define('CLOUD_METERING_DEFAULT_TERMINATE_WARNING_MAIL_SUBJECT'  , variable_get('cloud_metering_terminate_warning_mail_subject', 'Warning! [cloud_metering_instance:instance-name] has crossed the allocated budget.'));
define('CLOUD_METERING_DEFAULT_TERMINATE_WARNING_MAIL_BODY'     , variable_get('cloud_metering_terminate_warning_mail_template', "Dear [cloud_metering_instance:username],\n\nThe cloud instance [cloud_metering_instance:instance-name] has crossed the budget allocated to it. Consider terminating it to keep with your budget.\n\n-- [site:name]"));

// user default mail templates
define('CLOUD_METERING_DEFAULT_USER_MAIL_SUBJECT'       , variable_get('cloud_metering_user_mail_subject', 'The total cost of your cloud instances has gone past your budget!'));
define('CLOUD_METERING_DEFAULT_USER_MAIL_BODY'          , variable_get('cloud_metering_user_mail_template', "Dear [cloud_metering_user:username],\n\nThe total cost of your cloud instances has gone past your budget of [cloud_metering_user:user_budget]. It is currently costing [cloud_metering_user:user_cost].  Please consider terminating some instances or have the adminstrator increase your budget.\n\n-- [site:name]"));

// group default mail templates
define('CLOUD_METERING_DEFAULT_GROUP_MAIL_SUBJECT'      , variable_get('cloud_metering_group_mail_subject', "Cloud instances has gone past the group budget!"));
define('CLOUD_METERING_DEFAULT_GROUP_MAIL_BODY'         , variable_get('cloud_metering_group_mail_template', "Dear [cloud_metering_group:username],\n\nThe total cost of your group's cloud instances has gone past the allocated budget of [cloud_metering_group:group_budget]. It is currently [cloud_metering_group:group_cost].  Please consider terminating some instances or increasing the group's budget.\n\n-- [site:name]"));

define('CLOUD_METERING_MAIL_INTERVAL'                   , 604800); // 7 days in seconds
define('CLOUD_METERING_THRESHOLD_MAIL_INTERVAL'         , 3600);  //1 hour for threshold emails
define('CLOUD_METERING_TERMINATE_WARNING_INTERVAL'      , 600);

// Queue related constants
define('CLOUD_METERING_QUEUE'                           , 'cloud_metering_queue');

define('CLOUD_METERING_QUEUE_STATUS_INITIATED'          , 'initiated');
define('CLOUD_METERING_QUEUE_STATUS_IN_PROGRESS'        , 'in progress');
define('CLOUD_METERING_QUEUE_STATUS_COMPLETED'          , 'completed');
define('CLOUD_METERING_QUEUE_STATUS_FAILED'             , 'failed');
define('CLOUD_METERING_QUEUE_STATUS_WAITING'            , 'waiting');

define('CLOUD_METERING_BACKUP'                          , 'backup');
define('CLOUD_METERING_BACKUP_CHECK'                    , 'backup check');
define('CLOUD_METERING_DETACH'                          , 'detach');
define('CLOUD_METERING_DETACH_CHECK'                    , 'detach check');
define('CLOUD_METERING_TERMINATE'                       , 'terminate');
define('CLOUD_METERING_TERMINATE_CHECK'                 , 'terminate check');
