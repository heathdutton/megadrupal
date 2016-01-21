<?php

function smartling_translation_context_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_upload_context_after_content" : {
    "LABEL" : "Upload context after content",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "context", "Smartling" ],
    "REQUIRES" : [ "rules", "smartling_translation_context", "smartling" ],
    "ON" : { "smartling_after_submission_upload_event" : [] },
    "DO" : [
      { "smartling_translation_context_content_upload_action" : { "eid" : "[eid:value]" } }
    ]
  }
}';
  $configs['rules_upload_context_after_content'] = rules_import($rule);
  return $configs;
}