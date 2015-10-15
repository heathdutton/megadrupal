<?php
/**
 * @file
 * deploy_addon_content.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function deploy_addon_content_default_rules_configuration() {
  $items = array();
  $items['deploy_addon_content_add_file_to_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_add_file_to_push_to_next_deploy_plan" : {
      "LABEL" : "Add File to Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "file" : { "label" : "File", "type" : "file" } },
      "ACTION SET" : [
        { "deploy_rules_action_add_to_managed_plan" : { "plan_name" : "push_to_next", "entity" : [ "file" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_add_file_to_user_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_add_file_to_user_deploy_plan" : {
      "LABEL" : "Add File to User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "file" : { "label" : "File", "type" : "file" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_rules_action_add_to_managed_plan" : { "entity" : [ "file" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_add_user_to_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_add_user_to_push_to_next_deploy_plan" : {
      "LABEL" : "Add User to Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "user" : { "label" : "User", "type" : "user" } },
      "ACTION SET" : [
        { "deploy_rules_action_add_to_managed_plan" : { "plan_name" : "push_to_next", "entity" : [ "user" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_add_user_to_user_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_add_user_to_user_deploy_plan" : {
      "LABEL" : "Add User to User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "user" : { "label" : "User", "type" : "user" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_rules_action_add_to_managed_plan" : { "entity" : [ "user" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_remove_file_from_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_remove_file_from_push_to_next_deploy_plan" : {
      "LABEL" : "Remove File from Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "file" : { "label" : "File", "type" : "file" } },
      "ACTION SET" : [
        { "deploy_manager_action_delete_from_plan" : { "plan_name" : "push_to_next", "entity" : [ "file" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_remove_file_from_user_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_remove_file_from_user_deploy_plan" : {
      "LABEL" : "Remove File from User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "file" : { "label" : "File", "type" : "file" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_manager_action_delete_from_plan" : { "entity" : [ "file" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_remove_user_from_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_remove_user_from_push_to_next_deploy_plan" : {
      "LABEL" : "Remove User from Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "user" : { "label" : "User", "type" : "user" } },
      "ACTION SET" : [
        { "deploy_manager_action_delete_from_plan" : { "plan_name" : "push_to_next", "entity" : [ "user" ] } }
      ]
    }
  }');
  $items['deploy_addon_content_remove_user_from_user_deploy_plan'] = entity_import('rules_config', '{ "deploy_addon_content_remove_user_from_user_deploy_plan" : {
      "LABEL" : "Remove User from User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "user" : { "label" : "user", "type" : "user" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_manager_action_delete_from_plan" : { "entity" : [ "user" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_add_block_to_ad_hoc_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_add_block_to_ad_hoc_deploy_plan" : {
      "LABEL" : "Add Block to User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "block" : { "label" : "Block", "type" : "bean" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_rules_action_add_to_managed_plan" : { "entity" : [ "block" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_add_block_to_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_add_block_to_push_to_next_deploy_plan" : {
      "LABEL" : "Add Block to Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "block" : { "label" : "Block", "type" : "bean" } },
      "ACTION SET" : [
        { "deploy_rules_action_add_to_managed_plan" : { "plan_name" : "push_to_next", "entity" : [ "block" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_add_node_to_ad_hoc_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_add_node_to_ad_hoc_deploy_plan" : {
      "LABEL" : "Add Node to User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_rules_action_add_to_managed_plan" : { "entity" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_add_node_to_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_add_node_to_push_to_next_deploy_plan" : {
      "LABEL" : "Add Node to Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "deploy_rules_action_add_to_managed_plan" : { "plan_name" : "push_to_next", "entity" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_add_term_to_ad_hoc_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_add_term_to_ad_hoc_deploy_plan" : {
      "LABEL" : "Add Term to User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "term" : { "label" : "Term", "type" : "taxonomy_term" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_rules_action_add_to_managed_plan" : { "entity" : [ "term" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_add_term_to_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_add_term_to_push_to_next_deploy_plan" : {
      "LABEL" : "Add Term to Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "term" : { "label" : "Term", "type" : "taxonomy_term" } },
      "ACTION SET" : [
        { "deploy_rules_action_add_to_managed_plan" : { "plan_name" : "push_to_next", "entity" : [ "term" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_remove_block_from_ad_hoc_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_remove_block_from_ad_hoc_deploy_plan" : {
      "LABEL" : "Remove Block from User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "block" : { "label" : "Block", "type" : "bean" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_manager_action_delete_from_plan" : { "entity" : [ "block" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_remove_block_from_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_remove_block_from_push_to_next_deploy_plan" : {
      "LABEL" : "Remove Block from Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "block" : { "label" : "Block", "type" : "bean" } },
      "ACTION SET" : [
        { "deploy_manager_action_delete_from_plan" : { "plan_name" : "push_to_next", "entity" : [ "block" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_remove_node_from_ad_hoc_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_remove_node_from_ad_hoc_deploy_plan" : {
      "LABEL" : "Remove Node from User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_manager_action_delete_from_plan" : { "entity" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_remove_node_from_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_remove_node_from_push_to_next_deploy_plan" : {
      "LABEL" : "Remove Node from Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "deploy_manager_action_delete_from_plan" : { "plan_name" : "push_to_next", "entity" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_remove_term_from_ad_hoc_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_remove_term_from_ad_hoc_deploy_plan" : {
      "LABEL" : "Remove Term from User Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy_adhoc_plan" ],
      "USES VARIABLES" : { "term" : { "label" : "Term", "type" : "taxonomy_term" } },
      "ACTION SET" : [
        { "deploy_adhoc_plan_manager_action_delete_from_plan" : { "entity" : [ "term" ] } }
      ]
    }
  }');
  $items['rules_deploy_addon_remove_term_from_push_to_next_deploy_plan'] = entity_import('rules_config', '{ "rules_deploy_addon_remove_term_from_push_to_next_deploy_plan" : {
      "LABEL" : "Remove Term from Push to Next Deploy Plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "term" : { "label" : "Term", "type" : "taxonomy_term" } },
      "ACTION SET" : [
        { "deploy_manager_action_delete_from_plan" : { "plan_name" : "push_to_next", "entity" : [ "term" ] } }
      ]
    }
  }');
  return $items;
}
