This simple module allows nodes with required elements to be left empty when the
node is in select workflow states.

This is Useful for 'draft', 'unpublished' or 'staging' kind of workflow states
where the node isn't yet finished. It's especially useful when using nodes as
complex formal applications over several pages where users typically spend lot
of time to fill out and work with them.

The FAPI '#required' attribute will recursively be unset after the form is
build, but enabled again just before rendering, giving the user the same visual
experience as before.

This depend on the workflow.module, and it enhances the workflow state admin
page at admin/config/workflow/workflow/<wid> with an additional column "Ignore
required".


USAGE
--------------------------------------------------------------------------------

Enable the workflow.module, add a workflow as usual at
admin/config/workflow/workflow, and setup all its required stages. Check the
'Ignore required' column for workflow states that can have incomplete nodes.


LIMITATIONS
--------------------------------------------------------------------------------

Lots. This is a _simple_ module yet. The most obvious ones:

  - Node title may be empty.
  - No way to configure individual fields for 'ignore required' setting.
