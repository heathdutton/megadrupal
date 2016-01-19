User Priority Ranking

https://drupal.org/project/user_priority_ranking

Maintainer: iStryker (Tyler Struyk)
https://drupal.org/user/303676

Module Development Sponsored by the University of Waterloo

***

This module provides a function to sort a list of content based on priority the user gives them.

Installation
------------
1) Copy the user_priority_ranking directory to the modules folder in your installation.

2) Enable the module using Administer -> Modules (/admin/build/modules)

Configuration
-------------
The configuration for User Priority Ranking is spread between Views configuration
and the Flag site building page. To configure:

1) Configure the flags for your site at
   Administer -> Structure -> Flags (/admin/structure/flags)

   You can create and edit flags on this page. Descriptions for the various
   options are provided below each field on the flag edit form.

   You must checkoff the checkbox called 'Allow the flag to be prioritized' for 
   Drupal to save the prioritized values to the database

2) Configure the flag limit value for your flag
   Administer -> Structure -> Flags -> Limits (/admin/structure/flags/limit)
   
   You must set the flag limit value for your flag, otherwise the module will not
   work if you prioritize more than 10 things.
   @TODO fix the 10 thing limit
   
3) Go to the Views building pages at
   Administer -> Site Building -> Views (/admin/structure/views)

   Install the User Priority Ranking Example module/feature that comes with this 
   project to get you started organizing your user priority rankings. You can 
   override the view or use it as a template to control the display of your
   priorities.
   
How I create what is in the example feature module 
--------------------------------------------------

1.)  Create new content type call Toys (admin/structure/types/add).
     Follow the steps.  There is nothing special

2.)  Create a flag called wants (admin/structure/flags/add)
       Flag name: 'wants'
       Flag type: Nodes
       Then click 'Submit'
       Title: 'The things I want'
       Global flag: unchecked
       Allow the flag to be prioritized: checked
       Messages
         Flag link text:  I want this
         Unflag link text: I do not want this anymore
         Flaggable content: check Toys (and only Toys)
       Click Submit at the bottom

3.)   Set the flag limit (admin/structure/flags/limits)
       Check 'Impose a Limit on Things I want'
       Things I want Limit: 3
       Save

4.)   Create a view (admin/structure/views/add)
        View name: 'User Priority Test Example'
        Item to display: 0 (for Unlimited)
        Check 'Create a block'
        Block title: 'Things I want'
        Display format: Table
        Items per page: 0 (for Unlimited)
        Click 'Continue & edit'

...@TODO finish documented steps 

Support
-------
If you experience a problem with User Priority Ranking or have a problem, file a
request or issue on the flag_weights queue at
http://drupal.org/project/issues/user_priority_ranking. DO NOT POST IN THE FORUMS.
Posting in the issue queues is a direct line of communication with the module
authors.

No guarantee is provided with this software, no matter how critical your
information, module authors are not responsible for damage caused by this
software or obligated in any way to correct problems you may experience.

Licensed under the GPL 2.0.
http://www.gnu.org/licenses/gpl-2.0.txt