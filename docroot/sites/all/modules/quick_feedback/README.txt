
Quick Feedback module


Installation:

1) Enable the module.
https://drupal.org/documentation/install/modules-themes

2) Drop the UA Parser library in your libraries folder. Get that here...
https://github.com/faisalman/ua-parser-js
It should wind up here: /sites/all/libraries/ua-parser-js

3) Configure the module.
Find that here: admin/config/system/quick-feedback


Adding forms:

You may want to add more forms to the list that get a feedback link. The easy
way to do this is to add the form id within the admin form. Done.

If you want a toggle checkbox you can add a form... to the form, with a module.

function MYMODULE_form_quick_feedback_settings_form_alter(&$form, &$form_state) {
  $form['forms']['forms_list']['#options']['my_module_important_form'] = t('My Important Form');
}
