Compatability note, this module is designed to work with the 7.x-3.x version of
the Views module.

  https://drupal.org/project/views_revisions

Once the module is enabled, a checkbox and textarea will be provided on the
Views UI for creating a revision when editing a View. For example:

  admin/structure/views/view/frontpage/edit

Make changes to a View as you normally would, then before hitting 'Save' you may
choose to create a revision for the View. It is recommended to enter a message
describing the changes you have made so other admins (and your future self) will
understand your intentions.

When a revision is created, it takes a snapshot of the View BEFORE the changes
are saved, allowing you to easily roll back to older versions if necessary.

After revisions have been created, click the 'Views Revisions' link located on
the Views UI to see a list of revisions for that View, for example:

  Front Page Views UI:            admin/structure/views/view/frontpage/edit
  Front Page Views Revisions:     admin/structure/views/revisions/frontpage

Click the 'View' link in the revisions table to see the ctools export data
for the View. To revert a view, copy the ctools export data from the revision
and then go to:

  admin/structure/views/import

Check the box to 'Replace an existing view if one exists with the same name' and
then hit the 'Import' button. This will bring you back to the Views UI form with
the reverted settings ready for you to save. At this point, it is recommended to
create another revision while saving, that way you can save a backup copy of the
changes you are about to get rid of.

Finally, hit the 'Save' button to bring the revised copy of the view back.

