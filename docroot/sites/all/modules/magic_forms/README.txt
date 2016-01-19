Magic forms
-----------------------------

The magic forms module supplies several FAPI tweeks allowing for easier
customization of forms.

Features include:

* Custom error messages/management
  * [ ] Custom styles
  * [+] Errors displayed within the field wrapper
  * [+] Errors displayed at the top with anchors to the fields
  * Field validators
    * [+] Url
    * [+] Url, scheme and domain
    * [+] Email
    * [+] Email and domain
    * [ ] Number
    * [ ] RegEx
  * [+] Custom error messages
  * [+] Error field wrapper with custom class
* AJAX
  * [ ] AJAX field validation
  * [ ] AJAX submission
* Form helpers
  * [ ] Default form button (enter to submit)
  * [+] Set #collapsible if #collapsed is set (and TRUE)
* HTML5
  * [+] HTML5 required attribute
  * [ ] Placeholders (HTML5 placeholder + JQuery placeholder)
  * Inputs
    * [+] Email
    * [+] Url
    * [+] Number
* Admin UI
  * [ ] Apply form config via form-id
  * [ ] Apply field config via form-id and name
* [+] Example form

The configuration can be applied to the form using FAPI (hook_form,
hook_form_alter) or using the UI.
