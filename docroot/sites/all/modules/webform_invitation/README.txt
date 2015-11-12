-- SUMMARY --

The Webform Invitation module allows you to restrict submissions to a webform
by generating codes which may then be distributed e.g. by email to
participants. Upon activating the invitation mode for a webform a new component
will automatically be added to the webform where the code has to be entered.
The code may also be submitted by adding ?code=[CODE] to the webform's url.
When the participant hits the submit button, the code will be checked for 
validity.


-- REQUIREMENTS --

Webform module 7.x-3.x
This module has been developed while using Webform module 7.x-3.18.


-- INSTALLATION --

Install as usual.


-- CONFIGURATION --

No special configuration necessary. Works out of the box.


-- USAGE --

When installed, this module adds a new tab to webforms called "Invitation" 
(next to "View", "Edit", "Webform", and "Results").
There are four sub-tabs called "Settings" (default action), "List codes", 
"Generate", and "Download".

The "Settings" tab currently only contains the option to enable or disable 
invitations for the current webform.
"List codes" shows a simple list of all invitation codes that have been
generated for the current webform and whether or not they have been used.
"Generate" provides a form to generate invitation codes. Currently, you may
only choose the number of generated codes.
"Download" tab has no special content but lets you download the list of 
generated codes as an Excel sheet including code and full url to access the 
webform with auto-submitting the invitation code. 