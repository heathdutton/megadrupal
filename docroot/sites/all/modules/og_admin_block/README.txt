OG Admin Block

-------------------------------------------------------------------------------
ABOUT
-------------------------------------------------------------------------------
This module creates a block, which displays a list of links to admin tasks for
an OG group, for an OG group administrator. The OG context must be set to the
group you want the links to relate to on the page(s) the block is displayed.

Links included:
- Edit the group,
- Manage members,
- Invite new member,
- Manage menus (if og_menu is enabled),
- Links to add each type of Group Content.

An alter hook is provided for adding additional links, reordering links and
general theming.

-------------------------------------------------------------------------------
INSTALLATION
-------------------------------------------------------------------------------
When you enable the module, you'll get a new a new block.

An OG context will have to be active for the block to appear.

You can add further items, or reorder the link by implementing
hook_og_admin_block_alter() - for examples see og_admin_block.api.php.
