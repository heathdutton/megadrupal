
iTweak Login Module
-------------------
by Ilya Ivanchenko, iva2k@yahoo.com

iTweak Login module provides very useful tweaks for the standard user login form:

* Independent of theme - works with any theme that does not already customize user login
* Select any label for the "Log in" button
* Change wording of "Create new account" link
* Change "Create new account" link to a "Register" button
* Select any label for the "Register" button
* Change "Request new password" link to a "Reset Password" button
* Select any label for the "Reset Password" button
* Change wording of "Request new password" link and change its URL (for example to a custom login help page)
* Settings are Multilingual (locale module required) and Internationalizable (i18n module required)
* Change "Username" and "Password" (if not using LoginToboggan)
* Custom labels are applied to tabs and buttons on /user/* pages for consistent look
* Ability to add text before username field in login page
* Option to remove "Request new password" link from block and user page (useful for sandbox sites where test users can't change their own password and for third party authentication like LDAP)


Installation
------------
* Copy the module's directory to your modules directory and activate the module.
* Run update.php. It will take care of registering all Internationalization variables.

Usage
-----
* Note: there is no separate settings page by this module
* Configure different options at this URL: admin/config/people/accounts
  (Administration > Configuration > People > Account settings)
* See "User Login settings" section

Internationalization
--------------------
i18n module is required for internationalization.

Internationalization handbook (section on Multilingual Variables) recommends changing settings.php to register certain variables for internationalization of the site. Doing that as recommended will break itweak_login internationalization. Instead, do a conditional registration (using if()).
