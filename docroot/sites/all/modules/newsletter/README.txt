Newsletter
===============

The Newsletter module aims to be a full blown newsletter solution for Drupal 7.
It is recommended for people
that want more than one newsletter list for their site.
If you only need a single newsletter list then you would be better with
something else since this module would be an overkill.


Requirements
==============

--Libraries
--jQplot jQuery library (download it and extract to sites/all/libraries/jqplot)

There are no other requirements for this module's basic functionality.

You could install Colorbox though if you want a cool subscription experience
for your readers!

Finally for better user experience when editing templates,
you could install the wysiwyg module and choose one of the many supported
html editors!


Installation
=============

-Download and extract your module in your sites/all/modules/newsletter folder.

-Enable it through the administration area (admin/modules) or through drush
 (drush -y pm-enable newsletter).

-Navigate to admin/config/media/newsletter.


Usage
==========

You should create your templates by visiting the templates tab.
  Basic templates like subscribe/unsubscribe/welcome are created automatically
  but you can edit them to your needs.
  Also a default template is provided as an example.

After you create your templates, you have to create your lists.
  Visit the subscribers/list tab and click on create new list.

  ---Choosing your list to be exposed means that when a subscriber subscribes
     to this list he/she will be able to choose the terms he/she wishes to
     subscribe to.
  ---A list can have certain schedules (Daily, Weekly, Monthly, Custom) or
     none if you choose to be Manual.
  ---If you choose to be Custom then you will have to fill when this list should
     be sent.(eg when 1 node of the template-chosen terms is published)

Place the subscribe block to the region you like through the
admin/structure/block page.

Adjust the permissions in admin/people/permissions.

Thats it.People can now subscribe to your lists!
You can now sit down and enjoy the statistics to the statistics tab.
To send a manual newsletter (or to create a custom one)
visit the create/send tab.


Author/maintainer
===================

Original Author and Maintainer:

Paris Liakos (rootatwc)
http://drupal.org/user/1011436


Support
=======

Issues should be posted in the issue queue on drupal.org:

http://drupal.org/project/issues/newsletter
