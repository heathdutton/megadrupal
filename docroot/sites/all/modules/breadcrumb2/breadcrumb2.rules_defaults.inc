<?php
/**
 * @file
 * breadcrumb2.rules_defaults.inc
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function breadcrumb2_default_rules_configuration() {
  $items = array();

  $items['rules_append_breadcrumb_trail_base_on_inheritance'] = entity_import('rules_config', '{ "rules_append_breadcrumb_trail_base_on_inheritance" : {
    "LABEL" : "Append breadcrumb trail base on inheritance",
    "PLUGIN" : "action set",
    "REQUIRES" : [ "rules", "breadcrumb2" ],
    "USES VARIABLES" : {
      "inherited_breadcrumb_path" : { "label" : "inherited breadcrumb path", "type" : "text" },
      "breadcrumb" : { "label" : "Breadcrumb", "type" : "breadcrumb2" }
    },
    "ACTION SET" : [
      { "entity_query" : {
          "USING" : {
            "type" : "breadcrumb2",
            "property" : "path",
            "value" : [ "inherited-breadcrumb-path" ],
            "limit" : "1"
          },
          "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "entity-fetched:0:link" ] },
          "ITEM" : { "list_item" : "Current list item" },
          "DO" : [
            { "breadcrumb2_append_breadcrumb_trail" : {
                "breadcrumb" : [ "breadcrumb" ],
                "title" : [ "list-item:title" ],
                "url" : [ "list-item:url" ]
              }
            }
          ]
        }
      }
    ]
  }
}');

  $items['rules_append_breadcrumb_trail_base_on_taxonomy_term'] = entity_import('rules_config', '{ "rules_append_breadcrumb_trail_base_on_taxonomy_term" : {
    "LABEL" : "Append breadcrumb trail base on taxonomy term",
    "PLUGIN" : "action set",
    "REQUIRES" : [ "breadcrumb2", "rules" ],
    "USES VARIABLES" : {
      "breadcrumb" : { "label" : "Breadcrumb", "type" : "breadcrumb2" },
      "taxonomy_term" : { "label" : "Taxonomy term", "type" : "taxonomy_term" }
    },
    "ACTION SET" : [
      { "breadcrumb2_taxonomy_get_parents_all" : {
          "USING" : { "taxonomy_term" : [ "taxonomy_term" ] },
          "PROVIDE" : { "parent_terms" : { "parent_terms" : "Parent terms" } }
        }
      },
      { "LOOP" : {
          "USING" : { "list" : [ "parent-terms" ] },
          "ITEM" : { "list_item" : "Current list item" },
          "DO" : [
            { "breadcrumb2_append_breadcrumb_trail" : {
                "breadcrumb" : [ "breadcrumb" ],
                "title" : [ "list-item:name" ],
                "url" : "taxonomy\/term\/[list-item:tid]"
              }
            }
          ]
        }
      }
    ]
  }
}');

  $items['rules_append_breadcrumb_trail_base_on_menu_trail'] = entity_import('rules_config', '{ "rules_append_breadcrumb_trail_base_on_menu_trail" : {
    "LABEL" : "Append breadcrumb trail base on menu trail",
    "PLUGIN" : "action set",
    "REQUIRES" : [ "breadcrumb2", "rules" ],
    "USES VARIABLES" : { "breadcrumb" : { "label" : "Breadcrumb", "type" : "breadcrumb2" } },
    "ACTION SET" : [
      { "breadcrumb2_menu_get_active_trail" : { "PROVIDE" : { "active_trail" : { "active_trail" : "Active trail" } } } },
      { "list_remove" : { "list" : [ "active-trail" ], "item" : [ "active-trail:0" ] } },
      { "LOOP" : {
          "USING" : { "list" : [ "active-trail" ] },
          "ITEM" : { "list_item" : "Current list item" },
          "DO" : [
            { "breadcrumb2_append_breadcrumb_trail" : {
                "breadcrumb" : [ "breadcrumb" ],
                "title" : [ "list-item:title" ],
                "url" : [ "list-item:href" ]
              }
            }
          ]
        }
      }
    ]
  }
}');
  
  $items['rules_build_breadcrumb_for_node'] = entity_import('rules_config', '{ "rules_build_breadcrumb_for_node" : {
    "LABEL" : "build breadcrumb for node",
    "PLUGIN" : "rule set",
    "REQUIRES" : [ "rules", "breadcrumb2" ],
    "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
    "RULES" : [
      { "RULE" : {
          "PROVIDE" : { "entity_created" : { "breadcrumb" : "Breadcrumb" } },
          "DO" : [
            { "entity_create" : {
                "USING" : { "type" : "breadcrumb2", "param_path" : "node\/[node:nid]" },
                "PROVIDE" : { "entity_created" : { "breadcrumb" : "Breadcrumb" } }
              }
            }
          ],
          "LABEL" : "create breadcrumb"
        }
      },
      { "RULE" : {
          "IF" : [
            { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "article" : "article" } } } }
          ],
          "DO" : [
            { "breadcrumb2_append_breadcrumb_trail" : {
                "breadcrumb" : [ "breadcrumb" ],
                "title" : [ "node:field-tags:0:name" ],
                "url" : "taxonomy\/term\/[node:field-tags:0:tid]"
              }
            }
          ],
          "LABEL" : "build breadcrumb trail for node type article"
        }
      },
      { "RULE" : {
          "IF" : [
            { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "page" : "page" } } } }
          ],
          "DO" : [
            { "component_rules_append_breadcrumb_trail_base_on_menu_trail" : { "breadcrumb" : [ "breadcrumb" ] } }
          ],
          "LABEL" : "build breadcrumb trail for node type page"
        }
      },
      { "RULE" : {
          "PROVIDE" : { "lightest_term" : { "lightest_term" : "First term" } },
          "IF" : [
            { "NOT node_is_of_type" : {
                "node" : [ "node" ],
                "type" : { "value" : { "article" : "article", "page" : "page" } }
              }
            }
          ],
          "DO" : [
            { "breadcrumb2_fetch_lightest_term_from_node" : {
                "USING" : { "node" : [ "node" ] },
                "PROVIDE" : { "lightest_term" : { "lightest_term" : "First term" } }
              }
            },
            { "component_rules_append_breadcrumb_trail_base_on_taxonomy_term" : { "breadcrumb" : [ "breadcrumb" ], "taxonomy_term" : [ "lightest-term" ] } }
          ],
          "LABEL" : "build breadcrumb trail for other node types"
        }
      }
    ]
  }
}');
   
  $items['rules_build_breadcrumb_for_taxonomy_term'] = entity_import('rules_config', '{ "rules_build_breadcrumb_for_taxonomy_term" : {
    "LABEL" : "build breadcrumb for taxonomy term",
    "PLUGIN" : "rule set",
    "REQUIRES" : [ "rules" ],
    "USES VARIABLES" : { "term" : { "label" : "Taxonomy term", "type" : "taxonomy_term" } },
    "RULES" : [
      { "RULE" : {
          "PROVIDE" : { "entity_created" : { "breadcrumb" : "Breadcrumb" } },
          "DO" : [
            { "entity_create" : {
                "USING" : { "type" : "breadcrumb2", "param_path" : "taxonomy\/term\/[term:tid]" },
                "PROVIDE" : { "entity_created" : { "breadcrumb" : "Breadcrumb" } }
              }
            },
            { "component_rules_append_breadcrumb_trail_base_on_taxonomy_term" : { "breadcrumb" : [ "breadcrumb" ], "taxonomy_term" : [ "term" ] } }
          ],
          "LABEL" : "create breadcrumb"
        }
      }
    ]
  }
}');

  $items['rules_build_breadcrumb_at_node_insert'] = entity_import('rules_config', '{ "rules_build_breadcrumb_at_node_insert" : {
    "LABEL" : "build breadcrumb at node insert",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules" ],
    "ON" : [ "node_insert" ],
    "DO" : [
      { "component_rules_build_breadcrumb_for_node" : { "node" : [ "node" ] } }
    ]
  }
}');
  
  $items['rules_build_breadcrumb_at_node_update'] = entity_import('rules_config', '{ "rules_build_breadcrumb_at_node_update" : {
    "LABEL" : "build breadcrumb at node update",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules" ],
    "ON" : [ "node_update" ],
    "DO" : [
      { "component_rules_build_breadcrumb_for_node" : { "node" : [ "node" ] } }
    ]
  }
}');
  
  $items['rules_build_breadcrumb_at_node_view'] = entity_import('rules_config', '{ "rules_build_breadcrumb_at_node_view" : {
    "LABEL" : "build breadcrumb at node view",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "breadcrumb2" ],
    "ON" : [ "node_insert" ],
    "IF" : [
      { "NOT breadcrumb2_breadcrumb_exist" : { "path" : "node\/[node:nid]" } }
    ],
    "DO" : [
      { "component_rules_build_breadcrumb_for_node" : { "node" : [ "node" ] } }
    ]
  }
}');

  $items['rules_build_breadcrumb_at_term_insert'] = entity_import('rules_config', '{ "rules_build_breadcrumb_at_term_insert" : {
    "LABEL" : "build breadcrumb at term insert",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "taxonomy" ],
    "ON" : [ "taxonomy_term_insert" ],
    "DO" : [
      { "component_rules_build_breadcrumb_for_taxonomy_term" : { "term" : [ "term" ] } }
    ]
  }
}');

  $items['rules_build_breadcrumb_at_term_update'] = entity_import('rules_config', '{ "rules_build_breadcrumb_at_term_update" : {
    "LABEL" : "build breadcrumb at term update",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "taxonomy" ],
    "ON" : [ "taxonomy_term_update" ],
    "DO" : [
      { "component_rules_build_breadcrumb_for_taxonomy_term" : { "term" : [ "term" ] } }
    ]
  }
}');
  return $items;
}
