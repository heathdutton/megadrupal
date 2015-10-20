Allows users to bypass form validations and required fields.

This module adds 2 new permissions:

- Bypass Form Validations
- Bypass Form Required Fields

By default, this module will only act on NODE forms.

You can use it as an API to remove form validations or required fields
using any custom logic that suits your application.

Simply call:

Drupal\bypass_form_validations\FormBypasser::RemoveFormValidations(&$form, $form_id);

or

Drupal\bypass_form_validations\FormBypasser::RemoveRequiredFields(&$form, $form_id);

You can add a custom logic through a hook:

hook_bypass_form_validations($form, $form_id)

just return TRUE in your custom hook if the form should have form validations
and required fields removed.

