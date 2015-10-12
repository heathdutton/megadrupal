<?php

/**
 * @file
 * Contains default rule configurations for the Rules FTP module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rules_ftp_default_rules_configuration() {
  $items = array();

  $items['rules_ftp_send_file_to_ftp_server'] = entity_import('rules_config', '{ "rules_ftp_send_file_to_ftp_server" : {
      "LABEL" : "Send file to FTP server",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Rules FTP" ],
      "USES VARIABLES" : { "file" : { "label" : "File", "type" : "file" } },
      "ACTION SET" : [
        { "rules_ftp_put" : {
            "host" : "example.com",
            "ssl" : 1,
            "username" : "user",
            "password" : "password",
            "file" : [ "file" ]
          }
        }
      ]
    }
  }');

  return $items;
}

/**
 * @}
 */
