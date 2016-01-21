<?php
/**
 * @file
 * commerce_sp_paymill.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function commerce_sp_paymill_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_default_message_type().
 */
function commerce_sp_paymill_default_message_type() {
  $items = array();
  $items['commerce_sp_paymill_webhook_log'] = entity_import('message_type', '{
    "name" : "commerce_sp_paymill_webhook_log",
    "description" : "Commerce SP Paymill Webhook log",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : { "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" } },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "Webhook invoked.",
          "format" : "plain_text",
          "safe_value" : "\\u003Cp\\u003EWebhook invoked.\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}
