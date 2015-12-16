Subs roles
----------

This module provide a way to set role to
users subscribe.

Its alter subs type entity to add a field
storing roles rid to attributes to users associated
to a subs, depending on the subs status.

In that way, you don't have to create rules each time
a new role/subs type have been created and wanted to
attribute them.

If subs status change and doesn't match status to trigger
for attribution, roles added will be removed.

As it used hook_entity, it deals with :
- CRUD from admin back-end
- cron job
- subs_form module if enable

NOTE : if further users have a subscription
and its subs type have roles to assign changed,
it doesn't revert user roles previously assigned.

-----------
REQUIREMENT
-----------
- subs

-------
INSTALL
-------

- Enable module like any other contrib modules.

- Go to admin/config/workflow/subs/roles and define
  default subs status to trigger for adding roles 
  to users subscribe (or on contrary to remove).

- Add/edit a subs type and in "roles assign workflow" fieldset,
  select which roles must be added/removed and status subs to trigger
  (if no status define, will used default previously set).

- Go to admin/people/permissions and define permissions
  for allowing roles to configure settings