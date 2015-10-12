
-- SUMMARY --

The Menu link module defines a menu link field type.

Drupal's core Menu module allows nodes to place menu links (linking to the node)
into the menu. The Menu link module however allows entities of any type to place
menu links into the menu using a field.

-- Menu link as a field --

* One of the benefits of fields is that they automatically are available to
  other modules (like Views).
* The Field API also provides the possibility to use separate widget and
  formatters per entity type (and bundle). Contrib modules can provide all kinds
  of new widgets and formatters.
* Another benefit of the Menu link module is that it provides revisioning for
  menu links. Their title and location in the menu for example are stored per
  revision.

-- Menu module integration --

The Menu link module integrates seamlessly into the Menu module. The UI for node
types, to configure which menus are available, isn't altered.

The "Menu settings" tab on node edit forms is however removed and replaced by a
field widget (To put the widget into a vertical tab use the Field group module).

Now that menu links are stored as a field it is thus possible to use different
widgets per node type.

-- DEVELOPERS --

* Relations between menu links and entities

  The Menu link module provides one predefined field stored in a fixed database
  table (using field_sql_storage) named {field_data_menu_link}. Using a fixed
  table allows other modules to use this table to retrieve and build upon
  relations between menu links and entities (This field is also being used for
  the integration with the Menu module).

* Menu API additions

  To speed up saving/deleting of menu links, the Menu link provides a couple of
  additions to the Menu API. Those may be used by other modules.

  * menu_link_load_multiple(array $mlids, array $conditions = array())
  * menu_link_delete_multiple(array $mlids)
