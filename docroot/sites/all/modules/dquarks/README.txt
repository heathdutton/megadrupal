Description
-----------
This module adds a dquarks content type to your Drupal site.
A dquarks can be a questionnaire, request form... These can be used 
by visitor to make contact or to enable a more complex survey than polls
provide. Submissions from a dquarks are saved in a database table and 
can optionally be mailed to e-mail addresses upon submission.

DQuarks includes:
- Creating quiz and surveys
- Showing correct answer and score of users
- Notify administrator after submission
- Download results into excel file
- Notify users by sending him emails
- Manage rules of direction: the form will be created based on responses 
recorded during the primary configuration.

Requirements
------------
Drupal 7.x

Installation
------------
1. Copy the entire dquarks directory the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. (Optional) Edit the settings under "Administer" -> "Configuration" ->
   "Content authoring" -> "dquarks settings"

4. Create a dquarks node at node/add/dquarks.
