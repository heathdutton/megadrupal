<?php
/**
 * @file
 * clientcert.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function clientcert_default_rules_configuration() {
  $items = array();
  $items['rules_clientcert_item_set'] = entity_import('rules_config', '{ "rules_clientcert_item_set" : {
      "LABEL" : "Clientcert item set (create\\/update)",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules", "clientcert" ],
      "ON" : { "clientcert_verify" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "clientcert-path-argument" ], "value" : "set" } },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "2" : "2" } }
          }
        }
      ],
      "DO" : [
        { "clientcert_item_set" : {
            "USING" : {
              "specialtoken" : 1,
              "uid" : [ "site:current-user:uid" ],
              "title" : "(SERVER:SSL_CLIENT_S_DN_Email)",
              "cert" : "(SERVER:SSL_CLIENT_CERT)",
              "serial" : "(SERVER:SSL_CLIENT_M_SERIAL)",
              "dn_subject" : "(SERVER:SSL_CLIENT_S_DN)",
              "dn_issuer" : "(SERVER:SSL_CLIENT_I_DN)",
              "verify" : "(SERVER:SSL_CLIENT_VERIFY)",
              "status" : 0,
              "verify_active" : "SUCCESS",
              "status_login" : 1,
              "status_encrypt" : 0,
              "status_public" : 0,
              "validity_start" : "(SERVER:SSL_CLIENT_V_START)",
              "validity_end" : "(SERVER:SSL_CLIENT_V_END)"
            },
            "PROVIDE" : { "clientcert_item" : { "clientcert_item" : "Created clientcert item entity." } }
          }
        }
      ]
    }
  }');
  $items['rules_clientcert_login'] = entity_import('rules_config', '{ "rules_clientcert_login" : {
      "LABEL" : "Clientcert_login",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules", "clientcert" ],
      "ON" : { "clientcert_verify" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "clientcert-path-argument" ], "value" : "login" } }
      ],
      "DO" : [
        { "clientcert_items_pem" : {
            "USING" : {
              "specialtoken" : 1,
              "cert" : "(SERVER:SSL_CLIENT_CERT)",
              "uid_token" : "uid"
            },
            "PROVIDE" : {
              "clientcert_items" : { "clientcert_items" : "List of clientcert item entities." },
              "clientcert_items_count" : { "clientcert_items_count" : "The number of retrieved clientcert items." }
            }
          }
        },
        { "component_rules_clientcert_login_mulitiple_user_message" : {
            "clientcert_items" : [ "clientcert_items" ],
            "clientcert_items_count" : [ "clientcert_items_count" ]
          }
        },
        { "component_rules_clientcert_update_data" : {
            "clientcert_items" : [ "clientcert_items" ],
            "clientcert_items_count" : [ "clientcert_items_count" ]
          }
        },
        { "component_rules_clientcert_login_user_directly" : {
            "clientcert_items" : [ "clientcert-items" ],
            "clientcert_items_count" : [ "clientcert-items-count" ]
          }
        },
        { "component_rules_clientcert_login_fail" : { "clientcert_items_count" : [ "clientcert_items_count" ] } }
      ]
    }
  }');
  $items['rules_clientcert_login_fail'] = entity_import('rules_config', '{ "rules_clientcert_login_fail" : {
      "LABEL" : "Clientcert login fail",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "clientcert_items_count" : { "label" : "Clientcert items count", "type" : "integer" } },
      "IF" : [
        { "data_is" : { "data" : [ "clientcert-items-count" ], "value" : "0" } }
      ],
      "DO" : [
        { "drupal_message" : {
            "message" : "No matching AND active clientcert for login.",
            "type" : "warning"
          }
        },
        { "redirect" : { "url" : "\\u003Cfront\\u003E" } }
      ]
    }
  }');
  $items['rules_clientcert_login_mulitiple_user_message'] = entity_import('rules_config', '{ "rules_clientcert_login_mulitiple_user_message_cloned_" : {
      "LABEL" : "Clientcert login mulitiple user message (with direct login link)",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "clientcert_items" : { "label" : "Clientcert items", "type" : "list\\u003Cclientcert\\u003E" },
        "clientcert_items_count" : { "label" : "Clientcert items count", "type" : "integer" }
      },
      "IF" : [
        { "data_is" : { "data" : [ "clientcert-items-count" ], "op" : "\\u003E", "value" : "1" } }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "clientcert-items" ] },
            "ITEM" : { "clientcert_item" : "Clientcert item" },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "user", "id" : [ "clientcert-item:uid" ] },
                  "PROVIDE" : { "entity_fetched" : { "user" : "user" } }
                }
              },
              { "drupal_message" : {
                  "message" : "Login link to: \\u003Ca href=\\u0022\\/clientcert\\/require\\/login?uid=[user:uid]\\u0022\\u003EUsername [user:name]\\u003C\\/a\\u003E",
                  "type" : "warning"
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_clientcert_login_user_directly'] = entity_import('rules_config', '{ "rules_clientcert_login_user_directly" : {
      "LABEL" : "Clientcert login user directly",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules", "gate_rules" ],
      "USES VARIABLES" : {
        "clientcert_items" : { "label" : "Clientcert items", "type" : "list\u003Cclientcert\u003E" },
        "clientcert_items_count" : { "label" : "Clientcert items count", "type" : "integer" }
      },
      "IF" : [
        { "data_is" : { "data" : [ "clientcert-items-count" ], "value" : "1" } },
        { "data_is" : { "data" : [ "clientcert-items:0:status" ], "value" : 1 } }
      ],
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "user", "id" : [ "clientcert-items:0:uid" ] },
            "PROVIDE" : { "entity_fetched" : { "account" : "User Account" } }
          }
        },
        { "gate_rules_login_data_set" : { "uid" : [ "account:uid" ], "access" : 1 } },
        { "gate_rules_user_login_directly" : {
            "USING" : { "user_account" : [ "account" ] },
            "PROVIDE" : { "user_login_report" : { "user_login_report" : "User login directly report." } }
          }
        },
        { "component_rules_clientcert_user_login_directly_success" : {
            "account" : [ "account" ],
            "user_login_directly_report" : [ "user-login-report" ]
          }
        }
      ]
    }
  }');
  $items['rules_clientcert_update_data'] = entity_import('rules_config', '{ "rules_clientcert_update_data" : {
      "LABEL" : "Clientcert update data",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules", "clientcert" ],
      "USES VARIABLES" : {
        "clientcert_items" : { "label" : "Clientcert items", "type" : "list\u003Cclientcert\u003E" },
        "clientcert_items_count" : { "label" : "Clientcert items count", "type" : "integer" }
      },
      "IF" : [
        { "data_is" : { "data" : [ "clientcert-items-count" ], "value" : "1" } },
        { "NOT data_is" : { "data" : [ "clientcert-items:0:status" ], "value" : 1 } }
      ],
      "DO" : [
        { "clientcert_item_set" : {
            "USING" : {
              "specialtoken" : 1,
              "uid" : [ "clientcert-items:0:uid" ],
              "title" : [ "clientcert-items:0:title" ],
              "cert" : [ "clientcert-items:0:cert" ],
              "serial" : "(SERVER:SSL_CLIENT_M_SERIAL)",
              "dn_subject" : "(SERVER:SSL_CLIENT_S_DN)",
              "dn_issuer" : "(SERVER:SSL_CLIENT_I_DN)",
              "verify" : "(SERVER:SSL_CLIENT_VERIFY)",
              "status" : 0,
              "verify_active" : "SUCCESS",
              "status_login" : [ "clientcert-items:0:status-login" ],
              "status_encrypt" : [ "clientcert-items:0:status-encrypt" ],
              "status_public" : [ "clientcert-items:0:status-public" ],
              "validity_start" : "(SERVER:SSL_CLIENT_V_START)",
              "validity_end" : "(SERVER:SSL_CLIENT_V_END)"
            },
            "PROVIDE" : { "clientcert_item" : { "clientcert_item_updated" : "Updated clientcert item entity." } }
          }
        },
        { "drupal_message" : { "message" : "Update of clientcert information. You can try to login again. If this message still appears your certificate cannot be verified." } }
      ]
    }
  }');
  $items['rules_clientcert_user_login_directly_success'] = entity_import('rules_config', '{ "rules_clientcert_user_login_directly_success" : {
      "LABEL" : "Clientcert User login directly success",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Clientcert" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "account" : { "label" : "User account", "type" : "user" },
        "user_login_directly_report" : { "label" : "User login directly report", "type" : "integer" }
      },
      "IF" : [
        { "data_is" : { "data" : [ "user-login-directly-report" ], "value" : "2" } }
      ],
      "DO" : [
        { "drupal_message" : { "message" : "You are successfully logged in with clientcert." } },
        { "redirect" : { "url" : "user" } }
      ]
    }
  }');
  return $items;
}
