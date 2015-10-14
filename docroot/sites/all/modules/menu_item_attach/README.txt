Menu Item Attach
----------------

INTRODUCTION
------------
This module allows to add some menu items inside another menu item. This is
very useful for creating mega menus.

FEATURES
--------
- Add new option "Attach to" on menu item edit page.
- Display attached menu items inside another item.
- Hide attached items from their normal position.

MARKUP BEFORE
------------
<ul>
  <li><a href="/">Home</a></li>
  <li>
  <a href="/">Level 1</a>
  <ul>
    <li>
      <a href="/">Level 2</a>
      <ul>
        <li><a href="/">Level 3</a></li>
        <li><a href="/">Level 3</a></li>
      </ul>
    </li>
    <li>
      <a href="/">Level 2</a>
      <ul>
        <li><a href="/">Level 3</a></li>
        <li><a href="/">Level 3</a></li>
      </ul>
    </li>
  </ul>
  </li>
</ul>

MARKUP AFTER
------------
<ul>
  <li><a href="/">Home</a></li>
  <li>
  <a href="/">Level 1</a>
  <ul>
    <li>
      <a href="/">Level 2</a>
      <ul>
        <li><a href="/">Level 3</a></li>
        <li><a href="/">Level 3</a></li>
      </ul>
      <a href="/">Level 2</a>
      <ul>
        <li><a href="/">Level 3</a></li>
        <li><a href="/">Level 3</a></li>
      </ul>
    </li>
  </ul>
  </li>
</ul>

DEPENDENCIES
------------
Menu module (core)

INSTALLATION AND CONFIGURATION
------------------------------
1. Download this module and unpack into sites/all/modules/contrib. See
   https://www.drupal.org/documentation/install/modules-themes for details.
2. Enable this module.
3. Go to your menu (Main menu, for example) and click 'edit' link on which
   menu item you want to attach to another.
4. Select in 'Attach to' option menu item where you want to attach this item.

SPONSOR
-------
This module was developed for IvTextil.ru http://ivtextil.ru website.
