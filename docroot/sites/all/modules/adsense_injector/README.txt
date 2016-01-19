
Copyright 2006-2008 Exodus Development, Inc. (http://exodusdev.com)


ABOUT ADSENSE INJECTOR
----------------------

The purpose of this module is to provide a simple method of automatically
placing 'inline' adsense module content into a full-page view of selected
node types. It does this by intercepting node rendering using hook_nodeapi
during 'page' rendering, and injecting selected text into the $node->body
field.

It uses a very simple scheme at present (just string concatenation) and
could be enhanced greatly by using regular expressions, templates, or other
techniques in a later revision.

Official Adsense Injector project page:
  http://exodusdev.com/drupal/4.7/modules/adsense_injector.module

Drupal Adsense Injector project page:
  http://drupal.org/project/adsense_injector

Sponsored by:
  http://exodusdev.com/


RATIONALE
----------------------

Why is this useful? In my experience, this simplifies certain important
aspects of ad insertion and placement.

Traditional approaches:

  - Modify your theme's node.tpl.php or other template file(s) in order to
    inject ads on every node view.

    What happens if you have multiple sites or use multiple themes, or use
    custom per-node-type template files (node-book.tpl.php, node-image.tpl.php
    etc)? Now you have to edit, test, and maintain multiple template files,
    and, if the theme is updated to fix bugs, you have to merge in your
    changes.

  - Hand-edit each node content and use inline [adsense:x:y] inline filter
    tags.

    This gives tremendous flexibility in layout, but creates a maintenance
    nightmare if you should wish to alter your channel or ad layouts site-wide.

  - Use block insertion into the theme's template regions.

    This is great if your theme's regions provide the flexibility you want - it
    seems that themes vary somewhat in the regions they provide, and those
    regions aren't always in the places you want - so once again, you are back
    to tweaking theme template files if you want to place the ads near or in
    the content.


USAGE
----------------------

AdSense Injector uses (and requires) installation and proper configuration of
the AdSense Module in order to function properly. Please install, configure,
and test the AdSense module before you install Adsense Injector.


ANY DETAIL VISIBILITY CONTROL?
----------------------

You may looking for some detail visibility control as like as that of block
visibility, e.g. with PHP-mode which target for expert only.

This will not exists for adsense injector module, as adsense module already
coming with similar visibility control managment. For keeping this module as
simple as possibility, we will not duplicate this handling.

For more detail:
  http://drupal.org/node/136031
  http://drupal.org/node/198907


LIST OF MAINTAINERS
----------------------

PROJECT OWNER
M: Michael Curry <exodusdev@gmail.com>
S: maintained
W: http://exodusdev.com/

CO-MAINTAINER
M: Edison Wong <hswong3i@gmail.com>
S: maintained
W: http://edin.no-ip.com/
