The External Help module makes it easier for other modules to add links to
external help pages, complementing description texts and help files. The
audience of this module is purely other modules – no user interface is provided
apart from the module's help page.

WHY THIS MODULE?
================

There is already Advanced Help out there, and you can write whatever you like in
description texts. So why use this module?

This module supports the idea that it makes sense to have module documentation
on drupal.org, in contrast to having it in-code with the module. This makes it
possible to find, read and improve documentation without having to install
modules and/or write patches.

The idea of this module is to make it easier to link to external help pages in a
consistent way, and having all the external help pages listed in one place in
the module code – making it easier to maintain them.

This module is also inspired by the discussions about having inline help texts
imported from drupal.org, similarly to how translations are imported, thereby
allowing updates and improvements without having to change core. This module,
however, doesn't go very far in trying to solve that problem. (For more
information on this issue, see http://drupal.org/node/1291058.)

STEPS FOR MAKING USE OF THIS MODULE
===================================

1. Add a mymodule.externalhelp.inc in your module folder.
2. In the new file, implement hook_externalhelp(). This function should return
   an array with 'topics' as keyes – strings you choose yourself, but are used
   later on for fetching the help URL. Each topic is in turn represented by
   another array, with label and url for the help page. Example:

     $topics = array(
       'overview' => array(
         'label' => t('Overview of My Module'),
         'url' => 'http://drupal.org/my-module-overview',
       ),
       'configuration_details' => array(
         'label' => t('Details about My Module configuration'),
         'url' => 'http://drupal.org/my-module-config',
       ),
     );

3. In descriptions and help texts, feel free to use externalhelp_url('module',
   'topic') to fetch the URL for a given module and a given topic.
4. Alternatively, use the syntax externalhelp_url('module', 'topic', TRUE) to
   get a rendered help icon instead of a bare URL.
5. You can also use externalhelp_list('module') to get the full list of external
   help pages for a module, for example to include on the module's on-site
   help page. The function returns a formatted HTML list with all pages, using
   their labels as link text. (See externalhelp.api.php for example.)

Note that it is perfectly possible to add more external help pages than are
actually used in inline help texts. If you have some good external documentation
pages, just add them to hook_externalhelp and they will show up when running
externalhelp_list.

FUTURE IDEAS AND PLANS
======================

* It could make sense to allow overrides of the icon for external help pages.
  Maybe. If so, this could be an optional third entry in the array describing
  each help topic.

* It would be really nice if there was a way to fetch the external help pages
  and store locally cached copies. Especially if it was possible to change these
  and have documentation modified to fit the present site. Or so.
