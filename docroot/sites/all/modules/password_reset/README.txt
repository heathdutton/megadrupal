Overview
--------
The password_reset module allows for passwords to be reset through the use of
security questions and user provided answers. Users are asked to choose a
question and provide an answer for it. If they lose their passwords, they can
get back into the system via a one-time login URL which is provided only if they
answer the security question associated with their account correctly. Once they
log back into the system, they are allowed to reset their password.

This module supports the following features:
  * Add (and manage) preset questions for users to choose from.
  * Allow for case-sensitive/insensitive answers.
  * Control the format of answers using regular expression checks.
  * Store answers in hashed form similar to Drupal core passwords. The module
    uses core's pluggable password.inc while doing so. 
  * Track the usage of each question.
  * Optionally allow users to choose their security question during registration
    and later, to manage it from their account management pages. 
  * Optionally redirect preexisting users who have not chosen their question to
    their account management page.
  * Optionally require users to enter their current password when adding or
    modifying their security questions.
  * Flood control to prevent abuse of the password reset form.
  * Security precautions which ensure that information about valid usernames is
    not inadverdently revealed through the password reset form.

Project navigation
------------------

Settings: Administration -> Configuration -> People 
  -> Password reset -> Settings
Manage questions: Administration -> Configuration -> People 
  -> Password reset -> List
Add questions: Administration -> Configuration -> People 
  -> Password reset -> Add question

Author
------
Karthik Kumar / Zen / |gatsby| : http://drupal.org/user/21209

Links
-----
Project page: http://drupal.org/project/password-reset
