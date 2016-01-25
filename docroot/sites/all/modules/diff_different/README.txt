
DIFF DIFFERENT
==============
Diff Different provides a variation of the standard Diff functionality for
writer/editor scenarios such as teachers correcting students' work online.

Both teacher and student create new revisions of the same content. The module
automatically presents as part of the "full view" the latest revision by the
student (the original author of the content) against the latest corrections by
the teacher, as well as the resulting content, with all corrections applied.
Unlike the Diff module, when showing the side-by-side comparison, other fields,
links and node comments will also be shown on the same single integrated page.
Any revision log comments entered by either the student or the teacher are also
included in the Diff.
When a teacher does not respond with further corrections to the latest student
revision, the page displays identically to when viewed without this module.

The different Diff style can be enabled for selected content types on the module
configuration page, admin/config/content/diff-different. This is also where you
specify above or below which field of the content type you wish the module to
insert the Diff.

Diff Different works with or without Display Suite, drupal.org/project/ds.

A student is defined as the initial creator (a.k.a. author) of the content
(node). Teachers are any other users modifying that content. Hence there may be
multiple teachers correcting the content, but there can only be one student per
node.
A similar paradigm is that of author/writer working with a reviewer/editor.

Diff Different only affects node/% pages and nodes displayed in the "full", as
opposed to "teaser" mode. The module aims to not change the Diff module pages
(node/%/revisions/...) in any way.

Diff Different inherits relevant settings from the Diff module configuration
where applicable.

In order to see the side-by-side Diff section on their page, the student must
have the "View own unpublished content" permission. Teachers must have the
"Bypass content access control" permission to view unpublished content.
This is impractical for most use-cases. So enable the Revisioning module,
drupal.org/project/revisioning, for more fine-grained permissions.
Then you can keep all content of the "writing" type UNpublished to the world,
while still allowing student and teacher(s) to have a dialog on the same node,
by giving the student the "<content-type>: View revisions of own content"
permission while the teacher gets the permission to "<content-type>: View
revisions of anyone's content".
Neither student nor teacher need to have the "Edit content revisions"
permission.
