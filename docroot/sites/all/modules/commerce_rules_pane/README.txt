WHAT IS IT FOR?
This is a small module, that provides a new commerce checkout pane, the content
of which can be configured via rules. This comes useful for example when you
want to display a certain message to the user depending on the payment method.

The module provides a new rules event ("Determine rules checkout pane content")
and a two new actions ("Set the completion message" and "Set an entity to be
displayed as completion message").

"Set the completion message"
Allows users to display simple messages in the rules commerce pane.

"Set an entity to be displayed as completion message"
Allows users to display an entity in the rules commerce pane. E.g. a node with
detailed instructions, the active order, user information, etc.
If you want to display an existing node, you should fetch the entity first -
using the already available rules actions ("Fetch entity by ID" or "Fetch entity
by propery") and then select "entity_fetched" to be displayed in the "Set an
entity to be displayed as completion message" action.

INSTALATION
Enable the module like any other Drupal module:
http://drupal.org/documentation/install/modules-themes/modules-7

After enabling this module, a new action is available in rules that allows you
to set custom text which is later displayed in a checkout pane. By default this
checkout pane is positioned in the last step (/checkout/ORDER_ID/complete).It
becomes visible after appropriate rule action is configured.
