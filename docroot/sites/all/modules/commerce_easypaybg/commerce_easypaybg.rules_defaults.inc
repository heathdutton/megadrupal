<?php
/**
 * @file
 * Default rule configurations for Commerce Easypay BG.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_easypaybg_default_rules_configuration() {
  //Send EasyPay code to customer on order complete
  $rules_send_easypay_code_to_customer = '{ "rules_send_easypay_code_to_customer" : {
    "LABEL" : "Send EasyPay code to customer ",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "commerce_payment", "php", "rules", "commerce_checkout" ],
    "ON" : [ "commerce_checkout_complete" ],
    "IF" : [
      { "commerce_payment_selected_payment_method" : {
          "commerce_order" : [ "commerce-order" ],
          "method_id" : "commerce_easypaybg"
        }
      }
    ],
    "DO" : [
      { "drupal_message" : {
          "message" : "\u003C?php\u000D\u000A$message = db_select(\u0027commerce_payment_transaction\u0027, \u0027t\u0027)\u000D\u000A      -\u003E fields(\u0027t\u0027, array(\u0027message\u0027))\u000D\u000A      -\u003E condition(\u0027order_id\u0027, $commerce_order -\u003E order_id, \u0027=\u0027)\u000D\u000A      -\u003E execute()\u000D\u000A      -\u003E fetchField();\u000D\u000Aprint $message;\u000D\u000A?\u003E",
          "repeat" : 0
        }
      },
      { "mail" : {
          "to" : "\u003C?php print $commerce_order -\u003E mail; ?\u003E",
          "subject" : "\u003C?php print \u0027order\u0027 .  $commerce_order -\u003E order_id; ?\u003E",
          "message" : "\u003C?php\u000D\u000A$message = db_select(\u0027commerce_payment_transaction\u0027, \u0027t\u0027)\u000D\u000A      -\u003E fields(\u0027t\u0027, array(\u0027message\u0027))\u000D\u000A      -\u003E condition(\u0027order_id\u0027, $commerce_order -\u003E order_id, \u0027=\u0027)\u000D\u000A      -\u003E execute()\u000D\u000A      -\u003E fetchField();\u000D\u000Aprint $message;\u000D\u000A?\u003E",
          "language" : [ "commerce-order:state" ]
        }
      }
    ]
  }
}';
  $configs['rules_send_easypay_code_to_customer'] = rules_import($rules_send_easypay_code_to_customer);
  
  //Send an EasyPay notification e-mail to admin for EasyPaybg transaction
  $rules_send_an_easypay_notification_email_to_admin = '{ "rules_send_an_easypay_notification_email_to_admin" : {
    "LABEL" : "Send an EasyPay notification e-mail to admin",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "php", "rules", "commerce_easypaybg" ],
    "ON" : [ "commerce_easypay_payment_receive" ],
    "DO" : [
      { "mail_to_users_of_role" : {
          "roles" : { "value" : { "3" : "3" } },
          "subject" : "Order \u003C?php print $commerce_order -\u003E order_id; ?\u003E notification",
          "message" : "Order \u003C?php print $commerce_order -\u003E order_id; ?\u003E is PAID by EasyPay."
        }
      }
    ]
  }
}';
  $configs['rules_send_an_easypay_notification_email_to_admin'] = rules_import($rules_send_an_easypay_notification_email_to_admin);

  return $configs;
}
