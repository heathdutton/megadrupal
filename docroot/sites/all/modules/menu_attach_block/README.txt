# Menu Attach Block

Menu Attach Block allows you to embed blocks in menu items. You may choose to
replace a menu item entirely with a block, or keep the menu link and embed the
block after it in the wrapper element.

Embedded blocks will appear like this in the menu:

  <li class="menu-346 first">
    <a href="/node/1">Node</a>
    <a href="# " class="menu-attach-block-drop-link">More</a>
      <div class="menu-attach-block orientation-vertical">
      // block HTML goes here
     </div>
  </li>

Some basic JavaScript and CSS is included. You can override the Javascript by
altering Drupal.behaviors.menu_attach_block.

Embedded blocks respect block page and permissions settings.

## Getting started

* Download and install the module as usual. Check here
<http://drupal.org/documentation/install/modules-themes/modules-7> if you need
 help.
* Go to the Menu admin page at admin/structure/menu.
* Click 'List links' on the menu you want to edit.
* Click 'Edit' on the menu item you want to attach a block to.
* Select a block in the 'Attach Block' fieldset.
  * If you want to replace the menu link entirely, set <block> as the path.
* You may also choose an orientation for the parent menu. This effects how 
  blocks are shown. Choose horizontal for horizontal navigation bars, and
  vertical for sidebar navigation.

The block will now appear in your menu.
