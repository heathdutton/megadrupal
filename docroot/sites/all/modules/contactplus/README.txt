Contact Plus augments the core Contact module with several features.

* Allows redirection of the user to a custom path, per category, for the
  site-wide contact form/module.
* Allows for disabling/hiding of form elements on the contact form -
  if you have an all-anon user base or all-auth or a mix, there is a combination
  available to trim down the amount of fields required to get from empty form to
  submission ASAP, or simply hide fields that aren't required in certain circumstances.

Instructions:

Contact Plus adds one additional field to each category settings page. On this
page you will now find a "redirect path" field. Here you can set the drupal
path to redirect to when the form is submitted. This enables an administrator to
ensure that the end user is presented with information that is pertinent to the
category they have submitted the form with. This is useful for presenting "extra info"
pages such as targeted sales information, for example.

Contact Plus adds an additional tab, "Settings" to the contact module's default
configuration. The settings here are one-time only - they work for all categories.
These are as follows.

For "Alter name field" and "Alter sent-from address" there are several options:
1 Leave the name field as per default
  - Leave the name field on the form, make no alteration.
2 Make the name field visible but not editable
  - Leave the name field on the form, but grey it out - the form element is uneditable.
    Useful if your site uses only authenticated users, or only authenticated users can
    use the contact form. These users always have a default value set in the name field.
3 Make the name field visible but not editable, but only if the field already has a value
  - Same as 2, but if the field has no default value (ie you're allowing anonmyous users
    to use the contact form) then display the blank field.
4 Hide the name field completely
  - Same as 2, but remove the form completely instead of displaying it greyed out.
5 Hide the name field completely, but only if the field already has a value
  - See 3 & 4 :)

The other two options are "remove subject field" and "remove copy option". The first
checkbox, when selected, will completely remove the subject field (not sure why you'd
want to - maybe just make the form as short as possible?) Note that a value will still
be placed in the email (that of [category name]) so the email is perfectly valid. It
simply removes the extra option. The second checkbox naturally removes the option to
send an extra copy to the user of the form. Some people just don't want that option
available.

NB There is an existing D5/6 module called http://drupal.org/project/contact_redirect
which is now out of date. I originally wrote similar code several years ago and
just recently found and updated it. This module supercedes that one as it adds a
lot of new features.
