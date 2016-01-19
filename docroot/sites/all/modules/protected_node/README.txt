CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Similar modules
 * Known conflicts
 * Installation
 * Configuration
 * Documentation
 * Maintainers


INTRODUCTION
------------

With the protected node module, users can restrict access to a node with a
password they provide when creating the node (or a site wide password or a per
node type password). On node creation, authorized authors can protect a node by
supplying a password and verifying the strength of the password via JavaScript.

Users that want to view the node or download one of its private attachments are
first redirected to a password query page (/protected-node). Once the user
entered the right password, they are redirected back to the original node.
Authorizations are stored in sessions, so users don't have to enter the
password over and over again once provided (requires cookies).

Email feature: The module is capable of sending the password via unencrypted
emails. This is useful if you create a page to be viewed only by a few people
you know.

Views submodule: The module has a few extensions supporting Views. It includes
filters and output extensions.

Rules submodule: The module includes a Rules extension allowing you to act on a
protected node and test the current state of a node for the current user.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/project/protected_node
 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/130911


REQUIREMENTS
------------

There is no special requirement for this module.


SIMILAR MODULES
---------------

 * Protected Pages (https://www.drupal.org/project/protected_pages):
   Protected Pages uses the path to protect your page so you can password
   protect Views pages for example. With Protected node, content creators can
   add the password as they create content, they don't have to go to another
   configuration page.


KNOWN CONFLICTS
---------------

 * Login Destination (https://www.drupal.org/project/login_destination)


INSTALLATION
------------

Once enabled, go to the administer permissions page to set the new permissions.

Below is a matrix with permissions you must set depending on your needs.

Needs:
- 1: No access at all to the resource.
- 2: Need to enter the password to see the resource.
- 3: Can view resource without password, need to enter the password to edit the
resource.
- 4: Bypass completely the protection (can edit and view without password).
- 5: Edit node passwords.

Permissions:
- A: Access protected node password form.
- B: View protected content (bypass password).
- C: Edit protected content (bypass password).
- D: Edit content type password.

Permissions\Cases | 1 | 2 | 3 | 4 | 5 |
------------------|---|---|---|---|---|
         A        |   | 1 | 1 |   |   |
------------------|---|---|---|---|---|
         B        |   |   | 1 |   |   |
------------------|---|---|---|---|---|
         C        |   |   |   | 1 |   |
------------------|---|---|---|---|---|
         D        |   |   |   |   | 1 |


CONFIGURATION
-------------

Global settings: /admin/config/content/protected_node

Per content type: admin/structure/types/manage/%content_type in the fieldset
"Protected node settings".

You can set password on the add/edit node form in the fieldset
"Password protect this %type_name".


DOCUMENTATION
-------------

 * Original:   http://www.m2osw.com/doc-protected-node
 * Drupal.org: https://drupal.org/node/2176627


MAINTAINERS
-----------

Current maintainers:
 * Ismaeil Abouljamal (izus) - https://www.drupal.org/user/514568
 * Florent Torregrosa (Grimreaper) - https://www.drupal.org/user/2388214

Previous maintainers:
 * Márk Tolmács (mtolmacs) - https://www.drupal.org/user/72361
 * AlexisWilke (AlexisWilke) - https://www.drupal.org/user/356197
 * Adam Thomason (adam_thomason) - https://www.drupal.org/user/2576960
 * zilverdistel (zilverdistel) - https://www.drupal.org/user/276047
