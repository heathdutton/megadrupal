<?php

/**
 *  Implements hook_default_rules_configuration().
 */
function pixgather_default_rules_configuration() {
  $items['rules_pixgather_photo_upload_create_new_entity'] = entity_import('rules_config', '{ "rules_pixgather_photo_upload_create_new_entity" : {
    "LABEL" : "PixGather Photo Upload - Create New Entity",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "pixgather" ],
    "ON" : [ "pixgather_photo_uploaded" ],
    "DO" : [
      { "entity_create" : {
          "USING" : {
            "type" : "' . variable_get('pixgather_entity_type', '') . '",
            "param_type" : "' . variable_get('pixgather_bundle', '') . '",
            "param_title" : "PixGather Photo Uploaded on [site:current-date]",
            "param_author" : [ "site:current-user" ]
          },
          "PROVIDE" : { "entity_created" : { "new_pixgather_entity" : "New PixGather Entity" } }
        }
      },
      { "data_set" : {
            "data" : [ "new-pixgather-entity:' . variable_get('pixgather_image_field','') . ':file" ],
          "value" : [ "file" ]
        }
      },
      { "data_set" : {
          "data" : [ "new-pixgather-entity:' . variable_get('pixgather_name_field','') . '"],
          "value" : [ "name" ]
        }
      },
      { "entity_save" : { "data" : [ "new-pixgather-entity" ] } }
    ]
  }
}');

  return $items;
       
}
