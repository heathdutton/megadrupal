REST/JSON
=========

This module is a hassle-free tool to expose Drupal content via a REST/JSON interface. 

It's designed for mobile apps, particularly for read-only Drupal backends and this means strong READ APIs and rough WRITE APIs.

Dependencies:

- entity: https://www.drupal.org/project/entity
- select_translation: https://www.drupal.org/project/select_translation

APIs
----

REST/JSON is meant to be as plug-and-play as possible, thus just enabling it you'll get the following APIs out-of-the-box:

* NODE APIs

- http://example.com/<language>/rest/list/<content-type>: list of all entities of given content-type
- http://example.com/rest/node/<nid>: details of given node
- http://example.com/rest/add/node/<content-type>: create a new node

For each node the APIs print all custom fields (i.e. fields starting with "field_" defined through the FieldsUI) and the
base fields configured in rest-config.inc.

If you want to add more fields, i.e. computed fields or non-trivial fields you can implement 

- hook_rest_json_node_alter(&$nodew, $page) // $nodew is the entity wrapper of the node being printed
- hook_rest_json_node_add_alter(&$nodew) // $nodew is the entity wrapper of the node being added

* COMMENT APIs

- http://example.com/rest/add/coment/<nid>: create a new comment on node <nid>

For each comment the APIs print all custom fields (i.e. fields starting with "field_" defined through the FieldsUI) and the
base fields configured in rest-config.inc.

If you want to add more fields, i.e. computed fields or non-trivial fields you can implement 

- hook_rest_json_comment_alter(&$commentw) // $commentw is the entity wrapper of the comment being printed
- hook_rest_json_comment_add_alter(&$commentw) // $commentw is the entity wrapper of the comment being added

* USER APIs

- http://example.com/rest/user/login: login API
- http://example.com/rest/user/logout: logout API
- http://example.com/rest/user/auth: check if user is logged
- http://example.com/rest/user/register: register new user
- http://example.com/rest/user/password/change: change password API
- http://example.com/rest/user/password/reset: send the reset link to the user

For each user the APIs print all custom fields (i.e. fields starting with "field_" defined through the FieldsUI) and the
base fields configured in rest-config.inc.

If you want to add more fields, i.e. computed fields or non-trivial fields you can implement 

- hook_rest_json_user_alter(&$userw) // $userw is the entity wrapper of the user being printed

Consumer apps can login with the login API and save the session cookie. Subsequent logged APIs will read that cookie to authorize the user.

More documentation can be found in the live demo at http://dev.mobimentum.it/drupal-rest-json/

As you've seen so far, this is a module for developers, there's no UI here. Have a look at rest-config.inc, there are a few options to 
customize, such as image format, fields to print, rest context and so on.

Custom APIs
-----------

Last but not least, there are a few functions that may be useful:

- rest_json_response($result, $bundle, $content, $message, $is_cached): useful if you want to implement a custom API endpoint, e.g.

function mymodule_custom_api() {
	rest_json_response(FALSE, 'node', NULL, 'This is a custom message');
}

Pretty print
------------

Add "d" parameter to each API to pretty print the output.

