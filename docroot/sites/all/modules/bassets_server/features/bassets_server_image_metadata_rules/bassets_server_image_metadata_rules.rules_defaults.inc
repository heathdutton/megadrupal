<?php
/**
 * @file
 * bassets_server_image_metadata_rules.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function bassets_server_image_metadata_rules_default_rules_configuration() {
  $items = array();
  $items['rules_bassets_aspect_ratio'] = entity_import('rules_config', '{ "rules_bassets_aspect_ratio" : {
      "LABEL" : "image_aspect_ratio",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_update" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "entity:field-computed-height" ] } },
        { "NOT data_is_empty" : { "data" : [ "entity:field-computed-width" ] } }
      ],
      "DO" : [
        { "data_convert" : {
            "USING" : {
              "type" : "integer",
              "value" : [ "entity:field-computed-height" ],
              "rounding_behavior" : "up"
            },
            "PROVIDE" : { "conversion_result" : { "height" : "height" } }
          }
        },
        { "data_convert" : {
            "USING" : {
              "type" : "integer",
              "value" : [ "entity:field-computed-width" ],
              "rounding_behavior" : "up"
            },
            "PROVIDE" : { "conversion_result" : { "width" : "Width" } }
          }
        },
        { "data_calc" : {
            "USING" : { "input_1" : [ "width" ], "op" : "\\/", "input_2" : [ "height" ] },
            "PROVIDE" : { "result" : { "width_height_result" : "width_height_result" } }
          }
        },
        { "variable_add" : {
            "USING" : { "type" : "text", "value" : "other" },
            "PROVIDE" : { "variable_added" : { "calculated_ratio" : "calculated_ratio" } }
          }
        },
        { "variable_add" : {
            "USING" : { "type" : "integer", "value" : "4" },
            "PROVIDE" : { "variable_added" : { "4_3_first_value" : "4:3 first value" } }
          }
        },
        { "data_calc" : {
            "USING" : { "input_1" : [ "4-3-first-value" ], "op" : "\\/", "input_2" : "3" },
            "PROVIDE" : { "result" : { "result_4_3" : "Calculation 4:3" } }
          }
        },
        { "variable_add" : {
            "USING" : { "type" : "integer", "value" : "16" },
            "PROVIDE" : { "variable_added" : { "16_9_first_value" : "16:9 first value" } }
          }
        },
        { "data_calc" : {
            "USING" : { "input_1" : [ "16-9-first-value" ], "op" : "\\/", "input_2" : "9" },
            "PROVIDE" : { "result" : { "result_16_9" : "Calculation 16:9" } }
          }
        },
        { "CONDITIONAL" : [
            {
              "IF" : { "data_is" : { "data" : [ "width-height-result" ], "value" : "1.5" } },
              "DO" : [ { "data_set" : { "data" : [ "calculated-ratio" ], "value" : "3:2" } } ]
            },
            {
              "ELSE IF" : { "data_is" : { "data" : [ "width-height-result" ], "value" : "1.6" } },
              "DO" : [
                { "data_set" : { "data" : [ "calculated-ratio" ], "value" : "16:10" } }
              ]
            },
            {
              "ELSE IF" : { "data_is" : { "data" : [ "width-height-result" ], "value" : [ "result-4-3" ] } },
              "DO" : [ { "data_set" : { "data" : [ "calculated-ratio" ], "value" : "4:3" } } ]
            },
            {
              "ELSE IF" : { "data_is" : { "data" : [ "width-height-result" ], "value" : [ "result-16-9" ] } },
              "DO" : [ { "data_set" : { "data" : [ "calculated-ratio" ], "value" : "16:9" } } ]
            }
          ]
        },
        { "data_set" : {
            "data" : [ "entity:field-file-image-aspect-ratio" ],
            "value" : [ "calculated-ratio" ]
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_insert_collection'] = entity_import('rules_config', '{ "rules_bassets_insert_collection" : {
      "LABEL" : "image_insert_collection",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        }
      ],
      "DO" : [
        { "component_rules_bassets_metadata_copyright" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue1" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_datetime" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue2" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_description" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue3" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_photographer" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue4" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_orientation" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue5" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_meteringmode" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue6" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_distancerange" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue7" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_photometric" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue8" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_metadata_colorspace" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue9" : "Continue Rules" } }
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_licence_mapping'] = entity_import('rules_config', '{ "rules_bassets_licence_mapping" : {
      "LABEL" : "bassets_licence_mapping",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_update" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "entity:field-license" ] } }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "entity:field-asset-license" ],
            "value" : [ "entity:field-license:name" ]
          }
        },
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-license:field-license-url" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-license-url" ],
                    "value" : [ "entity:field-license:field-license-url" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_colorspace'] = entity_import('rules_config', '{ "rules_bassets_metadata_colorspace" : {
      "LABEL" : "bassets_metadata_colorspace",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "entity" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-image-colorspace" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-colorspace" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-image-colorspace" ],
                    "value" : [ "entity:field-exif-colorspace" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_copyright'] = entity_import('rules_config', '{ "rules_bassets_metadata_copyright" : {
      "LABEL" : "bassets_metadata_copyright",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-asset-copyright" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-copyright" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-copyright" ],
                    "value" : [ "entity:field-exif-copyright" ]
                  }
                }
              ]
            },
            { "ELSE" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-copyright" ],
                    "value" : [ "entity:field-iptc-copyright-notice" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_datetime'] = entity_import('rules_config', '{ "rules_bassets_metadata_datetime" : {
      "LABEL" : "bassets_metadata_datetime",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional", "bassets_server" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-asset-datetimeoriginal" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-datetimeoriginal" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-datetimeoriginal" ],
                    "value" : [ "entity:field-exif-datetimeoriginal" ]
                  }
                }
              ]
            },
            { "ELSE" : [
                { "CONDITIONAL" : [
                    {
                      "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-iptc-creation-date" ] } },
                      "DO" : [
                        { "CONDITIONAL" : [
                            {
                              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-iptc-creation-time" ] } },
                              "DO" : [
                                { "rip_merge_iptc_creation_date_time" : {
                                    "USING" : {
                                      "iptc_creation_date" : [ "entity:field-iptc-creation-date" ],
                                      "iptc_creation_time" : [ "entity:field-iptc-creation-time" ]
                                    },
                                    "PROVIDE" : { "iptc_new_date" : { "iptc_new_date" : "Merged IPTC creation date\\/time" } }
                                  }
                                },
                                { "CONDITIONAL" : [
                                    {
                                      "IF" : { "NOT data_is_empty" : { "data" : [ "iptc-new-date" ] } },
                                      "DO" : [
                                        { "data_set" : {
                                            "data" : [ "entity:field-asset-datetimeoriginal" ],
                                            "value" : [ "iptc-new-date" ]
                                          }
                                        }
                                      ]
                                    }
                                  ]
                                }
                              ]
                            }
                          ]
                        }
                      ]
                    }
                  ]
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_description'] = entity_import('rules_config', '{ "rules_bassets_metadata_description" : {
      "LABEL" : "bassets_metadata_description",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-asset-description" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-imagedescription" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-description" ],
                    "value" : [ "entity:field-exif-imagedescription" ]
                  }
                }
              ]
            },
            { "ELSE" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-description" ],
                    "value" : [ "entity:field-iptc-caption" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_distancerange'] = entity_import('rules_config', '{ "rules_bassets_metadata_distancerange" : {
      "LABEL" : "bassets_metadata_distancerange",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "entity" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-image-subjectdistancerange" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-subjectdistancerange" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-image-subjectdistancerange" ],
                    "value" : [ "entity:field-exif-subjectdistancerange" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_meteringmode'] = entity_import('rules_config', '{ "rules_bassets_metadata_meteringmode" : {
      "LABEL" : "bassets_metadata_meteringmode",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "entity" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-image-metering-mode" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-meteringmode" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-image-metering-mode" ],
                    "value" : [ "entity:field-exif-meteringmode" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_orientation'] = entity_import('rules_config', '{ "rules_bassets_metadata_orientation" : {
      "LABEL" : "bassets_metadata_orientation",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-image-orientation" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-orientation" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-image-orientation" ],
                    "value" : [ "entity:field-exif-orientation" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_photographer'] = entity_import('rules_config', '{ "rules_bassets_metadata_photographer" : {
      "LABEL" : "bassets_metadata_photographer",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-asset-photographer" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-artist" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-photographer" ],
                    "value" : [ "entity:field-exif-artist" ]
                  }
                }
              ]
            },
            { "ELSE" : [
                { "data_set" : {
                    "data" : [ "entity:field-asset-photographer" ],
                    "value" : [ "entity:field-iptc-by-line" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_metadata_photometric'] = entity_import('rules_config', '{ "rules_bassets_metadata_photometric" : {
      "LABEL" : "bassets_metadata_photometric",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_create" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "entity" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "data_is_empty" : { "data" : [ "entity:field-image-photometricinterpret" ] } }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "entity:field-exif-photometricinterpreta" ] } },
              "DO" : [
                { "data_set" : {
                    "data" : [ "entity:field-image-photometricinterpret" ],
                    "value" : [ "entity:field-exif-photometricinterpreta" ]
                  }
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_printable_measure'] = entity_import('rules_config', '{ "rules_bassets_printable_measure" : {
      "LABEL" : "image_printable_measure",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_update" ],
      "REQUIRES" : [ "rules", "rules_conditional" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "entity:field-computed-height" ] } },
        { "NOT data_is_empty" : { "data" : [ "entity:field-computed-width" ] } }
      ],
      "DO" : [
        { "data_convert" : {
            "USING" : {
              "type" : "integer",
              "value" : [ "entity:field-computed-height" ],
              "rounding_behavior" : "up"
            },
            "PROVIDE" : { "conversion_result" : { "height" : "Height" } }
          }
        },
        { "data_convert" : {
            "USING" : { "type" : "integer", "value" : [ "entity:field-computed-width" ] },
            "PROVIDE" : { "conversion_result" : { "width" : "Width" } }
          }
        },
        { "data_convert" : {
            "USING" : { "type" : "decimal", "value" : [ "entity:field-unit-of-measurement" ] },
            "PROVIDE" : { "conversion_result" : { "unit" : "Unit" } }
          }
        },
        { "data_calc" : {
            "USING" : {
              "input_1" : [ "height" ],
              "op" : "\\/",
              "input_2" : [ "entity:field-image-print-resolution" ]
            },
            "PROVIDE" : { "result" : { "result_height" : "Calculation result height" } }
          }
        },
        { "data_calc" : {
            "USING" : { "input_1" : [ "result-height" ], "op" : "*", "input_2" : [ "unit" ] },
            "PROVIDE" : { "result" : { "result_height_2" : "Calculation result height 2" } }
          }
        },
        { "data_convert" : {
            "USING" : { "type" : "integer", "value" : [ "result-height-2" ] },
            "PROVIDE" : { "conversion_result" : { "result_height_final" : "Conversion result height final" } }
          }
        },
        { "data_calc" : {
            "USING" : {
              "input_1" : [ "width" ],
              "op" : "\\/",
              "input_2" : [ "entity:field-image-print-resolution" ]
            },
            "PROVIDE" : { "result" : { "result_width" : "Calculation result width" } }
          }
        },
        { "data_calc" : {
            "USING" : { "input_1" : [ "result-width" ], "op" : "*", "input_2" : [ "unit" ] },
            "PROVIDE" : { "result" : { "result_width_2" : "Calculation result width 2" } }
          }
        },
        { "data_convert" : {
            "USING" : { "type" : "integer", "value" : [ "result-width-2" ] },
            "PROVIDE" : { "conversion_result" : { "result_width_final" : "Conversion result width final" } }
          }
        },
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "result-height-final" ] } },
              "DO" : [
                { "CONDITIONAL" : [
                    {
                      "IF" : { "NOT data_is_empty" : { "data" : [ "result-width-final" ] } },
                      "DO" : [
                        { "data_set" : {
                            "data" : [ "entity:field-printable-measure" ],
                            "value" : "Bei [entity:field-image-print-resolution] dpi druckbar bis [result-height-final:value] x [result-width-final:value] [entity:field-unit-of-measurement]"
                          }
                        }
                      ]
                    }
                  ]
                }
              ]
            }
          ]
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_tags_mapping'] = entity_import('rules_config', '{ "rules_bassets_tags_mapping" : {
      "LABEL" : "bassets_tags_mapping",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_update" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "entity" ],
            "type" : "file",
            "bundle" : { "value" : { "image" : "image" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "entity:field-image-tags" ] } }
      ],
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "text" },
            "PROVIDE" : { "variable_added" : { "assets_tags_list_flat" : "Assets Tags List Flat" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity:field-image-tags" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "data_set" : {
                  "data" : [ "assets-tags-list-flat" ],
                  "value" : "[assets-tags-list-flat:value] [list-item:name]"
                }
              }
            ]
          }
        },
        { "data_set" : {
            "data" : [ "entity:field-asset-tags" ],
            "value" : [ "assets-tags-list-flat" ]
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  $items['rules_bassets_update_collection'] = entity_import('rules_config', '{ "rules_bassets_update_collection" : {
      "LABEL" : "image_update_collection",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "entity_rules_update" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "entity" : { "label" : "Entity", "type" : "file" },
        "continue" : { "label" : "Continue Rules", "type" : "boolean" }
      },
      "DO" : [
        { "component_rules_bassets_insert_collection" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue2" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_aspect_ratio" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue3" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_printable_measure" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue4" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_licence_mapping" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue5" : "Continue Rules" } }
          }
        },
        { "component_rules_bassets_tags_mapping" : {
            "USING" : { "entity" : [ "entity" ], "continue" : 0 },
            "PROVIDE" : { "continue" : { "continue6" : "Continue Rules" } }
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "continue" ]
    }
  }');
  return $items;
}
