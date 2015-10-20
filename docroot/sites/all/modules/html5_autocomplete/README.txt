HTML5 AUTOCOMPLETE
==================

This module allows HTML5 autocomplete attributes to be set for form input
elements to help browsers to autofill them. Alternatively, disable autofill
suggestions if your webform fields contain sensitive information.

Background information & browser support
----------------------------------------
The Web Hypertext Application Technology Working Group (WHATWG) has proposed
extending the autocomplete attribute in HTML5 to support 'typed' autocomplete
fields -- see http://wiki.whatwg.org/wiki/FAQ#What_is_the_WHATWG.3F
You can specify what kind of data a field expects with this attribute, such as
'shipping province' so that users can just use their browser's autofill
suggestions rather than manually entering all their data.

Most modern browsers can be dissuaded from providing autofill suggestions for
input fields that have the attribute set to 'off', which may be useful for
security if a field may contain sensitive information that should not appear in
autocomplete suggestions.

Google Chrome implements named autocomplete types, although it will only do so
when there are at least 3 fields on a page using the autocomplete attribute.
Internet Explorer uses vCard information - see
http://msdn.microsoft.com/en-us/library/ms533032.aspx#implement - in a similar
way using the vcard_name attribute instead of autocomplete. This module allows
this attribute to be set too.
Unfortunately, these are currently the only browsers that allow setting hints
for autocomplete suggestions.

Module dependencies
-------------------
* Webform
* Form Builder Webform UI (part of the form_builder project)

This module currently only implements its functionality for Webform textfield,
email and number components using the Form Builder Webform UI.

Example usage
-------------
When editing/adding a textfield, number or email component on a webform in the
form builder UI, click the component to edit its details and set the
autocomplete type and/or vCard names. Save the form. In Chrome, there needs to
be at least 3 fields with an autocomplete type. In Internet Explorer, only the
vCard name setting is recognised. See the links above for information on each of
those browsers' implementation and lists of their supported attribute values.
View the webform and double-click the fields to see autofill suggestions based
on previous forms. If there is no information, it may be that there are no
suggestions. Try creating another similar form with at least 3 fields (but
potentially with different names/labels) and the same autocomplete types / vCard
names, and submitting that with data first - you should then have some
suggestions for each of the fields that have an autocomplete type / vCard name.

About the Developers
--------------------
This project is currently maintained by developers at ComputerMinds - visit us
at http://www.computerminds.co.uk. We at ComputerMinds pride ourselves on
offering quality Drupal training, development and consulting. Go Drupal!
