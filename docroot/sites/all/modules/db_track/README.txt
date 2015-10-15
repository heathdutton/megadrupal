README.txt
==========

INTRODUCTION
------------
  Module provides additional log of non-tracking Admin panel entities
  and elements, such as changes in configuration settings on admin/config.
  Different system events, like adding new content types or switching
  modules off, already tracked by system Database logging module.
  Using of DB Track can provide more complete history of working session.
  Idea of creating this project has appeared during working on maintain
  around 50 sites with three environments on each, when sometimes we cannot
  remember all changes made on one environment and simply write them in notes.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/andrdrx/2432537


REQUIREMENTS & INSTALLATION
---------------------------
 * Simply download and turn on this module - it doesn't need any dependencies or
   additional steps. Just install it in way you prefer and enjoy it!.

CONFIGURATION
-------------
 * Configure user permissions in Administration >> Configuration >>
   Development >> DB Track:

   - Exclude forms which you don't want to track by URL ;
   Some pages will not be track because they already handle by
   other modules. You can find  report about content, modules or 
   people events here /admin/reports/dblog. Please see the list 
   of hardcoded exclude pages:
   - admin/content
   - admin/modules
   - admin/people
   - admin/modules/list/confirm
   - admin/reports/db_track

REPORTING
---------
 * View report  in Administration >> Reports >>
   DB Track

   - View current report;
   - Clean current report;

AUTHOR/MAINTAINER
-----------------
Current maintainers:
 * Yaremiy Roman - https://drupal.org/u/andrdrx
 * Yuriy Kostin - https://drupal.org/u/yuriy.kostin

SUPPORTING ORGANIZATION
-----------------------
This project supporting by:
 * Blink Reaction
   https://drupal.org/marketplace/blink-reaction

   Blink Reaction is a leader in Enterprise Drupal Development, delivering
   robust, high-performance websites for dynamic companies. Blink creates
   scalable and flexible web solutions that provide the best in customer
   experience and meet brand, marketing, and business goals.
