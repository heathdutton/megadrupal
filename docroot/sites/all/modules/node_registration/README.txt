
Entity based registration system for Drupal
====

Set up
----

1. Download & enable + dependencies
2. Choose node types to enable
   (`admin/structure/types/manage/TYPE/edit`)
3. Add fields to new registration type
   (`admin/structure/node_registration/manage/TYPE/fields`)
4. Tweak registration type settings
   (`admin/structure/node_registration/manage/TYPE/settings`)
5. Party

Permissions
----

Most permissions are intelligent: a combination of
checkboxes (`People > Permissions`), registration type
settings and node registration settings (`node/2/registration/settings`).

Settings
----

For registration types and nodes:

* Enable (per type **and** per node)
* Capacity
* Max slots per authenticated registration
* Max slots per anonymous registration
* ~~Allow e-mail address change?~~
* ~~Require e-mail address verification?~~
* Send reminder before event
* Registration success message
* E-mail sender name
* E-mail sender address
* E-mail to registree
* E-mail to admin
* Reminder e-mail
* Mailhandler which provides attachment support (needs mimemail module)

For registration type only:

* Date field (which node type field to use for start date/time)

For nodes only:

* Registration start date

Cool options
----

* `drupal_alter`s
* Views integration
* Tokens

Developers
----

See the dev docs: docs/DEVELOPERS.md

To do
----

### Settings

* Cancel mail to registree
* Cancel mail to admin

### More

* **double check & test tokens**
* more drupal_alter interfaces, because they rule
* fix the crazy menu to be like the node type menu tree
* documentation obviously (inline and outline)
