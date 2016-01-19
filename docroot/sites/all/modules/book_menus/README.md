### Introduction ###

Book Menus allows you to set books as normal drupal menus.  This means they will
be listed in `admin/structure/menu`, and have all of the additional
functionality that comes along with it.

This will allow you to use menu-manipulation modules.  For example, the initial
development was due to wanting a more flexible book menu structure with
titles/sections that are not necessarily links.  This can be done using
book_menus along with [special_menu_items][special_menu_items].

Another possibility would be to use it with [menu_html][menu_html].  Say you
have icons requiring special markup like `<span class="icon-book"></span>`.
Book menus will allow you to do this since it's a normal menu.

### Installation ###

- [Enable][enable] as usual,
- To manage the menu, the user must have the `administer menus` permission,
  in addition to the normal book permissions.

### Configuration ###

- Create some books,
- Navigate to `admin/content/book/settings` and chose the Book Menus,
- On the book list page, the edit operation will redirect to the normal menu,
- You can also see the menu at `admin/structure/menu`,
- Modify the menu as required.

### Blocks ###

This menu provides a block, *Book Menu Navigation*, that is similar in
functionality to core book.module with the *show block only on book pages*
setting.

However, the main difference it that it will always show the full tree.  This
was necessary as non-book menu links and #below items weren't getting rendered
as core book.module passes the book menu item to menu_tree_output.

You can still use the core book.module Book Navigation block, but on Book Menus
it will likely be the *Book Menus navigation* block you're looking for.

### Notes ###

- As this module intercepts the `admin/content/book/*` page, other book
  manipulation module may not work with it,
- Since the book navigation is a real menu, you can optionally integrate it with
  menu modules such as [nice_menus][nice_menus].

[special_menu_items]: https://www.drupal.org/project/special_menu_items
[menu_html]: https://www.drupal.org/project/menu_html
[enable]: https://drupal.org/documentation/install/modules-themes/modules-7.
[nice_menus]: https://www.drupal.org/project/nice_menus
