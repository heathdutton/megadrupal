Read More Control module - http://drupal.org/sandbox/Monochrome/1088582
=======================================================================

DESCRIPTION
------------
A module to control when to show the Read More link that appears in Drupal 7.

There is no mechanism in core to decide if the Read More link should appear.
This means that this link will always show no matter if there is additional
content to display. This module tries to overcome this issue by allowing
administrators to determine how to handle this link in relation to both the
content type and individual fields.

As of version 7.x-1.1, the module can also be used to add Read More links to
other view modes and also other entity types. Core Drupal defined modes have
little additional functionality; the node RSS and Search Results page are about
the only additional displays that will accept a Read More link.

The module Entity view modes will allow users to define custom view modes that
will be customizble to allow users to append Read More links at will.

Note that some themes only display links on node teaser views, so these may
need twicking to get the link to appear.

REQUIREMENTS
------------
Drupal 7.x

INSTALLATION
------------
1.  Download the module from http://drupal.org/project/readmorecontrol

2.  Copy the module Read More Control modules into your modules directory.
    This is normally the "sites/all/modules" directory.

3.  Go to admin/build/modules. Enable the module.
    It is found in the User interface section.

UPGRADING
---------
As per any module, back up your site and run update.php.

USAGE
-----
By default, the module does not do anything, it must be configured first.

1 - Global settings
-------------------

There are three levels of configuration, the global is the recommended method.

Visit the page Admin » Configuration » Content authoring » Read more settings

This gives you the following options:

a) Always show link

This is the standard Drupal 7 behaviour. The link will show unless the theme or
another module removes it from the display.

b) Never show link

This removes the link.

c) Show link when required by any supported fields

This removes the link if there are fields with content that have a different
display settings that the main view. 

d) Show link when required by any supported text based fields

As per c, but limited to "Long text" or "Text with summary" text fields only.

e) Show link when required by the Body (body) field

Limited to the automatically created Body (body) field. This mimics the standard
Drupal 6 behaviour that checked the body, but ignored CCK fields.

Note:
* "Always/Never show link" (a & b) options never check field settings
* All fields can be configured to be ignored irrespective of these settings.
* Empty fields are not used when checking the need for the Read more link.
* If the main view does not show the field, the field will not trigger a link.
* Most themes link teaser titles to the node irrespective of these settings.
* When Text fields are referenced, this means fields of "Long text" or "Text
  with summary" only. Any teaser field is compared to the complete text value
  when determining if the read more link is show.
* All other fields have their field settings used to estimate if the link should
  appear. If display types or display type settings are different, then the two
  different views are considered different and this would trigger a Read more
  link. 
* More or less options may show here depending on the display mode and entity
  type that is being configured.

2 - Display settings
--------------------

Every custom display mode now has these options. Settings that were once found
in the content types edit page are now found under the default display settings.

These have the same options as above. Additionally, there is a "Default" option.
This effectively means that you want the global settings used rather than
overriding this value here.

If a custom view mode uses the default settings, the view mode settings for the
Default view mode are first checked, and if not set, then the global defaults
are used.

3 - Instance settings
---------------------

A new setting "Read More Behaviour" appears.

The default behaviour is to use the global or content type settings, but you can
expressively define that this field should be ignored or used. 

Note that if the global or content type settings do not use this field, then the
settings here are ignored. For example:

This setting is not processed if:

a) Always show link
b) Never show link

Fields are only checked if they are "Long text" or "Text with summary" fields.

d) Show link when required by any supported text based fields

Field is only checked if it is the core Body (body) field.

e) Show link when required by the Body (body) field

An example of when 'Read More Ignores Field' can be useful is if your teaser 
displays an image thumbnail that links to the full article that displays the
full sized image in it. If there is body text and the display between full and
teaser views don't differ, then the read more link wouldn't be displayed. To 
see the full image a visiter can just click on the image. However if the 
body text does differ a 'Read More' link would be displayed indicating there is
more body text in the article.

Another example when 'Read More Ignores Field' can be useful is to stop the 
'Read More' links from appearing if want to hide taxonomy fields from teaser 
view. Normally when the 'Display Read More' setting is set to 'When Required'
if you have a hidden field in teaser view that is not hidden in full view, the
'Read More' link will be shown. By setting the 'Read More Ignores Field' 
setting on something like a Taxonomy field you can stop it from being shown in 
the teaser but can still have it shown in the full view and not have a 
'Read More' link shown. 

As a note, even if the 'Read More' link is not shown, clicking on the Title of 
a node teaser will show the full node, so full node view is still easy to see.

AUTHORS
-------
Alan D. - http://drupal.org/user/198838.

Alan took over the very early sandbox prototype and developed this into the 
module it is now.

Alexis Ryan (Monochrome) - http://drupal.org/user/1147898.

Alexis developed the initial version of this module and has full credit for the
concept and base implementation.

Thanks to everybody else who have helped test and contribute patches!
