
OVERVIEW

Simpletest automator
Allows an end-user to create a simpletest via the Drupal UI.


WHAT THE MODULE DOES

Install the module and create a simpletest file by clicking around the Drupal UI. The user specifies the conditions under which they will run the simpletest (e.g. a user with a specific set of permissions), then creates the simpletest by completing the activity in the Drupal UI.

    * Allows you to specify a user with configurable permissions.
    * Switches you over to be that user.
    * Records the links you click (and other pages you visit).
    * Saves form submissions into a file in you files/ directory
    * Adds assertions made from the content of the result -- not the blocks, just the content itself.


HOW TO INSTALL THE MODULE

1. Untar the module in your modules directory (usually <drupalroot>/sites/all/modules/).

2. Visit Administer >> Site Building >> Modules

3. Check the box next to Automator to activate the module.


HOW TO USE THE MODULE

1. Visit Drupal administration.

2. Click on the Automator link
Administer >> Site Building >> Automator

3. For the test you are about to create, provide a description, machine readable name, human readable name, and group.

4. Specify the permissions for the user.

5. Click "Start testing with this user"

6. Complete your test

7. Click on "Simpletest Automator Stop" once you have completed your test.

8. Your test will be stored in the Drupal files/ directory.