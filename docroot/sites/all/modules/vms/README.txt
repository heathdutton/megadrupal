This project contains two modules: Views Menu Support and Menu Item Reference
Widget. Briefly, this is what they do:

Menu Item Reference Widget allows populating integer fields with menu item IDs,
picked from select lists.

Views Menu Support provides a filter handler and a default argument plugin to
Views, allowing for filtering on currently active menu item(s). This can be
filtered on only the active item, the whole menu trail, or the menu trail
excluding the currently active item.

## What?

The functionality in Views Menu Support may seem very abstract, so here's an
example of what you can do:

* Add a new content type, "Sidebar message". Add an integer field with the Menu
  Item Reference Widget.
* Build a views block listing Sidebar messages. Add a filter "MLID filtering"
  for your integer field, selecting "is in currently active menu item".
* Place the block in a sidebar. On each page, all Sidebar messages matching the
  active menu item will be displayed.

MORE!

* Add another integer field to Sidebar messages, labeled "Show below these
  pages", and with the Menu Item Reference Widget.
* Edit the view and add another filter group (with OR condition). Add an MLID
  filter for the new field, selecting "is in trail, excluding current page".
* Result: On each page, all Sidebar messages matching the active menu item (with
  the first field) OR any parent menu item (with the second field) will be
  displayed.

Confused? It's ok. Try turning on the example feature bundled with this project,
and you can see what this looks like. But don't forget to place the block
"Sidebar messages" in a region!
