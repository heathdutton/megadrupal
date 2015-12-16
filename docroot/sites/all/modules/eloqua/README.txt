Description:
------------
This module expects an existing subscription with eloqua.com. If you do not have
a subscription with eloqua.com at this point, please go to eloqua.com and review
purchasing options if you wish to continue using this module.

The Eloqua module will enable visitor tracking. For webform integration you will
want to enable the Eloqua webform module.

The Eloqua webform module will allow you to create webforms and link them to
your existing Eloqua forms.  At present, this module requires you to still set
up the form in both Eloqua (using their form builder) and in Drupal
(using webform). This may change at a later date, but will at least provide a
more Drupal-esque way to create forms for use with Eloqua.

Requirements:
-------------
Webform module must installed and enabled.

Use:
----
Form:
When creating a webform, a new checkbox should exist on the webform tab form
settings allowing you to make the particular webform node Eloqua enabled. After
checking the box, there is an "Eloqua Form Name" field that should map exactly
to the form name on Eloqua.

Fields:
After you have created the webform and made it Eloqua enabled, when creating new
form components (fields), a new field exists for identifying the Eloqua name of
that field ("Eloqua Field Name"). An example of use: your form might have a
First Name field. When you create the textfield for First Name, Drupal assigns
it the name of first_name (webform only allows lowercase alphanumeric +
underscores), but your field in Eloqua might be First-Name. Drupal/Webform will
have first_name as the "Field Key", but you can set the "Eloqua Form Name" to
First-Name so that, when submitted to Eloqua, this field maps over exactly as
expected

URL Parameters
--------------
Using URL Parameters instead of relying on Token values for the default value
provide a better experience when dealing with compound values that cannot
be expressed simply.

This functionality can be extended by implementing the function
   _eloqua_form_url_map_{type}($value, &$element)
where type is the webform component type.

When using the URL Parameter functionality to define the default values, rather
than token replacement in the default-value, the following rules are in effect.

Type:
 time: Express as a 24 hour value in the format HH:mm.   ie: &t=23:59
 date: Express as an ISO date in the format YYYY-mm-dd.  ie: &h=2010-12-18
 radio: Express as a simple value.  ie: &s=AK
 checkboxes: Use array notation.   ie: &c[]=opt1&c[]=opt2
