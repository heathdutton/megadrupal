The block search user module provides a simple "Search Users" 
block that is nothing more than a shortcut for running a user 
search. The user search function from the core search module 
is used.

This module is particularly useful in conjunction with the 
Quick Tabs module; by adding the core search block and the 
user search block from this module to a quick tab, a control 
for searching either the site content or site users similar to 
the one shown on the right can easily be created. Users of 
Organic Groups will find that the group search block will also 
fit in nicely here.

LIMITATION: Presently, this module does not provide all of the 
same theme hooks that the search block does. Workarounds may 
be necessary in your theme's css.

For example:

  #block-quicktabs-search_quicktabs #edit-block-search-user-search-user-block-form-1-wrapper {
    display: inline;
  }

The above example presumes that you have created a quicktab
named "search_quicktabs", and added the user search block to 
it.  Future releases of this module will add the 
"container-inline" div, just as the core search block does, so
that this step will no longer be necessary.
