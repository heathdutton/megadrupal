Overview
========
The Webform A/B Test module creates a new node type (appropriately called "Webform A/B Test"). To build a test, create a Webform A/B Test node as you would with any other content type. When a user visits the URL for this test node, they will be redirected to one of the forms included in the test (selected at random). Once a form has been declared the winner of the test, all users who visit the test will be redirected to that form, and the others will be "retired."


Creating a Test
===============
Options when creating or editing a Test
---------------------------------------
### A/B Test Settings
This fieldset includes basic information about the Test.

* Test Name: This is the title of the Test node, so it will be used in content lists and shown on the admin overview for the test.
* Admin Description: A brief description of the Test. This is only shown to admin users who have permission to edit Webform A/B Test nodes.
* Public Teaser: Teaser text to show to unpriveleged users when the node is shown in teaser mode, such as a list of nodes or if it's promoted to the home page.
* Which types of webforms will this test use: A Test can only be run with a single type of webform. By default, this list only includes Standard Webforms, but other modules can define other types of webforms to use in Tests. For example, the Fundraiser A/B Test module (not yet released in contrib) allows Fundraiser forms to be used for a Test.

### Winning Thresholds
Before the winning conditions (detailed below) are ever checked, every form in the test must meet all of the enabled thresholds. Typically, this will at least include the Minimum Hits option, to ensure that all of the forms in the test are viewed some number of times before a winner is declared.

To understand why this is important, consider a test where the winning condition is a conversion rate of 50%. If the first person who visits the test submits the form, the conversion rate for that form will be 100%. That doesn't mean the form should be declared the winner already, though: the other forms haven't even been seen by users, so how can anyone know that this one is better than the others?

To prevent this, winning thresholds should be configured so that the winning conditions aren't even considered until enough data has been collected about the forms' performance.

To learn more about defining your own winning thresholds, see API.txt.

### Winning Conditions
The first form to meet all of the winning conditions will be declared the winner, and any users who visit the test node will be redirected to that winning form. Any number of winning conditions can be enabled for a test.

Some winning conditions are:

* Total Conversions: Specify a number of conversions that must be reached. If set to 20, a form will win once 20 users successfully submit the form.
* Conversion Percentage: Specify a percentage of hits that must result in conversions. If set to 40% (or 0.4), the form will win once 40% of the users who are presented the form successfully submit it.

For more information on defining your own winning conditions, see API.txt.

### Other Options
Like every other node, you can also set author information, menu settings, and so on. Note that if the test is not published, it will not be available to regular users.


Included Webforms
-----------------
An A/B Test needs to include some webforms. After creating the node, visit the Included Webforms tab to add the ones you want to include in the Test. Use the Add Webform dropdown to choose the forms to be included. Note that this dropdown will only include forms that can be included in this Test, based on what was chosen for "Which types of webforms will this test use?" If the Test was setup to use Fundraiser forms, then only Fundraiser forms will be listed in the dropdown.

Included webforms are listed here, with links to view or edit the webform node, or to remove them from the A/B test.


Viewing a Test
==============
Regular Users
-------------
### Full node

When a regular user visits the Webform A/B Test node, one of the forms included in the Test will be chosen at random, and the user will be redirected there. Once they have visited the Test this first time, they will always be redirected to the same form.

If one of the included forms has already 'won' the Test, then users will always be redirected there.

### Teaser
If only the node teaser should be displayed (for example, if the node is promoted to the front page or included in a View), the Public Teaser field is displayed instead of redirecting to one of the included forms.


Admin Users
-----------
When an admin with edit rights on the Test visits the node, they will see an overview page with some basic details about the Test. A link at the top of the page gives them to option to view the test as a regular user would, for the sake of testing.


Reports and Monitoring
======================
The Reports and Monitoring tab on a Test shows a summary report of data collected in the Test. Each webform included in the Test is listed with some information about how they are performing. The Reset Data tab will wipe all data about hits and conversions on forms in the Test and restart the Test (after confirming that you want to do so).

The following information is included in the report:

* Status: Active, Winner, or Retired. Forms start out Active, until a form is declared the winner. At that time, one will be listed as Winner, and the rest will be Retired.
* Name: Title of the webform node.
* Hits: Number of hits to the webform through this test. By default, the module only logs one hit per user per form, but this option can be changed in the Webform A/B Test Settings (see next section).
* Conversions: Total number of successful submissions made to the form.
* Conversion %: Percentage of hits that resulted in successful submissions.

The following report columns only apply to the winning webform:

* Pre-win Hits: Total number of hits before being declared the winner.
* Pre-win Conversions: Total number of successful submissions before being declared the winner.
* Pre-win Conversion %: Percentage of hits that resulted in successful submissions before being declared the winner.
* Post-win Hits: Total number of hits after being declared the winner.
* Post-win Conversions: Total number of successful submissions after being declared the winner.
* Post-win Conversion %: Percentage of hits that resulted in successful submissions after being declared the winner.


Webform A/B Test Settings
=========================
The Webform A/B Test Settings form is found under Administer > Site Configuration. Here, admins can configure some basic settings that affect all A/B Tests.

* Roles to ignore hits from: Typically, hits from admins should not be counted to determine a winning form, since they are likely just testing it out.
* IPs to ignore hits from: This allows more generic blocking of hits from administrators. By adding an admin's IP to this list, you can ensure that hits and conversions from that person will not be logged even if they log out of Drupal to try out the Test as an anonymous user.
* Log only one hit per user per form: By default, the module will only log one hit per user for each form that they hit (for authenticated users, it's based on their user ID. For anonymous users, it's based on their IP). This prevents the hit count from being artificially inflated if someone is refreshing the page or something like that. When this option is disabled, ALL webform hits will be logged. This can be useful for testing the configuration of a Test. Typically, one should then re-enable this feature and reset the Test data before publishing it.


Planned Features
================
* Notifications: Allow admins to configure notifications for a Test:
	* Daily summary of Test by email or SMS
	* Notice that a form has won, by email or SMS
* Option to 'start' a test and lock down changes from that point forward.
