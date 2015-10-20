<?php
/**
 * @file
 * courseplanner.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function courseplanner_default_rules_configuration() {
  $items = array();
  $items['rules_cp_clear_lesson'] = entity_import('rules_config', '{ "rules_cp_clear_lesson" : {
      "LABEL" : "Clear a lesson",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "lesson" : { "label" : "Lesson", "type" : "field_collection_item" } },
      "IF" : [
        { "data_is" : { "data" : [ "lesson:field-name" ], "value" : "cp_offering_lessons" } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "lesson:cp-lesson-title" ] } },
        { "data_set" : { "data" : [ "lesson:cp-lesson-section" ], "value" : { "value" : [] } } }
      ]
    }
  }');
  $items['rules_cp_copy_lesson_to_weeks'] = entity_import('rules_config', '{ "rules_cp_copy_lesson_to_weeks" : {
      "LABEL" : "Copy lesson to weeks",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : {
        "lesson" : { "label" : "Lesson", "type" : "field_collection_item" },
        "weeks" : { "label" : "Weeks", "type" : "list\\u003Ctext\\u003E" }
      },
      "ACTION SET" : [
        { "cp_copy_lesson_to_weeks" : { "lessons" : [ "lesson" ], "weeks" : [ "weeks" ] } }
      ]
    }
  }');
  $items['rules_cp_create_and_add_sections_to_outline'] = entity_import('rules_config', '{ "rules_cp_create_and_add_sections_to_outline" : {
      "LABEL" : "Create sections and add to an outline",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "outline" : { "label" : "Course outline", "type" : "node" },
        "titles" : { "label" : "Section titles and sizes", "type" : "list\\u003Ctext\\u003E" }
      },
      "ACTION SET" : [
        { "cp_add_sections" : { "outline" : [ "outline" ], "sections_info" : [ "titles" ] } }
      ]
    }
  }');
  $items['rules_cp_create_course_outline_from_selected_lessons'] = entity_import('rules_config', '{ "rules_cp_create_course_outline_from_selected_lessons" : {
      "LABEL" : "Create course outline from selected lessons",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "lessons" : { "label" : "Lessons", "type" : "list\\u003Cfield_collection_item\\u003E" },
        "title" : { "label" : "Outline title", "type" : "text" },
        "make_owner" : {
          "label" : "Make copies of any course sections I do now already own",
          "type" : "boolean"
        }
      },
      "DO" : [
        { "cp_create_outline" : {
            "lessons" : [ "lessons" ],
            "title" : [ "title" ],
            "make_owner" : [ "make_owner" ]
          }
        }
      ]
    }
  }');
  $items['rules_cp_fill_lessons_with_outline'] = entity_import('rules_config', '{ "rules_cp_fill_lessons_with_outline" : {
      "LABEL" : "Fill lessons with course outline",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "lessons" : { "label" : "Lessons", "type" : "list\\u003Cfield_collection_item\\u003E" },
        "outline" : { "label" : "Course outline", "type" : "node" }
      },
      "ACTION SET" : [
        { "cp_fill_lessons" : { "lessons" : [ "lessons" ], "outline" : [ "outline" ] } }
      ]
    }
  }');
  $items['rules_cp_fill_lessons_with_section'] = entity_import('rules_config', '{ "rules_cp_fill_lessons_with_section" : {
      "LABEL" : "Fill lessons with a section",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "lessons" : { "label" : "Lessons", "type" : "list\\u003Cfield_collection_item\\u003E" },
        "section" : { "label" : "Section", "type" : "node" }
      },
      "ACTION SET" : [
        { "cp_fill_lessons_with_section" : { "lessons" : [ "lessons" ], "section" : [ "section" ] } }
      ]
    }
  }');
  $items['rules_cp_lesson_repeat'] = entity_import('rules_config', '{ "rules_cp_lesson_repeat" : {
      "LABEL" : "Repeat lesson",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "ACCESS_EXPOSED" : "1",
      "USES VARIABLES" : {
        "lesson" : { "label" : "Lesson", "type" : "field_collection_item" },
        "end_date" : { "label" : "End date", "type" : "date" }
      },
      "ACTION SET" : [
        { "cp_repeat_lesson" : { "lesson" : [ "lesson" ], "end_date" : [ "end_date" ] } }
      ]
    }
  }');
  $items['rules_cp_lesson_set_room'] = entity_import('rules_config', '{ "rules_cp_lesson_set_room" : {
      "LABEL" : "Set lesson room",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "lesson" : { "label" : "Lesson", "type" : "field_collection_item" },
        "room" : { "label" : "Room", "type" : "text" }
      },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "lesson" ], "field" : "cp_lesson_room" } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "lesson:cp-lesson-room" ], "value" : [ "room" ] } }
      ]
    }
  }');
  $items['rules_cp_lessons_import'] = entity_import('rules_config', '{ "rules_cp_lessons_import" : {
      "LABEL" : "Import lessons",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "offering" : { "label" : "Course offering", "type" : "node" },
        "dates" : { "label" : "Dates", "type" : "list\\u003Ctext\\u003E" }
      },
      "ACTION SET" : [
        { "cp_lessons_import" : { "offering" : [ "offering" ], "dates" : [ "dates" ] } }
      ]
    }
  }');
  $items['rules_cp_lessons_shift'] = entity_import('rules_config', '{ "rules_cp_lessons_shift" : {
      "LABEL" : "Shift lessons",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "lessons" : { "label" : "Lessons", "type" : "list\\u003Cfield_collection_item\\u003E" },
        "steps" : { "label" : "Number of steps", "type" : "integer" }
      },
      "ACTION SET" : [
        { "cp_shift_lessons" : { "lessons" : [ "lessons" ], "steps" : [ "steps" ] } }
      ]
    }
  }');
  $items['rules_cp_offering_create'] = entity_import('rules_config', '{ "rules_cp_offering_create" : {
      "LABEL" : "Actions on Course offering creation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "cp_offering" : "cp_offering" } }
          }
        }
      ],
      "DO" : [ { "redirect" : { "url" : "node\\/[node:nid]\\/admin" } } ]
    }
  }');
  $items['rules_cp_push_lessons_down'] = entity_import('rules_config', '{ "rules_cp_push_lessons_down" : {
      "LABEL" : "Push lessons down",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : { "lessons" : { "label" : "Lessons", "type" : "list\\u003Cfield_collection_item\\u003E" } },
      "ACTION SET" : [ { "cp_shift_lessons" : { "lessons" : [ "lessons" ], "steps" : "1" } } ]
    }
  }');
  $items['rules_cp_quick_embed_resource'] = entity_import('rules_config', '{ "rules_cp_quick_embed_resource" : {
      "LABEL" : "Quick-embed resource",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : {
        "section" : { "label" : "Section", "type" : "node" },
        "contextual_description" : { "label" : "Contextual description", "type" : "text" },
        "reference" : { "label" : "Reference", "type" : "text" },
        "title" : { "label" : "Resource title", "type" : "text" }
      },
      "ACTION SET" : [
        { "cp_quick_add_embedded_resource" : {
            "section" : [ "section" ],
            "contextual_description" : [ "contextual_description" ],
            "reference" : [ "reference" ],
            "title" : [ "title" ]
          }
        }
      ]
    }
  }');
  $items['rules_delete_lessons'] = entity_import('rules_config', '{ "rules_delete_lessons" : {
      "LABEL" : "Delete lessons",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Course planner" ],
      "REQUIRES" : [ "courseplanner" ],
      "USES VARIABLES" : { "lessons" : { "label" : "Lessons", "type" : "list\\u003Cfield_collection_item\\u003E" } },
      "ACTION SET" : [ { "cp_lessons_delete" : { "lessons" : [ "lessons" ] } } ]
    }
  }');
  return $items;
}
