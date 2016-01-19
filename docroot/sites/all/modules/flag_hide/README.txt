
FLAG, HIDE & MUTE QUICK INSTALL & USAGE INSTRUCTIONS

Flag, Hide & Mute is a module for site administrators to
o nominate a flag for users to be employed as a "hide this content" toggle
o nominate a flag for users to "mute" other users, i.e., hide all of their
  posts, both nodes and comments

Most of the Flag, Hide & Mute functionality relies on the Flag module. You can
install it from drupal.org/project/flag.

To the site administrator, the functionality to hide nodes and/or mute users
appears as a tick box at the top of the existing Flag form. Enter via
admin/structure/flags and press either "edit" on an existing flag. Or "Add
flag", selecting either the "Nodes" (for hiding content) or "Users" (for muting
users) radio buttons.
After ticking the "Use this flag to hide... " box, complete the remainder of
the form as per usual, renaming the flag if desired, setting link texts etc.

For example, for a node-hiding flag:
  Title: "Hide content"
  Flag link text: "Hide this"
  Flag link description: "Do not show this post to me again"
  Flagged message: "From now on this post will be hidden from view."
  Unflag link text: "Unhide this"
  Unflag link description: "Remove this post from your hide-list"
  Unflagged message: "From now on this post will be visible again."
  Display in entity links: Display on Full content and Teaser view modes
  Link type: Normal link
Press "Save flag".

A Link type of "Normal" will make sure the page is refreshed the moment the user
marks the content as to be hidden.

A quick way to test the above is by promoting a few nodes to the front page and
then clicking the "Hide content" link that should appear with these.

For a user-muting flag try something like this:
  Title: "Mute users"
  Flag link text: "Mute this user"
  Flag link description: "Suppress any posts by this user"
  Flagged message: "From now on posts by this user will be hidden from view."
  Unflag link text: "Unmute this user"
  Unflag link description: "Remove this user from your mute list"
  Unflagged message: "From now on posts by this user will be visible again."
  Display in entity links: Display on User account view mode
  Link type: Normal link

From the moment the user presses the "Hide this" (aka bookmark) link, the
associated content will remain invisible to them until they decide to "Unhide"
(unbookmark) it. For this they would typically visit a View, e.g. the /bookmarks
View that comes with the Flag module. It can be visited from both the navigation
menu and the user's account page /user/%/bookmarks. You probably want to rename
this link to /your-hidden-content or similar.
The "Mute this user" flag can be toggled by visiting the foe user's profile
page, /user/%/view. Alternatively, similar to the /bookmarks View, you could
create a View of user names, each with their own "Mute this user" flag.

FREQUENTLY ASKED QUESTIONS

Q: How to set flag permissions, i.e. which roles may use which flags?
A: These may be set up using the tick boxes at either admin/people/permissions
   or on the flag configuration page admin/structure/flags/manage/<flag-name>.

Q: Can node-hiding and user-muting flags be used as "global" flags?
A: Yes. Both node-hiding and user-muting flags may be used globally, i.e. you
   may tick the "Global flag" check box on their flag configuration pages,
   admin/structure/flags/manage/<flag-name>.

Q: Can anonymous users hide/unhide individual pieces of content?
A: Yes, provided you have installed Session API and have ticked the appropriate
   permissions at admin/people/permissions.

Q: Can anonymous users mute authenticated users?
A: Yes, see the above answer and provided anonymous users have access to
   authenticated user profiles ("View user profiles" permission) or a View with
   "Mute this user" links.

Q: Can anonymous users mute anonymous users?
A: No. Anonymous users cannot be muted by anyone.


FURTHER OPTIONAL CONFIGURATIONS

The flag-to-hide function relies on the Flag module to do the heavy lifting.
However a couple of complimentary options are available that are also useful
when the Flag module is not enabled. These are "No results behaviour" for blocks
and suppression of the View title when the View outputs no text or images.
You find all configuration options with brief descriptions at
admin/config/people/flag_hide.
Note that <div> placeholders for maps (e.g. Google, Leaflet) may constitute 
"empty" content if no text or images accompany the maps.

To implement user-selected suppression of content outside a Views context, e.g.
for individual content items promoted to the front page, two methods are
provided. If you do not wish to use the default method of suppressing
node.tpl.php content, then you can achieve the same effect by dropping an empty
or near-empty file node--hide.tpl.php in your theme. It must go in the same
directory as your theme's existing node.tpl.php file.
This node--hide.tpl.php file may be empty or you can put a message in it:
<?php
  echo "The post with nid=$nid was hidden at your request.";

NOTES

Only nodes and users are supported for flagging in combination with this module.
Comments are suppressed based on their authors, i.e. the user flag. You cannot
hide individual comments
You cannot globally or individually hide taxonomies, files or profiles.

Node content is hidden in teaser and full content formats anywhere where
rendered through your theme's node.tpl.php template.

In Views, content hiding works out of the box for Content and Rendered Entities
in Grids, Unformatted and HTML lists. 
However for a more complete solution that also works when displaying entity
Fields (i.e. in Views tables) do the following:
1) Add the "Flags: Node flag" relationship. Untick the box "Include only flagged
   content", press the radio button next to your flag and also the radio button
   "By: Current user".
2) Under Fields you can now add a "Flags: Flag link" to appear with each entity
   output by your View.
3) To filter on flagged (i.e. hidden) entities, add "Flags: Flagged" under
   Filter Criteria. Then press either "Flagged" or "Not flagged" depending on
   whether you want to tabulate hidden or visible nodes.

                                    * * *
