
BASICS ___

The Context Form Alteration module is a front-end, friendly alternative
to writing form alters in code. You can set form values based on any
context condition available via core or contrib plugins. This is mostly
useful for hidden fields, but other default values can be nice depending
on the area of your site, user roles, etc.

The purpose of this module is to allow non-programmers an easy way to
make changes to forms. It's also a preferrable way for experienced
programmers to make small changes to forms without always going the module
route. Additionally placing form changes in features can be great for
Drupal distrobutions to reduce custom distro module code.



EXAMPLES ___

Change the button text on user registration...

Form ID                 user_register_form
Parent Hierarchy        actions, submit
Attribute               #value
Value                   Sign Up
i18n                    TRUE

Preset a profile field within user registration...

Form ID                 user_register_form
Parent Hierarchy        profile_about_you, field_profile_industry
Attribute               #default_value
Value                   High Tech

Force a few specific content types into the advanced serach options...

Form ID                 search_form
Parent Hierarchy        advanced, type, #options
Attribute               #options
Value                   my_content_type

Form ID                 search_form
Parent Hierarchy        advanced, type, #options
Attribute               #options
Value                   another_type

Add a note under your language drop within untranslated pages...

Form ID                 lang_dropdown_form
Parent Hierarchy
Attribute               #suffix
Value                   This page has not yet been translated.
Append                  TRUE
i18n                    TRUE



BONUS NOTES ___

1. Multiple Forms - You can alter mulitple forms by entering them
   as comma separated values.

2. Empty Parent - You can edit the form object by leaving the parent
   column empty.

3. Allow your value strings to be registered for string translation
   by marking the setting as such.

4. Append to existing form components. This includes both string values
   and items added to an array (ie. #attributes, class).



GOTCHA! ____

AJAX Forms:

Forms with AJAX submits can be troublesome. Don't go altering AJAX powered
fields willy-nilly, the cached form state can make your head spin. Changing
the value of a submit can cause a form to fail. Also, context only runs
when the page is being built, so your alterations won't last through the
AJAX processing later.


Validation:

You should not use this module to alter validation, or the way a field
operates as it realted to calidation. Context will not always fire when
the page is reloaded due to rendiner order and various caching layers.
For more complex form alters create a custom module.


Strings & Arrays:

When adding a element to a form which should be an array (ex: classes,
queires, etc.) leave the attribute empty and add the last element to
the parent.

Here's the error you might see...
Fatal error: [] operator not supported for strings in /path/to/some_file.inc

This happens because the context reaction is creating a new element and
it defaults to a string. Sometimes this needs to be an array and often
processes down the line rely on that. When viewed in Feature form here
is an example circumstance...

CAUSES ERROR:
0 => array(
  'form_id' => 'user_register_form',
  'parents' => 'actions, submit, #attributes',
  'attribute' => 'class',
  'value' => 'btn-primary',
  'append' => 1,
),

FIXED:
0 => array(
  'form_id' => 'user_register_form, some_other_form',
  'parents' => '#attributes, class',
  'attribute' => '',
  'value' => 'my-class',
  'append' => 1,
  'i18n' => 0,
),



BEGINNERS: USING THE FORM API ___

You'll need to find the actual form array heirarchy in order to traverse
down through any fieldset or other structure down to the element you are
intending to alter. This can sometimes be done by inspecting the HTML of
a form, but often you'll probably need to do the following:

Install the Devel module, which allows outputting structured objects in
super readable and interactive ways. Get that here...
http://drupal.org/project/devel

Create a custom module to output form details (only in your development
environment). You'll only need this...

FILE: mymodule.module
<?php
function mymodule_form_alter(&$form, &$form_state, $form_id) {
    dsm($form);
}

...which will output the entire form structure for you to browse.



Happy altering.
