
# PA11Y Integration 1.x for Drupal 7.x
----------------------------
 -- SUMMARY --

 The PA11Y Integration module acts as a client for a PA11Y webservice.

A pa11y-webservice provides scheduled accessibility reports for multiple URLs. 
It runs pa11y on a list of URLs, which you can update and query the results of 
via a JSON web-service.

pa11y is your automated accessibility testing pal.
It runs HTML CodeSniffer from the command line for programmatic accessibility 
reporting.

For a full description of the module, visit the project page:
http://drupal.org/project/pa11y_integration

To submit bug reports and feature suggestions, or to track changes:
http://drupal.org/project/issues/pa11y_integration

## Requirements

A running PA11Y webservice

## Installation
------------
Can be installed like any other Drupal module -- place it in
the modules directory for your site and enable it on the `admin/build/modules` 
page.

## Usage
------------
Configure user permissions in Administration -> People -> Permissions:

 administer P11YA 

 Access the administration page for PA11Y, run tests and view the reports

In admin/config/content/pa11y_integration you specify the url for your PA11Y 
server and the standard for which to test (Section508, WCAG2A, WCAG2AA, WCAG2AAA

`Start PA11Y tests` button submits the tasks, which will submit all published 
content to the web service

`Delete all PA11Y tests` button deletes all the tasks from the webservice

A cron run will run a set of tasks on each run, which will eventually results 
in all the pages getting tested.

To change the number of tasks run per cron, change the PA11Y run cron limit. 
This is limited by the capacity of the PA11Y webservice, which does not seem to 
be able to handle high levels of task runs at once.

The PA11Y test run option is only meant to be used on very small volume sites, 
it will submit the tasks and run them at the same time, rather than use the cron
job, thereby speeding up the process. This could result in webservice crashes
however, so use with caution!

In admin/reports/pa11y you should find a table displaying the results of the 
tests run. Those tasks where the results cannot be retrieve have a 
`Run task option`,
to run each task manually.


## Future developments
------------

1. Per node testing
2. Historic testing diff
3. Ajax load of test results in report
4. Drush task

## CONTACT

Current maintainers:
    * Michel Vloon (michel_v) - https://drupal.org/user/412695

This project has been sponsored by:
    * Nomensa Ltd
    Humanising Technology. Visit http://www.nomensa.com for more information.
