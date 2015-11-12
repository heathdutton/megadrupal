About
-----

Project for Drupal 7.
The modules improves administration/editorial usability and provides
configurable "Editor Notes" field, which can be added to any Drupal entity.

In general concept of 'Editor Notes' is similar to 'Comments'.
The main difference is that 'Editor Notes' are for editors and admins
rather than for end users.

Usage
--------

* As website editor I can write status notes to myself and other editors working
  on the same material.
* The module provides 'Editor Notes' field available on 'Manage Fields' page.
* This field can also be added to any Drupal fieldable entity (user etc).
* List of previously added notes displays in 'content edit' form in configurable
  table.
* Notes can also be displayed for the end user in configurable table
  if necessary.
* When editor opens 'content add' form for the first time he sees a field
  (textarea) providing him an option to add the first note.
* When editor opens 'content edit' form he sees 'Editor Notes' widget
  with already added notes if any found.

Features
--------

* Website editor can only add / update / delete his own notes by default.
* However notes may also be updated or removed by any user with
  "Administer any editor note" permission.
* CRUD operations on notes support Ajax and perform in configurable modal window
  without page reload.
* Module supports Views, content revisions and content translation.

Similar Projects
----------------

The key feature of the Editor Notes module is that editor is able to add his own
notes and browse notes of other editors directly in 'edit content' form.
It saves time when adding/updating large amount of content.

1. [Admin Notes] creates a block that displays a textarea pre-filled with
   the existing comment for that specific page.
2. [SiteNotes] introduces a new content type "sitenotes." It also creates
   a menu item in the Admin >> Site building menu, where it's available only
   to privileged users.
3. [Personal Notes] consist of titles and text rendered into a block.
   The notes are specific to each authenticated user on the site
   and can be added and deleted only by the user who created them. They can only
   be viewed on the website by the user who created them.
4. [Stickynote] creates a block with the ability to add, delete,
   and edit notes (ajaxified). Notes are stored on a per path basis and
   the block can be made visible to certain roles/paths like any other block.

The difference from mentioned above modules is that Editor Notes creates
configurable ajaxified field (instead of blocks or content type)
that can be attached to an entity and associated with that entity.
Each field item has its author and controls CRUD operations based on authorship
(in the way like Comment module does).

Requirements
------------

* [Chaos tool suite]
* [Entity API]

Installation
------------

1. Drop the entire Editor Note module into your 'sites/all/modules' folder.
2. Enable the module from the Administration area modules page (admin/modules).
3. Create new or edit existing content type and add a new field of type
   'Editor Notes' (admin/structure/types in D7).

Sponsorship
-----------

This project was sponsored by:

* [EPAM Systems]
* [NBC Universal]

Acknowledgments
---------------

Special thanks to [Sergei Churilo] and [Alexey Yahnenko]
for reviewing the module.

Author
------

[Rostislav Sergeenkov]


[Admin Notes]:https://www.drupal.org/project/admin_notes
[SiteNotes]:https://www.drupal.org/project/sitenotes
[Personal Notes]:https://www.drupal.org/project/personal_notes
[Stickynote]:https://www.drupal.org/project/stickynote
[Chaos tool suite]:https://www.drupal.org/project/ctools
[Entity API]:https://www.drupal.org/project/entity
[EPAM Systems]:https://www.drupal.org/node/2114867
[NBC Universal]:http://www.nbcuni.com
[Sergei Churilo]:https://www.drupal.org/user/584658
[Alexey Yahnenko]:https://www.drupal.org/user/2635711
[Rostislav Sergeenkov]:https://www.drupal.org/u/rostislav-sergeenkov
