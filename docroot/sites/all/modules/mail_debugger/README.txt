# Mail debugger
A module to test the mail system

# Features
* Send user account mails
* Send custom mails
* API for integration with other modules, like commerce (todo!)

# API
Extend this module by implementing one of these hooks:

## hook_mail_debugger_info
* include - (optional) An include file array, enter the parameters from module_load_include
* title - Tab title
* form - Form elements callback
* submit - Execute sending the mail
* validate (optional) - validate the values
* weight (optional) - Sort order of tabs

## hook_mail_debugger_info_alter
* Change any of the provided tabs

## form callback
Please note: don't define required fields. Use a validate callback to do so.
function form_callback($default_values, &$form_state) {
  // $default_values: array of default values
  // $form_state: access to the form_state parameter
}

## validate callback
function form_callback($values, $element, &$form_state) {
  // $values: array of values
  // $element: element name for each value for use in the form_set_error function.
  // $form_state: access to the form_state parameter
}

## submit callback
function form_callback($values, &form_state) {
  // $values: array of values
  // $form_state: access to the form_state parameter
}
