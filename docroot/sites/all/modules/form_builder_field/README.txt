This module creates a field that uses the form_builder interface to create dynamic forms for the node_view.

See the form_builder module for for information about how to use components.

## New Hooks

hook_form_builder_field_validate(&$form, &$form_state):
  Runs on the form validation. This is seperate then the element validations.

hook_form_builder_field_action($values, &$form, &$form_state)
  Runs on form submit.  The $values contain form_id, entity and the user entered valued under form.


