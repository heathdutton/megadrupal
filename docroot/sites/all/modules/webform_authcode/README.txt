#
# Webform Authorization Code by ericmulder1980
#
# Documentation created : 14-06-2013
# Documentation last changed : 14-06-2013

# Description

The webform module provides ways to limit access to a webform based on user roles, a maximum number of submissions and more. If you want to limit access to a webform but still want it to be accessible for anonymous users, the webform authorization code module is right for you.

# Features

+ Adds a pass phrase option to the webform configuration form.
+ Hides the webform and node content when a pass phrase has been set and displays a pass phare form to the visitor.
+ Displays the protected webform only after the user has entered the right pass phrase.
+ Currently supports one pass phrase per webform
+ After entering a valid pass phrase it is stored in the users cookie. The user will be able to access the form again without having to re-validate. The cookie lifetime is set either in php.ini of settings.php
+ If the pass phrase is altered in the webform settings, all users will have to re-validate.

# Installation

1) Unpack the module into your modules directory.
2) Enable the module in your module overview page or use drush pm-enable webform_authcode

# Usage

1) Open the node containing your webform
2) Click the webform tab (primary tab)
3) Click the Form settings tab (secondary tab)
4) At the bottom of the submission settings fieldset find the 'Protect this form' checkbox
5) Check the 'Protect this form' checkbox, a textfield will appear
6) Enter your pass phrase to protect the webform.
7) At the bottom of the page, click the Save configuration button.
8) Click the View tab (primary tab) to see the results

# Questions?

Please use the modules issue queue located at https://drupal.org/project/issues/webform_authcode
