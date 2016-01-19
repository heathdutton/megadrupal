09-09-2014, Copyright by Cipix Internet. E-mail: info@cipix.nl.

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Known issues
 * Installation

INTRODUCTION
------------

Current Maintainer: Bas van Meurs <bas@cipix.nl>

Drupal provides url alias support in the 'path' core module. But is has a
problem. When an alias to a path changes, for instance when a node title is
updated, the old url alias is overwritten or removed and gives a 404. This is
bad for SEO! This module remembers old url aliases and redirects them to the
path as well.

Notice that the 'redirect' module more or less does the same. However, I got
fed up by the fact that loops were easy to create and we encountered some
strange debug messages on production sites. The main difference with the
redirect module is that this module does NOT provide any settings page(s). It is
designed to 'just' work! It simply remembers old aliases to existing content,
what more configuration do you need!

Additionally, this module offers static 301 redirects. Go to the page
/admin/config/system/301-redirects to set the desired redirects.

KNOWN ISSUES
------------

None.

INSTALLATION
------------

Enable the module. From that point onwards, when a new url alias is assigned,
the previous alias is remembered and redirected to the newest alias.

DETAILS
------------

Possible loops and other problems are prevented because of the simple
implementation:
1) Old aliases are ONLY used in case that no menu item (menu_get_item) was found
   for the current path (a 404). This means that if a new node is created that
   has the same name as an existing old alias, the new node is shown.
2) Before redirecting from an old url alias, a check is made to assert that the
   source destination is indeed still a valid menu item. So if the aliased node
   is deleted, no redirecting will be performed (instead, the 404 will be
   delivered).
3) In case and old url alias is identified, the redirection will be issued to
   the new url alias and not to the source path. This saves you from one 301
   when having 'global_redirect' enabled, which is better for SEO and what you
   want to happen.
4) No url alias chaining is used. Consider node N that has url alias A. Then a
   new alias B is assigned to N. Then later alias C is assigned to N. Then we
   do not 301 from C to B and then B to A, but directly from C to A and B to A.
   This construction prevents possible loops and unnecessary 301s that may harm
   SEO of your website.
5) Proof that is impossible that infinite loops will occur: when performing a
   301 from an old alias, it is certain that the target has a menu_get_item (2).
   Because of this, after the redirection, another redirection by this module
   will not be performed as the menu_get_item does exist (1). Note that this
   also proves that this module will never perform more than one redirect after
   one another.