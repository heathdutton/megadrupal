Zoundation Support module

This module is meant to be a helper module for the <a href="http://drupal.org/project/zoundation">zoundation theme</a>.

This project currently provides the following;

1. Checks to ensure that jquery_update > 2.3 is installed and configured to use
at least jquery 1.7. This is required for proper function
2. Custom menu builder functions and blocks to create appropriate block menu
content depending on the target region. Each menu is rendered as a single menu
item with the menu name as the initial link and all menu items as links under
that. For topbar blocks all menu levels are rendered in the Foundation style
flyout menus that are completely responsive. buttonbar menus are limited to one
depth only. Regardless of the topbar or buttonbar choice you can configure the
block to start rendering a menu after a given depth. This is useful if you only
want the 2nd (and deeper) links in a menu. Please ensure that you are adding the
proper block type to the proper region. Since hook_block_view() does not know
about the target context of the content as it is being built we have to provide
two different blocks to create the appropriate markup.
3. Better placeholder integration for <a href="http://drupal.org/project/elements">elements</a>
module. If the elements module is available this will add additional field
configuration to provide placeholder text on form elements.
4. Additional minor UI improvements that cannot easily be accompslished in the
zoundation theme.
