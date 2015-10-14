Form save module

Features
--------

Allows you to submit forms using either Ctrl+S or Cmd+S depending on OS.
The module also takes into account multiple forms on the page and manages
form focus to make sure the correct form is submitted.


Requirements
------------

None


Installation instructions
-------------------------

Enable the Form Save module on your site.


Usage
-----

Whilst on a form on the site press either Ctrl+S or Cmd+S to save the form.

If you wish to force a button as the default, you can do so when declaring
the button in your form code by adding '#default_button' => TRUE. If you don't
set this, the module will automatically select the first button it comes across.

Example:

$form['my_button'] = array(
  '#type' => 'submit',
  '#value' => t('My Button'),
  '#default_button' => TRUE,
);


Bugs, Features and Support
--------------------------

For all bugs, feature requests or support requests please use the 
Form Save issue queue at http://drupal.org/project/issues/form_save