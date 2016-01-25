
The wf_required_fields module allows you to configure which fields of a node are
required, based on the workflow state of the node.

Required fields are marked on the node edit form. Also transitions are objected
(with a suitable message) if required fields for the target state are missing.

Installation
------------

1. Copy this whole folder to modules directory, as usual. Drupal should
   automatically detect the module. Enable the module on the modules'
   administration page.

2. Go to admin/config/workflow/wf-required-fields to configure which content
   types to use, and which fields should be required fields in which workflow
   state.

Authors
-------
Originally written by Oswald Jaskolla <oswald.jaskolla [at] schieferdecker.com>.
