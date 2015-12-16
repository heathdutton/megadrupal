-- SUMMARY --

OG Invite People module is an extension to OG 7.x-2.x to add the invite
funcitionality via email.
It creates a new system user and adds him to OG group from which he was invited.

-- REQUIREMENTS --

OG 7.x-2.x.

-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.

-- HOW TO USE --

Enable the module, go to OG group permission page (admin/config/group/permissions)
and enable "Invite people" permission for permitted group roles.
A new link "Invite People" is displayed on your group's Group page.

-- HOOKS --

Hook triggered after user is invited.
- hook_og_invite_people_invited($account, $og_membership)