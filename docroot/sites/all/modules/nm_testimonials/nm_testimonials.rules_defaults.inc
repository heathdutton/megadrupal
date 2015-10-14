<?php
/**
 * @file
 * nm_testimonials.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function nm_testimonials_default_rules_configuration() {
  $items = array();
  $items['rules_testimonial_notification'] = entity_import('rules_config', '{ "rules_testimonial_notification" : {
      "LABEL" : "Testimonial Notification",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "nodemaker", "notification", "testimonials" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "nm_testimonial" : "nm_testimonial" } }
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : "[site:mail]",
            "subject" : "[site:name]: A new testimonial has been added.",
            "message" : "A testimonial has been created on [site:url-brief].  \\r\\n\\r\\nPlease review it and publish it: [node:edit-url].",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_testimonial_redirect'] = entity_import('rules_config', '{ "rules_testimonial_redirect" : {
      "LABEL" : "Testimonial Redirect",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "nodemaker", "redirect", "testimonials" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "nm_testimonial" : "nm_testimonial" } }
          }
        },
        { "AND" : [] },
        { "NOT user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "3" : "3" } } } }
      ],
      "DO" : [ { "redirect" : { "url" : "\\u003Cfront\\u003E" } } ]
    }
  }');
  $items['rules_testimonial_thankyou'] = entity_import('rules_config', '{ "rules_testimonial_thankyou" : {
      "LABEL" : "Testimonial Thank You",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "nodemaker", "testimonials", "thank you" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "nm_testimonial" : "nm_testimonial" } }
          }
        }
      ],
      "DO" : [
        { "drupal_message" : { "message" : "Thank you for submitting a testimonial!" } },
        { "mail" : {
            "to" : "[node:field-nm-email]",
            "subject" : "Thank you for your Testimonial!",
            "message" : "[node:field-nm-name],\\r\\n\\r\\nThank you very much for submitting a testimonial on [site:name].  We truly value the feedback from our customers and strive hard to make every experience wonderful.\\r\\n\\r\\nCheers, \\r\\nThe team at [site:name]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
