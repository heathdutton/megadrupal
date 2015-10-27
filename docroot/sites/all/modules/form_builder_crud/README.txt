
The purpose of Form Builder CRUD is to provide a permanent storage mechanism
for the Form Builder module. To accomplish this, this module provides the admin
with an interface where they can assign a form_id and a description to a form
before they actually start building it. As the form is built, Form Builder will
keep it in a temporary cache, which then Form Builder CRUD will save to a
permanent storage from where it can be retrieved later.

Usage
=====
After enabling this module, go to Administer > Build > Form builder and you
will see an empty table. Click on Add to start creating your form.

Once your form is created and saved, you will see it populating the table, from
where you can administer it.

To actually access the form, call the following line from your module:

$form = form_builder_crud_form_load($form_id);
$form['#submit'][] = 'my_submission_function';

Where $form_id is the ID of the form you created with the drag-drop interface
and 'my_submission_function' should be an actual submission function that you
have previously programmed. Now you should be able to treat the $form as any
other Drupal form in your website.

Limitations
===========
* So far, the only way to call a form is through the code.
* From Builder does not provide any mechanism for custom validation or
submission.

To do
=====
* Allow the admin to set a path from where the form may be accessed.
* Add handlers to Form Builder to account for validation and submission.

Credits
=======
Author: Victor Kareh (vkareh) - http://www.vkareh.net
