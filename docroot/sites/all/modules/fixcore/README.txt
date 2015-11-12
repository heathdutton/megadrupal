
README.txt for Fix Core 6.x-1.x



>>>> Please feel free to suggest improvements and additions to this file and the module in general! <<<<



This module collects various fixes to core issues that have not been
implemented yet. By default, all of these fixes are disabled, and you can
enable them one by one. 

   You do this at your own risk!
   Be sure to test each fix before implementing it on a live site.


Whenever you update Drupal, you must check whether the enabled fixes still
apply. At some point, some of the issues may get fixed in core, and the
corresponding fix in this module may then start to cause problems, if it's
still enabled.


Administrator issues
--------------------

-- Avoid clobbering the user's access time on the admin/user/user page through
   administrative action

   This bug was introduced in Drupal 6.00 BETA3 and it seems impossible to get
   fixed. It's the reason for starting this module.
   
   [ ] Don't fake access, preserve 'never'
       If you create a new user, core immediately sets their Last access to the
       same time as their Member for value. Correct behavior would be to show
       'never' until the user has actually accessed the site, so that you can
       easily tell whether a user has activated their account or not. Also,
       users should not appear in the Who's new block if they've never accessed
       your site.

       If you want to fix existing users and find out who has never accessed
       your site, run the following query:

          UPDATE {users} SET access = 0 WHERE access = created;

       The downside of enabling this fix is the following: the user pages of
       users with access=0 are inaccessible to other users, and if you have a
       buggy module or view that displays a list of user records and fails to
       filter out the access=0 records, then you may get links to 'access
       denied' (403) pages; this needs to be fixed in that module or view!

   [ ] Preserve access time when editing an existing user
       When an administrator edits a user, e.g. to block them, core sets their
       Last access to the current time. Correct behavior would be to leave Last
       access unchanged.


-- [ ] Avoid MSOffice Server Extensions page not found errors
       If you're tired of seeing page not found errors for MSOffice/cltreq.asp
       and _vti_bin/owssvr.dll, then turn this on.
       
       
End user issues
---------------

-- Newbie users (not) changing their password

   This is a severe usability and maintenance issue: experience shows that
   newbie users who log in via a log-in link often forget to click the [Save]
   button after changing their password. This causes endless frustration to
   your users and to you, because they always fail to log in.

   [ ] Provide a static Save hint on the user/1/edit page
       This adds a reminder to the description below the password change widget.

   [ ] Provide a JavaScript Save hint on the user/1/edit page
       This adds a dynamic reminder to the password change widget (and
       suppresses the static reminder, if JavaScript is enabled).

   [ ] Write a watchdog entry when the user password is updated
       Knowing whether they did change their password or not will help you to
       get them over that critical hurdle.

-- [ ] Collapse the Revision information fieldset for new nodes
       This is a usability issue: if you turn on Create new revision for some
       content type and you have regular users creating nodes of that type,
       then these users are often confused by the open Revision information
       fieldset. This is rarely needed for new nodes.

-- [ ] Redirect logins to the front page
       This is a usability issue: The Drupal default is to redirect to the dull
       and ugly user page; turn this on to redirect to the (hopefully now
       enhanced) front page. Logins through the Login block are not affected.

-- [ ] Allow preselecting the contact category
       This is a usability issue: the site-wide contact form does not allow
       preselecting a category, and thus you cannot create category-specific
       links. Turn this on and add the index of the desired category to the
       path, such as contact/1.


Display issues
--------------

No I-prefer-it-this-way tweaks, only fixes for obvious bugs that interfere with
intended use!

-- [ ] Make multiselect definition lists auto-sized
       Drupal core's system.css gives only a hard-coded 8em width to
       multiselect definition lists such as the one on admin/user/user. This is
       not enough for some translations, e.g. German, resulting in a garbled
       display.

-- [ ] Make the background of unpublished nodes a darker shade of red
       Users with the administer nodes permission have access to unpublished
       nodes, and Drupal core's node.css displays them with a reddish
       background. If it looks white to you, then turn this on to make it
       redder.


Development issues
------------------

-- [ ] Provide a debug_backtrace() when user 1 gets an 'access denied' error
       This should never happen, but when it does, you need to find out where
       it occurs. Install the Devel module to see the result.

-- [ ] Make sure $node->tid is the forum tid -- FIXED in 6.16.
       This is a technical issue: if you have forum nodes with additional
       terms, $node->tid (which is supposed to be the forum id) can be the id
       of one of the other terms; core uses the lowest tid. This can confuse
       other modules.




Acknowledgements
================

All current fixes by salvis.

Use the issues queue to suggest new fixes. Include a patch as well as a link to
the issue that describes the problem.

