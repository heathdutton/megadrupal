CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * How to use
 * Development


INTRODUCTION
------------

Current Maintainer: Tim van der Linden <tim@shisaa.jp>

Sometimes a View will produce only a single result.
In such a case, having the search results listed could be a bit redundant
and cutting out this step (thus skipping directly to the content) is more
user friendly.

That is exactly what Views Fast Forward does, without the need to put non-semantical,
custom PHP inside your View's header/footer area.

This module gives you an extra option on each of your View's displays to
enable such a fast forward if it returns just one result.
The path that will be used to redirect to is fully configurable and
supports token field replacement.

Note that, for now, this only works on Views which use the "Fields" display style.

INSTALLATION
------------

1. Simply checkout the latest release of the module or directly download its \*.tar.gz/\*.zip
2. Enable the module under admin/modules


HOW TO USE
----------

1. Go to the View/display you wish to fast forward (a View using the "Fields" display style)
2. In the Others listing under the Advanced heading you will find a new option called Fast forward
3. Click on Disabled to enable and setup your redirection path


FURTHER DEVELOPMENT
-------------------

The module currently simply does what is says: redirect to a path on a single result View.

Yet, there may be more features or ideas that would fit nicely inside this module which
I did not think of yet. Or maybe there are other use cases which should be taken into
consideration. I'm open to everyone's input!

Please let me know all your ideas / features / worries via the issue queue.
