<?php
/**
 * @file
 * faktura.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function faktura_default_rules_configuration() {
  $items = array();
  $items['rules_faktura_calculate_invoice_subtotal'] = entity_import('rules_config', '{ "rules_faktura_calculate_invoice_subtotal" : {
      "LABEL" : "Faktura calculate invoice subtotal",
      "PLUGIN" : "rule",
      "TAGS" : [ "Calculation", "Faktura", "Invoice" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "invoice" : "invoice" } } } }
      ],
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "decimal", "value" : "0" },
            "PROVIDE" : { "variable_added" : { "subtotal" : "Subtotal" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-invoice-item" ] },
            "ITEM" : { "current_invoice_position" : "Current invoice position" },
            "DO" : [
              { "data_calc" : {
                  "USING" : {
                    "input_1" : [ "subtotal" ],
                    "op" : "+",
                    "input_2" : [ "current-invoice-position:field-invoice-item-total" ]
                  },
                  "PROVIDE" : { "result" : { "result" : "Result" } }
                }
              },
              { "data_set" : { "data" : [ "subtotal" ], "value" : [ "result" ] } }
            ]
          }
        },
        { "data_set" : { "data" : [ "node:field-invoice-subtotal" ], "value" : [ "subtotal" ] } },
        { "entity_save" : { "data" : [ "node" ], "immediate" : 1 } }
      ],
      "PROVIDES VARIABLES" : [ "node" ]
    }
  }');
  $items['rules_faktura_calculate_invoice_total'] = entity_import('rules_config', '{ "rules_faktura_calculate_invoice_total" : {
      "LABEL" : "Faktura calculate invoice total",
      "PLUGIN" : "rule",
      "TAGS" : [ "Calculation", "Faktura", "Invoice" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "invoice" : "invoice" } } } }
      ],
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "decimal", "value" : [ "node:field-invoice-subtotal" ] },
            "PROVIDE" : { "variable_added" : { "total" : "Total" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-invoice-tax" ] },
            "ITEM" : { "tax_list_item" : "Tax list item" },
            "DO" : [
              { "data_calc" : {
                  "USING" : {
                    "input_1" : [ "total" ],
                    "op" : "+",
                    "input_2" : [ "tax-list-item:field-tax-amount" ]
                  },
                  "PROVIDE" : { "result" : { "result" : "Result" } }
                }
              },
              { "data_set" : { "data" : [ "total" ], "value" : [ "result" ] } }
            ]
          }
        },
        { "data_set" : { "data" : [ "node:field-invoice-total" ], "value" : [ "total" ] } },
        { "entity_save" : { "data" : [ "node" ], "immediate" : 1 } }
      ],
      "PROVIDES VARIABLES" : [ "node" ]
    }
  }');
  $items['rules_faktura_calculate_tax'] = entity_import('rules_config', '{ "rules_faktura_calculate_tax" : {
      "LABEL" : "Faktura calculate tax",
      "PLUGIN" : "rule",
      "TAGS" : [ "Calculation", "Faktura", "Tax" ],
      "REQUIRES" : [ "rules", "faktura" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "invoice" : "invoice" } } } }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-invoice-tax" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [ { "entity_delete" : { "data" : [ "list-item" ] } } ]
          }
        },
        { "faktura_calculate_taxes" : { "node" : [ "node" ] } }
      ],
      "PROVIDES VARIABLES" : [ "node" ]
    }
  }');
  $items['rules_faktura_invoice_calculation'] = entity_import('rules_config', '{ "rules_faktura_invoice_calculation" : {
      "LABEL" : "Faktura invoice calculation",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Calculation", "Faktura", "Invoice" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert", "node_update" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "invoice" : "invoice" } } } }
      ],
      "DO" : [
        { "entity_save" : { "data" : [ "node" ], "immediate" : 1 } },
        { "component_rules_faktura_calculate_tax" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "node" : { "node_taxed" : "Node (taxed)" } }
          }
        },
        { "component_rules_faktura_calculate_invoice_subtotal" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "node" : { "node_subtotal" : "Node (subtotal)" } }
          }
        },
        { "component_rules_faktura_calculate_invoice_total" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "node" : { "node_total" : "Node (total)" } }
          }
        }
      ]
    }
  }');
  $items['rules_get_time_report_values'] = entity_import('rules_config', '{ "rules_get_time_report_values" : {
      "LABEL" : "Get time report values",
      "PLUGIN" : "rule",
      "TAGS" : [ "Hourly wage invoice" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "node" : { "label" : "Node", "type" : "node" },
        "hourly_wage" : { "label" : "Houry wage", "type" : "decimal", "parameter" : false },
        "quantity" : { "label" : "Quantity", "type" : "decimal", "parameter" : false },
        "tax" : { "label" : "Tax", "type" : "node", "parameter" : false },
        "customer" : { "label" : "Customer", "type" : "node", "parameter" : false }
      },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_hourly_wage" } },
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "time_report" : "time_report" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "hourly-wage" ], "value" : [ "node:field-hourly-wage" ] } },
        { "data_set" : { "data" : [ "quantity" ], "value" : [ "node:field-duration" ] } },
        { "data_set" : { "data" : [ "tax" ], "value" : [ "node:field-invoice-item-tax" ] } },
        { "data_set" : { "data" : [ "customer" ], "value" : [ "node:field-customer" ] } }
      ],
      "PROVIDES VARIABLES" : [ "hourly_wage", "quantity", "tax", "customer" ]
    }
  }');
  $items['rules_invoice_to_dunning_letter'] = entity_import('rules_config', '{ "rules_invoice_to_dunning_letter" : {
      "LABEL" : "Invoice to dunning letter",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "invoice" : { "label" : "Invoice", "type" : "node" },
        "dunning_level" : { "label" : "Dunning level", "type" : "node" }
      },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "invoice" ],
            "type" : { "value" : { "invoice" : "invoice" } }
          }
        }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "dunning",
              "param_title" : "[dunning-level:title] - [invoice:field-customer]",
              "param_author" : [ "site:current-user" ]
            },
            "PROVIDE" : { "entity_created" : { "dunning_letter" : "Dunning letter" } }
          }
        },
        { "data_set" : { "data" : [ "dunning-letter:field-invoice" ], "value" : [ "invoice" ] } },
        { "data_set" : {
            "data" : [ "dunning-letter:field-ref-company" ],
            "value" : [ "invoice:field-ref-company" ]
          }
        },
        { "data_set" : {
            "data" : [ "dunning-letter:field-dunning-level" ],
            "value" : [ "dunning-level" ]
          }
        },
        { "data_set" : {
            "data" : [ "dunning-letter:field-customer" ],
            "value" : [ "invoice:field-customer" ]
          }
        }
      ]
    }
  }');
  $items['rules_prefill_invoice'] = entity_import('rules_config', '{ "rules_prefill_invoice" : {
      "LABEL" : "Prefill invoice",
      "PLUGIN" : "action set",
      "TAGS" : [ "Prefill invoice" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "time_reports" : { "label" : "Time reports", "type" : "list\\u003Cnode\\u003E" } },
      "ACTION SET" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "invoice",
              "param_title" : "RULES CREATED INVOICE",
              "param_author" : [ "site:current-user" ]
            },
            "PROVIDE" : { "entity_created" : { "invoice_created" : "Invoice" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "time-reports" ] },
            "ITEM" : { "current_time_report_list_item" : "Current time report" },
            "DO" : [
              { "component_rules_get_time_report_values" : {
                  "USING" : { "node" : [ "current-time-report-list-item" ] },
                  "PROVIDE" : {
                    "hourly_wage" : { "hourly_wage" : "Houry wage" },
                    "quantity" : { "quantity" : "Quantity" },
                    "tax" : { "tax" : "Tax" },
                    "customer" : { "customer" : "Customer" }
                  }
                }
              },
              { "entity_fetch" : {
                  "USING" : {
                    "type" : "node",
                    "id" : [ "current-time-report-list-item:nid" ],
                    "revision_id" : [ "" ]
                  },
                  "PROVIDE" : { "entity_fetched" : { "time_report_entity_fetched" : "Fetched time report entity" } }
                }
              },
              { "entity_create" : {
                  "USING" : {
                    "type" : "field_collection_item",
                    "param_field_name" : "field_invoice_item",
                    "param_host_entity" : [ "invoice-created" ]
                  },
                  "PROVIDE" : { "entity_created" : { "invoice_items_entity_created" : "Invoice items entity" } }
                }
              },
              { "drupal_message" : { "message" : [ "current-time-report-list-item:title" ] } },
              { "data_set" : {
                  "data" : [ "invoice-items-entity-created:field-invoice-item-desc" ],
                  "value" : [ "current-time-report-list-item:body:value" ]
                }
              },
              { "data_set" : {
                  "data" : [ "invoice-items-entity-created:field-invoice-item-unit-price" ],
                  "value" : [ "hourly-wage" ]
                }
              },
              { "data_set" : {
                  "data" : [ "invoice-items-entity-created:field-invoice-item-qty" ],
                  "value" : [ "quantity" ]
                }
              },
              { "data_calc" : {
                  "USING" : { "input_1" : [ "hourly-wage" ], "op" : "*", "input_2" : [ "quantity" ] },
                  "PROVIDE" : { "result" : { "result" : "Calculation result" } }
                }
              },
              { "data_set" : {
                  "data" : [ "invoice-items-entity-created:field-invoice-item-total" ],
                  "value" : [ "result" ]
                }
              },
              { "data_set" : {
                  "data" : [ "invoice-items-entity-created:field-invoice-item-tax" ],
                  "value" : [ "tax" ]
                }
              },
              { "data_set" : {
                  "data" : [ "invoice-created:field-customer" ],
                  "value" : [ "customer" ]
                }
              }
            ]
          }
        },
        { "entity_save" : { "data" : [ "invoice-created" ], "immediate" : 1 } },
        { "redirect" : { "url" : "invoices" } },
        { "component_rules_reference_time_report_to_created_invoice" : {
            "time_reports" : [ "time-reports" ],
            "created_invoice" : [ "invoice-created" ]
          }
        }
      ]
    }
  }');
  $items['rules_reference_time_report_to_created_invoice'] = entity_import('rules_config', '{ "rules_reference_time_report_to_created_invoice" : {
      "LABEL" : "Reference time report to created invoice",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "time_reports" : { "label" : "Time reports", "type" : "list\\u003Cnode\\u003E" },
        "created_invoice" : { "label" : "Created invoice", "type" : "node" }
      },
      "ACTION SET" : [
        { "LOOP" : {
            "USING" : { "list" : [ "time-reports" ] },
            "ITEM" : { "current_time_report_list_item" : "Current time report" },
            "DO" : [
              { "component_rules_set_time_report_status_to_invoiced_selected_invoice_" : {
                  "USING" : {
                    "time_report" : [ "current-time-report-list-item" ],
                    "created_invoice" : [ "created-invoice" ]
                  },
                  "PROVIDE" : { "time_report" : { "time_report_changed" : "Time report changed" } }
                }
              },
              { "entity_save" : { "data" : [ "time-report-changed" ] } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_set_time_report_status_to_invoiced_selected_invoice_'] = entity_import('rules_config', '{ "rules_set_time_report_status_to_invoiced_selected_invoice_" : {
      "LABEL" : "Set time report status to invoiced (selected invoice)",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "time_report" : { "label" : "Time report", "type" : "node" },
        "created_invoice" : { "label" : "Created invoice", "type" : "node" }
      },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "time-report" ],
            "type" : { "value" : { "time_report" : "time_report" } }
          }
        },
        { "node_is_of_type" : {
            "node" : [ "created-invoice" ],
            "type" : { "value" : { "invoice" : "invoice" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "time-report:field-invoiced" ], "value" : 1 } },
        { "data_set" : {
            "data" : [ "time-report:field-invoice" ],
            "value" : [ "created-invoice" ]
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "time_report" ]
    }
  }');
  return $items;
}
