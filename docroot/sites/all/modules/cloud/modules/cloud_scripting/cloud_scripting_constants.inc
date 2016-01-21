<?php

/**
 * @file
 * Provides scripting feature such as bash, Perl and etc (Mainly bash scripts).
 * Works with Cloud and Server Templates module.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 *
 */

define('CLOUD_SCRIPTING_PREFIX'               , 'cloud_'                                            );
define('CLOUD_SCRIPTING_INPUT_PARAMETER_TABLE', CLOUD_SCRIPTING_PREFIX . 'scripting_input_parameter');
define('CLOUD_SCRIPTING_TABLE'                , CLOUD_SCRIPTING_PREFIX . 'scripting'                );
define('CLOUD_SCRIPTING_MASTER_TABLE'         , CLOUD_SCRIPTING_PREFIX . 'scripting_master'         );
define('CLOUD_SCRIPTING_INSTANCES_TABLE'      , CLOUD_SCRIPTING_PREFIX . 'scripting_instance'       );
define('CLOUD_SCRIPTING_PAGER_LIMIT'          , 50                                                  );

define ('CLOUD_SCRIPTING_TEMP_DIRECTORY', 'public://cloud');
define('CLOUD_SCRIPTING_PATH'                 , 'design' . CLOUD_PATH_SEPARATOR . 'scripting');
define('CLOUD_SCRIPTING_OUTPUT_LOG_FILE'      , CLOUD_PATH_SEPARATOR . 'script_execution_output.log');

define('CLOUD_SCRIPTING_SCRIPT_READY_FOR_EXECUTION', '0'   );
define('CLOUD_SCRIPTING_SCRIPT_UNDER_EXECUTION'    , '1'   );
define('CLOUD_SCRIPTING_SCRIPT_EXECUTED'           , '2'   );
define('CLOUD_SCRIPTING_FILE_NOT_EXIST'            , '0'   );
define('CLOUD_SCRIPTING_DIRECTORY_NOT_EXIST'       , '0'   );
