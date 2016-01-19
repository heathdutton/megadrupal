INTRODUCTION
============

Webform Campaign Monitor provides a webform component in checkbox format
that lets you subscribe users to specific lists on Campaign Monitor
(http://www.campaignmonitor.com/).

With Webform Campaign Monitor you can:

 1. Subscribe users to as many lists as you want.
 2. Map webform components to Campaign Monitor fields (including custom ones)
    for each one of the lists!

See https://drupal.org/project/webform_campaignmonitor for more information


INSTALLATION
============

 1. Install the module and dependencies.
 2. Configure Campaign Monitor module at admin/config/services/campaignmonitor
 3. Make sure you run cron to pull out the availale lists.
 4. Create a webform with textfield and email components to capture the users'
    name and email address.
 5. Add a Campaign Monitor component, tick the boxes that correspond to the
    list(s) the users will be subscribed to.
 6. For each one of the lists selected in the step above, map the Campaign
    Monitor fields to previously created webform components.
 6. Save the component and give it a go!


DEPENDENCIES
============

Webform Campaign Monitor depends on the following modules:

 *  webform
 *  campaignmonitor (For the campaign monitor integration functionality)


MAINTAINERS
===========

 *  jorgegc <https://www.drupal.org/u/jorgegc>
