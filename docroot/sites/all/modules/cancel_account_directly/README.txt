CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration


INTRODUCTION
------------

Current Maintainer: Viswanath Polaki <polaki_2005@yahoo.com>

Cancel account directly module helps users to cancel their account without
filling required fields in their profile form.

Users who have access to cancel their accounts or other accounts can skip
profile field validations (required field validations) to cancel users
account.

Case where this module will be very useful are as follows:
During site building when profile fields are increased or changed from non
required field to required field, many users who already created their account
have to refill their required profile fields to cancel account in profile
edit form. This is default functionality of account cancellation in drupal.

But as the user is canceling his account there is no sense in asking user to
fill the profile fields before canceling the account. He should be able to
cancel account directly.


INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
Based on "cancel account" or "administer users" permissions the cancel account
in the profile edit form is displayed replacing native cancel account button.
