Panelizer Variants

This module adds the functionality to have Panelizer Variants based off of
List Defaults in Panelizer Full Page Override.  What this does is allows you
to have multiple layouts for onecontent type based off a field selection.
Right now it just works with taxonomy term reference, more can be added later
on.

In order to use this, do the following:
1. Enable the module, make sure your node_view template in page manager is
enabled as well.  Should be if you are using Panelizer.
2. Go to your panelizer page: admin/config/content/panelizer
3. Select all the check boxes for your Panelizer Full Page Override.
4. After saving, go back up and select list
5. Click add in the page_manager list
6. Add your name of the variant
7. Set it up.
8. Go look at your node and this all should be happy.

NOTE
This currently only works with a single taxonomy term
