Module: iCopyright
Author: Jonathan Peterson <http://drupal.org/user/111183>


Description
===========
Adds the iCopyright Toolbar and Interactive Copyright Notice to a site.

This module is sponsored by iCopyright, Inc., http://www.icopyright.com.

Online documentation is available at http://info.icopyright.com/drupal.


Requirements
============
* CURL


Installation
============
Copy the 'icopyright' module directory in to your Drupal sites/all/modules
directory as usual.


Usage
=====
Log in as admin and enable the module.

Visit admin/config/content/icopyright/signup and fill out the form. This will
submit your information to the iCopyright servers, and give you an
appropriately configured account.

Then visit admin/config/content/icopyright/general and modify the circumstances
under which the article tools will be displayed. You can adjust the
content types or taxonomic categories, and select which style of
article tools you want to use.


Advanced
========
Visit https://license.icopyright.net/publisher to adjust settings for
your publication: the terms under which your content can be licensed,
the cost, and so on.

You can theme the feed via the template file icopyright-feed.tpl.php
to send additional information to the copyright servers about your content.


Troubleshooting
===============
In order for the body-specific services to work (print this story, for
example), the iCopyright servers must be able to access your server. (It
fetches via HTTP a decorated version of each node via the icopyright-feed.tpl.php
file.) If you are testing or developing behind a firewall, or with the
URL localhost or the like, then body-specific services will not work.

When you sign up for a new account using the admin form, the site URL
will be sent to iCopyright. It is at this site that iCopyright will try
to fetch your node details. If you move domains (while pushing from
development to production, for example) you will have to log in to your
Conductor account to change this.


Support
=======
iCopyright offers support at publishers@icopyright.com.


Background and Code Orientation
===============================
This section is for developers who want to understand the code base or how
the plugin works.

There are four main responsibilities of the icopyright plugin:

  1. Signing up for a free Conductor account with iCopyright
  2. Defining on what content the article tools should be attached to, and
     how they should appear
  3. Automatically adding the article tools to the right nodes
  4. Providing a machine-readable version of the node that the iCopyright
     servers can read

Each of these four is described in turn below.

1. Signing up
-------------
For the module to work, there needs to be a "Conductor Account" with iCopyright.
The icopyright.admin.inc file includes a form to sign up: the data is collected
from the user and submitted to the iCopyright servers via the correct function
in icopyright-common.php. If successful iCopyright sends back a "publication ID"
which uniquely identifies the site. The user email address, Conductor password,
and publiation ID are stored in variables for subsequent use.

If the user already has a publication ID the signup process can be skipped;
add that data to the "advanced" sections.

2. Article Tools options
------------------------
The rest of icopyright-admin.inc lets the admin specify how and under what
circumstances the article tools should be displayed.

  (a) The iCopyright Toolbar is normally displayed at the top of a story. These
      include the standard email, print, republish, etc. links that are all
      handled by the iCopyright servers. These can be displayed in a vertical
      or horizonal mode, and aligned left or right.

  (b) The Interactive Copyright Notice is a copyright notice at the bottom of
      the node, which is clickable. When clicked the user goes to the iCopyright
      menu of options for that particular story.

The output for both of these features is themeabe, if you so desire. The hyperlinks
to the iCopyright service must of course be correct, including the publication ID
and the right format. See the documentation in each tpl.php file.

Display of the article tools can be set to "manual", meaning that the module will
not automatically add the themed tools to the node. In this case it is up to some
other process, whatever that may be, to render them.

Further, the admin can select which stories should get the article tools and which
should not. The most common way is probably by node type: for example a standard
story or blog post should get the tools, but a page ("About me") or a product node
in a store probably should not.

If the site is using taxonomy, the admin can also select which taxonomic terms
the article tools should be applied to.

3. Article Tools display
------------------------
For the article tools to be used they obviously must be clickable by human readers.
Functions in icopyright.module hook node_api so that they are added as appropriate.
When a node is being loaded for display, it's checked to see if it meets the
criteria set from #2 above, and if it does, the article tools are themed and added
to the node for display.

Admins can also select specific nodes that the article tools should NOT be
applied to. For example, if a single node is actually copyrighted work owned by
someone else, then the site does not have the legal authority to offer that work
for reuse through the iCopyright system, so the node should not get the tools.

4. iCopyright Feed
------------------
For the iCopyright servers to create a version of the content to deliver to the user
when the content is licensed, obviously they need a copy of that content. To get
this content, the servers do an HTTP get against the Drupal site, at icopyright/%.
The icopyright_feed function is responsible for responding to it.

First, that method makes sure that it's legitimate to respond to the query at all.
The content is not delivered to iCopyright if the content is not visible to
anonymous viewers, if it doesn't have the article tools afixed to it, if the node
has explicitly been marked as not for iCopyright, and so on. Assuming all those tests
pass, the content is themed via icopyright-feed.tpl.php and returned to the
servers. (The fields are sanitized before sending to iCopyright, but iCopyright does
its own sanitation check on the content as well -- so if some of the content
is missing from the fulfilled content, that's why. iCopyright tech support can
change the sanitation policy for your site under certain circumstances.)

Note that there's no security check or the like done on the fetch URL. This is
the standard iCopyright implementation: it assumes that the content is free to all,
to read, so there's no authentication. To add such an authentication check, set up
HTTP basic authentication on your site with the SecureSite module
(http://drupal.org/project/securesite) and then configure the iCopyright servers
at Conductor > Publications > Feed Settings.

