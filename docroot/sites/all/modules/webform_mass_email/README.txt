
REQUIREMENTS
------------

- Webform module
- Cron jobs up and running (more information: http://drupal.org/cron).


ABOUT
------------

Webform Mass Email provides a functionality to send mass email for the subscribers
of a webform. You can select which email field from the result you would like to
use as the recipient's address.

This module uses Drupal's 'queue' table and cron jobs to send the emails
for users.


INSTALLATION
------------

- Enable the Webform Mass Email module at admin/modules.

- Configure Webform Mass Email permissions at admin/people/permissions and
  assign permission to send mass mail for the roles you want.

    - Administer Webform Mass Email
      - Allow user to administer all of the module configurations
        at admin/config/content/webform-mass-email.

    - Send mass mail
      - Allow user to access the Webform result/mass emailing form
        at node/NID/webform-results/email (Note: To view this page, you
        will also need to set the Webform permissions to view the results.

- Configure the module settings at admin/config/content/webform-mass-email.

    Cron throttle
      - How many messages are being sent per cron run.
    
    Mail expiration
      - Sometimes something might go wrong and the messages weren't sent as they
      were supposed to. This determines how long a (failed) email is stored at
      the Drupal 'queue' table before it is deleted. (Default: 600 seconds).

    Log emails
      - Should the emails be logged to the system log (admin/reports/dblog).
        Note that sending many (hundreds of) messages will fill up your log
        pretty badly. This is good for debugging.


SENDING MASS MAIL
------------

- To send mass mail, navigate to your Webform node (node/NID).
- Go to 'Results' tab and then to the 'Mass email' sub-tab.
- You'll see the Webform Mass Email for with some fields in it and the Webform
  results below it.

  Email field
    - The field that is going to be used as the recipient's email address.
      Auto-populates from the Webform's email components.

  Subject
    - Subject for your email.

  Body
    - Body for your email.
