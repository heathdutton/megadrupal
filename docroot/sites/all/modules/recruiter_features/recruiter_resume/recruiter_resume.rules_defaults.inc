<?php
/**
 * @file
 * recruiter_resume.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function recruiter_resume_default_rules_configuration() {
  $items = array();
  $items['rules_link_condition_resume_init_visible'] = entity_import('rules_config', '{ "rules_link_condition_resume_init_visible" : {
      "LABEL" : "Rules link: Resume init visible condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "recruiter_resume" ],
      "USES VARIABLES" : { "profile2" : { "label" : "profile2", "type" : "profile2" } },
      "AND" : [
        { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } },
        { "recruiter_resume_complete" : { "profile" : [ "profile2" ] } },
        { "data_is" : { "data" : [ "site:current-user" ], "value" : [ "profile2:user" ] } },
        { "data_is" : { "data" : [ "profile2:field-resume-state" ], "value" : "hidden" } }
      ]
    }
  }');
  $items['rules_link_condition_resume_set_hidden'] = entity_import('rules_config', '{ "rules_link_condition_resume_set_hidden" : {
      "LABEL" : "Rules link: Resume set hidden condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_resume" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "profile2" : { "label" : "profile2", "type" : "profile2" } },
      "AND" : [
        { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } },
        { "data_is" : { "data" : [ "profile2:field-resume-state" ], "value" : "visible" } },
        { "OR" : [
            { "data_is" : { "data" : [ "site:current-user" ], "value" : [ "profile2:user" ] } },
            { "user_has_role" : {
                "account" : [ "site:current-user" ],
                "roles" : { "value" : { "30037204" : "30037204" } }
              }
            }
          ]
        }
      ]
    }
  }');
  $items['rules_link_condition_resume_set_visible'] = entity_import('rules_config', '{ "rules_link_condition_resume_set_visible" : {
      "LABEL" : "Rules link: Resume set visible condition",
      "PLUGIN" : "and",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_resume" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "profile2" : { "label" : "profile2", "type" : "profile2" } },
      "AND" : [
        { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } },
        { "OR" : [
            { "data_is" : { "data" : [ "profile2:field-resume-state" ], "value" : "hidden" } },
            { "data_is" : { "data" : [ "profile2:field-resume-state" ], "value" : "auto_hidden" } }
          ]
        },
        { "OR" : [
            { "data_is" : { "data" : [ "site:current-user" ], "value" : [ "profile2:user" ] } },
            { "user_has_role" : {
                "account" : [ "site:current-user" ],
                "roles" : { "value" : { "30037204" : "30037204" } }
              }
            }
          ]
        }
      ]
    }
  }');
  $items['rules_link_set_resume_init_visible'] = entity_import('rules_config', '{ "rules_link_set_resume_init_visible" : {
      "LABEL" : "Rules link: Resume init visible rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "profile2" : { "type" : "profile2", "label" : "profile2" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [ { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } } ],
            "DO" : [
              { "redirect" : { "url" : "resume" } },
              { "data_set" : { "data" : [ "profile2:field-resume-state" ], "value" : "visible" } }
            ],
            "LABEL" : "Resume init visible"
          }
        }
      ]
    }
  }');
  $items['rules_link_set_resume_set_hidden'] = entity_import('rules_config', '{ "rules_link_set_resume_set_hidden" : {
      "LABEL" : "Rules link: Resume set hidden rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_resume" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "profile2" : { "label" : "profile2", "type" : "profile2" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [ { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } } ],
            "DO" : [
              { "data_set" : { "data" : [ "profile2:field-resume-state" ], "value" : "hidden" } },
              { "drupal_message" : { "message" : "Your resume has been hidden." } }
            ],
            "LABEL" : "Resume set hidden"
          }
        }
      ]
    }
  }');
  $items['rules_link_set_resume_set_visible'] = entity_import('rules_config', '{ "rules_link_set_resume_set_visible" : {
      "LABEL" : "Rules link: Resume set visible rules set",
      "PLUGIN" : "rule set",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_resume" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "profile2" : { "label" : "profile2", "type" : "profile2" } },
      "RULES" : [
        { "RULE" : {
            "IF" : [ { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } } ],
            "DO" : [
              { "data_set" : { "data" : [ "profile2:field-resume-state" ], "value" : "visible" } },
              { "drupal_message" : { "message" : "Your resume is active now." } }
            ],
            "LABEL" : "Resume set visible"
          }
        }
      ]
    }
  }');
  $items['rules_resume_initialization'] = entity_import('rules_config', '{ "rules_resume_initialization" : {
      "LABEL" : "Resume initialization",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_resume" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [], "user_update" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "30037204" : "30037204", "115362423" : "115362423" } },
            "operation" : "OR"
          }
        },
        { "data_is_empty" : { "data" : [ "account:profile-resume" ] } }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "profile2",
              "param_type" : "resume",
              "param_user" : [ "account" ]
            },
            "PROVIDE" : { "entity_created" : { "resume" : "Resume" } }
          }
        },
        { "entity_save" : { "data" : [ "resume" ], "immediate" : 1 } }
      ]
    }
  }');
  $items['rules_resume_set_label'] = entity_import('rules_config', '{ "rules_resume_set_label" : {
      "LABEL" : "Resume set label",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "recruiter_resume" ],
      "REQUIRES" : [ "rules", "profile2" ],
      "ON" : { "profile2_update" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "profile2:type" ], "value" : "resume" } },
        { "NOT data_is_empty" : { "data" : [ "profile2:field-resume-job-preferences" ] } },
        { "NOT data_is" : {
            "data" : [ "profile2:label" ],
            "value" : [ "profile2:field-resume-job-preferences:field-resume-job-title" ]
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "profile2:label" ],
            "value" : [ "profile2:field-resume-job-preferences:field-resume-job-title" ]
          }
        }
      ]
    }
  }');
  return $items;
}
