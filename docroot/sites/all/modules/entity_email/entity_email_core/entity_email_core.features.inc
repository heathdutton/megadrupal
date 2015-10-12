<?php
/**
 * @file
 * entity_email_core.features.inc
 */

/**
 * Implements hook_default_entity_email_type().
 */
function entity_email_core_default_entity_email_type() {
  $items = array();
  $items['register_admin_created'] = entity_import('entity_email_type', '{
    "name" : "register_admin_created",
    "label" : "Welcome (new user created by administrator)",
    "subject" : "An administrator created an account for you at [site:name]",
    "created" : "1367489503",
    "updated" : "1367489533",
    "entity_email_mail_body_plain" : { "und" : [
        {
          "value" : "[user:name],\\u000D\\u000A\\u000D\\u000AA site administrator at [site:name] has created an account for you. You may now log in by clicking this link or copying and pasting it to your browser:\\u000D\\u000A\\u000D\\u000A[user:one-time-login-url]\\u000D\\u000A\\u000D\\u000AThis link can only be used once to log in and will lead you to a page where you can set your password.\\u000D\\u000A\\u000D\\u000AAfter setting your password, you will be able to log in at [site:login-url] in the future using:\\u000D\\u000A\\u000D\\u000Ausername: [user:name]\\u000D\\u000Apassword: Your password\\u000D\\u000A\\u000D\\u000A--  [site:name] team",
          "format" : null,
          "safe_value" : "[user:name],\\u000D\\u000A\\u000D\\u000AA site administrator at [site:name] has created an account for you. You may now log in by clicking this link or copying and pasting it to your browser:\\u000D\\u000A\\u000D\\u000A[user:one-time-login-url]\\u000D\\u000A\\u000D\\u000AThis link can only be used once to log in and will lead you to a page where you can set your password.\\u000D\\u000A\\u000D\\u000AAfter setting your password, you will be able to log in at [site:login-url] in the future using:\\u000D\\u000A\\u000D\\u000Ausername: [user:name]\\u000D\\u000Apassword: Your password\\u000D\\u000A\\u000D\\u000A--  [site:name] team"
        }
      ]
    },
    "entity_email_mail_body" : { "und" : [
        {
          "value" : "\\u003Cp\\u003E[user:name],\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003EA site administrator at [site:name] has created an account for you. You may now log in by clicking this link or copying and pasting it to your browser:\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003E[user:one-time-login-url]\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003EThis link can only be used once to log in and will lead you to a page where you can set your password.\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003EAfter setting your password, you will be able to log in at [site:login-url] in the future using:\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003Eusername: [user:name]\\u003Cbr \\u002F\\u003Epassword: Your password\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003E--\\u0026nbsp; [site:name] team\\u003C\\u002Fp\\u003E",
          "format" : "full_html"
        }
      ]
    },
    "entity_email_mail_attachment" : [  ],
    "rdf_mapping" : [  ]
  }');
  $items['status_blocked'] = entity_import('entity_email_type', '{
    "name" : "status_blocked",
    "label" : "Account blocked",
    "subject" : "Account details for [user:name] at [site:name] (blocked)",
    "created" : "1367491245",
    "updated" : "1367491245",
    "entity_email_mail_body_plain" : { "und" : [
        {
          "value" : "[user:name],\\u000D\\u000A\\u000D\\u000AYour account on [site:name] has been blocked.\\u000D\\u000A\\u000D\\u000A--  [site:name] team",
          "format" : null,
          "safe_value" : "[user:name],\\u000D\\u000A\\u000D\\u000AYour account on [site:name] has been blocked.\\u000D\\u000A\\u000D\\u000A--  [site:name] team"
        }
      ]
    },
    "entity_email_mail_body" : { "und" : [
        {
          "value" : "\\u003Cp\\u003E[user:name],\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003EYour account on [site:name] has been blocked.\\u003Cbr \\u002F\\u003E\\u003Cbr \\u002F\\u003E--\\u0026nbsp; [site:name] team\\u003C\\u002Fp\\u003E",
          "format" : "full_html"
        }
      ]
    },
    "entity_email_mail_attachment" : [  ],
    "rdf_mapping" : [  ]
  }');
  return $items;
}
