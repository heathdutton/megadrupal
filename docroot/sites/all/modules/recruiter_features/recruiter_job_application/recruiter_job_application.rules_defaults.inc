<?php
/**
 * @file
 * recruiter_job_application.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function recruiter_job_application_default_rules_configuration() {
  $items = array();
  $items['rules_job_application_rules'] = entity_import('rules_config', '{ "rules_job_application_rules" : {
      "LABEL" : "Job Application Rules",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "job_application" ],
      "REQUIRES" : [ "rules", "content_access_rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "job_application" : "job_application" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "node:field-job-application-job-ref" ] } },
        { "node_is_of_type" : {
            "node" : [ "node:field-job-application-job-ref" ],
            "type" : { "value" : {
                "job_per_file" : "job_per_file",
                "job_per_link" : "job_per_link",
                "job_per_template" : "job_per_template"
              }
            }
          }
        },
        { "NOT user_has_role" : {
            "account" : [ "node:field-job-application-job-ref:author" ],
            "roles" : { "value" : { "1" : "1" } }
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "node:field-job-application-job-ref:author:mail" ],
            "subject" : "New application for your job offer [node:field-job-application-job-ref:title]",
            "message" : "You have received a new application for the job offering [node:field-job-application-job-ref:title] at [node:field-job-application-job-ref:url]. You can read the application under: [node:url]. \\r\\n\\r\\nIn order to see the application you need to login with your username [node:field-job-application-job-ref:author:name].",
            "language" : [ "" ]
          }
        },
        { "component_rules_send_additional_job_application_mail" : { "node" : [ "node" ] } },
        { "content_access_action_user_grant" : {
            "node" : [ "node" ],
            "content_access_user_view" : [ "node:field-job-application-job-ref:author" ],
            "content_access_user_update" : [ "" ],
            "content_access_user_delete" : [ "" ]
          }
        },
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "job_application_recruiter",
              "param_user" : [ "node:author" ]
            },
            "PROVIDE" : { "entity_created" : { "new_message" : "New message" } }
          }
        },
        { "data_set" : {
            "data" : [ "new-message:field-message-audience:0" ],
            "value" : [ "node:field-job-application-job-ref:author" ]
          }
        },
        { "data_set" : { "data" : [ "new-message:field-job-application" ], "value" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_link_condition_job_application_reject'] = entity_import('rules_config', '{ "rules_link_condition_job_application_reject" : {
      "LABEL" : "Rules link: Job application reject condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "type" : "node", "label" : "node" } },
      "AND" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "job_application" : "job_application" } }
          }
        },
        { "NOT data_is" : { "data" : [ "node:field-job-application-state" ], "value" : "rejected" } },
        { "data_is" : {
            "data" : [ "node:field-job-application-job-ref:author" ],
            "value" : [ "site:current-user" ]
          }
        }
      ]
    }
  }');
  $items['rules_link_condition_job_application_shortlist'] = entity_import('rules_config', '{ "rules_link_condition_job_application_shortlist" : {
      "LABEL" : "Rules link: Job application shortlist condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "type" : "node", "label" : "node" } },
      "AND" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "job_application" : "job_application" } }
          }
        },
        { "NOT data_is" : {
            "data" : [ "node:field-job-application-state" ],
            "value" : "shortlisted"
          }
        },
        { "data_is" : {
            "data" : [ "node:field-job-application-job-ref:author" ],
            "value" : [ "site:current-user" ]
          }
        }
      ]
    }
  }');
  $items['rules_link_set_job_application_reject'] = entity_import('rules_config', '{ "rules_link_set_job_application_reject" : {
      "LABEL" : "Rules link: Job application reject rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "type" : "node", "label" : "node" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "node_is_of_type" : {
                  "node" : [ "node" ],
                  "type" : { "value" : { "job_application" : "job_application" } }
                }
              }
            ],
            "DO" : [
              { "data_set" : { "data" : [ "node:field-job-application-state" ], "value" : "rejected" } },
              { "drupal_message" : { "message" : "The application has been rejected." } }
            ],
            "LABEL" : "Set to rejected"
          }
        },
        { "RULE" : {
            "PROVIDE" : { "entity_created" : { "message" : "Message" } },
            "IF" : [
              { "node_is_of_type" : {
                  "node" : [ "node" ],
                  "type" : { "value" : { "job_application" : "job_application" } }
                }
              }
            ],
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "message",
                    "param_type" : "job_application_rejected",
                    "param_user" : [ "node:field-job-application-job-ref:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "message" : "Message" } }
                }
              },
              { "data_set" : {
                  "data" : [ "message:field-message-audience:0" ],
                  "value" : [ "node:author" ]
                }
              },
              { "data_set" : { "data" : [ "message:field-job-application" ], "value" : [ "node" ] } },
              { "data_set" : {
                  "data" : [ "message:arguments:reason" ],
                  "value" : [ "node:field-job-application-rej-reason:name" ]
                }
              },
              { "data_set" : {
                  "data" : [ "message:arguments:reasonmsg" ],
                  "value" : [ "node:field-job-application-rej-msg" ]
                }
              }
            ],
            "LABEL" : "Write message log"
          }
        }
      ]
    }
  }');
  $items['rules_link_set_job_application_shortlist'] = entity_import('rules_config', '{ "rules_link_set_job_application_shortlist" : {
      "LABEL" : "Rules link: Job application shortlist rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "type" : "node", "label" : "node" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [
              { "node_is_of_type" : {
                  "node" : [ "node" ],
                  "type" : { "value" : { "job_application" : "job_application" } }
                }
              }
            ],
            "DO" : [
              { "data_set" : {
                  "data" : [ "node:field-job-application-state" ],
                  "value" : "shortlisted"
                }
              },
              { "drupal_message" : { "message" : "The application has been shortlisted." } }
            ],
            "LABEL" : "Set to shortlisted"
          }
        },
        { "RULE" : {
            "PROVIDE" : { "entity_created" : { "message" : "Message" } },
            "IF" : [
              { "node_is_of_type" : {
                  "node" : [ "node" ],
                  "type" : { "value" : { "job_application" : "job_application" } }
                }
              }
            ],
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "message",
                    "param_type" : "job_application_shortlisted",
                    "param_user" : [ "node:field-job-application-job-ref:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "message" : "Message" } }
                }
              },
              { "data_set" : {
                  "data" : [ "message:field-message-audience:0" ],
                  "value" : [ "node:author" ]
                }
              },
              { "data_set" : { "data" : [ "message:field-job-application" ], "value" : [ "node" ] } }
            ],
            "LABEL" : "Write message log"
          }
        }
      ]
    }
  }');
  $items['rules_send_additional_job_application_mail'] = entity_import('rules_config', '{ "rules_send_additional_job_application_mail" : {
      "LABEL" : "Send additional job application mail",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "job_application" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "application node", "type" : "node" } },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "job_application" : "job_application" } }
          }
        },
        { "node_is_of_type" : {
            "node" : [ "node:field-job-application-job-ref" ],
            "type" : { "value" : {
                "job_per_file" : "job_per_file",
                "job_per_link" : "job_per_link",
                "job_per_template" : "job_per_template"
              }
            }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "node:field-job-application-job-ref:field-job-email" ] } },
        { "NOT data_is" : {
            "data" : [ "node:field-job-application-job-ref:field-job-email" ],
            "value" : [ "node:field-job-application-job-ref:author:mail" ]
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "node:field-job-application-job-ref:field-job-email" ],
            "subject" : "New application for your job offer [node:field-job-application-job-ref:title]",
            "message" : "You have received a new application for the job offering [node:field-job-application-job-ref:title] at [node:field-job-application-job-ref:url]. You can read the application under: [node:url]. \\r\\n\\r\\nIn order to see the application you need to login with your username [node:field-job-application-job-ref:author:name].",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
