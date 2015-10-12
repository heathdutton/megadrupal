Description
-----------
This module provides a way for users to reply to webform submissions within the 
CMS by adding a 'reply' link on webform submissions. Permissions can be set to 
allow users to reply to all webform submissions or only those on a node the 
user has created. All emails sent are stored in the database and can be viewed 
from the submission page.


Requirements
------------
Drupal 7.x
Webform 7.x

Installation
------------
1.Copy the webform_email_reply directory the Drupal sites/all/modules directory.

2.Login as an administrator. Enable the module in the "Administer" -> "Modules".

3.Enable the required permissions, the two options are:
  a.View and send email replies to all webforms
  Allows user to view and send emails in reply to any webform.
  b. View and send email replies to own webforms
  Allows user to view and send emails in reply to a webform the author has 
  created.

4.Under the 'Form Settings' tab select which email field you want to use as the
default 'To' address when replying to an email.

5.Now when looking at a webform submission you'll see an "Email reply" link and 
tab on the page.
