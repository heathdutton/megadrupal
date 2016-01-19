Codit: Crons
============

CONTENTS
--------
 * Introduction
 * Features
 * Requirements
 * Installation
 * Configuration
 * Submodules
 * FAQ
 * Bonus Features
 * Maintainers

INTRODUCTION
------------
Codit: Crons allows a developer to add multiple jobs (functions with spoecific
actions) to the cron with each cron job having its own timing.

As with all Codit based modules, Codit: Crons' **basic premise** is that it is
easier for developers to push code changes than it is to push database changes.
So the entire module is built to support developers using code rather than admin
settings to build multiple cron jobs and have them all in one place.

**Advantages**:

* With a custom module or Feature build you can create only one
cron job with hook_cron. By using Codit_Cron you can create an unlimited number
of cron jobs without the one hook_cron limitation.
* Codit Crons uses the Drupal Queue method, so even complex cron jobs that may
time out should not keep the rest of your cron jobs from running.


FEATURES
--------
* Unlimited cron jobs using the Queue system by default.
* Drush integration for building cron jobs.
* Portable cron jobs - just copy the directory for a specific cron job to
  a new site.


REQUIREMENTS
------------

* Dependency: <a href="https://www.drupal.org/project/codit_local" target="_blank">Codit: Local</a>
* Dependency: <a href="https://www.drupal.org/project/codit" target="_blank">Codit</a>
* A working cron on your server.


INSTALLATION
------------
Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

* Drush(preferred): running 'drush codit-crons-local-init' will setup your crons directory
   inside codit_local and you are ready to build cron jobs.
* Manual: From inside this module, open boilerplate and copy the 'crons'
   directory and its contents into the Codit: Local module directory
  (codit_local/crons).  You are ready to build cron jobs.


CONFIGURATION
-------------
 * There is none. - see basic premise of Codit -

CREATING A CRON
---------------
Creating a cron job can be done either using Drush or manually:
##Drush Steps *(recommended)*:##
1. drush codit-crons-add {cron_job_name}  - This one command has created the
   cron job directory in codit_local/crons/ and created _callback.inc.
2. Your new cron job is now registered.  You can move on down to
   'Write Your Cron Job Code'.

##Manual Steps##
1. Codit: Crons are created by copying the _a_sample_cron_job directory inside
   codit_local/crons and naming it with the new cron name (lowercase and
   underscores only).
   example: cp -r codit_local/crons/_a_sample_cron_job codit_local/crons/my_job
   You must use underscores not hyphens and keep it shorter than 32 characters.
   Do not precede the name with an underscore, that will disable the cron job.
2. Flush the cache and your new cron job 'my_job' will be registered with the
   the system. You can move on down to 'Write Your Cron Job Code.'

##Write Your Cron Job Code##
1. Open the file _callback.inc within the directory named for your cron job.
   example: codit_local/crons/my_job/_callback.inc
2. Edit the value of $_time_pattern to reflect a time pattern (rule)
   See "Setting the Timing of a Cron" below for examples.
3. Edit the value of $_description to reflect a short description of what the
   cron job does. (optional - This is only used by Elysia Cron)
4. Inside the $_callback function, call any functions or do any processing you
   need.  If based on the results of what you have done or evaluated, decide
   whether to return TRUE or FALSE.  Return TRUE if you consider the cron job a
   success.  Return FALSE if you consider it incomplete or not done.


Drush Commands
--------------
* drush codit-crons-add {cron_job_name} --enable=FALSE
  Creates the cron job directory, and callback file in codit_local/crons/.
  and flushes the cron cache to register the new cron.  The optional enabled
  flag can be set to FALSE to start the cron job in the disabled state.
* drush codit-crons-enable {cron_job_name}
  If the cron job is currently disabled, it will enable it and flush the cron
  cache to register it.
* drush codit-crons-disable {cron_job_name}
  Disables the cron job if it is enabled and flushes the cron cache to
  de-register the cron.
* drush codit-crons-cache-clear-list
  Clears the Codit: Crons list cache to register or de-register a cron job.
* drush codit_crons_local_init
  Sets up the crons directory within Codit local if it does not exist.
* drush codit-crons-list
  Clears the cron cache and lists the Codit: Crons cron jobs that are enabled.


Setting the Timing of a Cron
----------------------------
The timing of a cron is controlled by a variable within the callback file.
Here are examples of the supported patterns:

* 'every 10 minutes'
* 'every 2 days'
* 'every 2 weeks
* 'every 3 months'
* 'every 2 years'
* 'every 10 days after 16:00'
* 'on the 6th of every 1 month'
* 'on the 15th of every 1 months after 16:00'
* 'on the 4th of July'
* 'on the 4th of July after 14:00'





FAQ
---
Q: How do I easily create a cron job?
 : A: View the README.md inside codit_crons which is also visible in
admin/help/codit_crons.

Q: Why do the time settings say 'after' instead of 'at'?
 : A: Cron timings on cron jobs from this module are evalatued every time cron
runs. Due to the settings of your cron, you may have a cron that only runs
every 2 hours, so the timings of the cron job can only be tested every
2 hours.  If you want your times to be more accurate, set cron to run more
often (warning this can have performance issues).

Q: Why is this better than just using Elysia Cron?
 : A: It is not better, just a nice addition.  With just using Elysia Cron, you
are still confined to typically one cron job per hook_cron(). Essentially one
cron job per module or Feature.  Any particular cron job is tied to a specific
module or Feature. To use it on another site, you'd have to re-use the entire
module.  With Codit: Crons, each cron_job can be exported to another site by
simple copy and paste.  Each cron job create in Codit: Crons can have its own
timing with or without Elysia Cron.

Q: What if I use Elysia Cron to manage the timings of my crons?
 : A:  You are welcome to.  Any cron jobs created in Codit: Crons will appear
  immediately in the cron display at /admin/config/system/cron.  Each cron job
  will have an Elysia cron rule (timing) that matches as closely as possible
  the timing specified in your Codit Crons callback.  However you can override
  the code settings in the Elysia Cron rules.

  **CAUTION:**  There are some time
  settings in Codit Crons that do can not map 100% to an Elysia Cron rule.  So
  if you are using Elysia Cron it is important that you verify that the rule
  in use is the timing you want.



ROADMAP
-------
* Add support for additional time settings.

  * 'every wednesday'
  * 'every tuesday after 18:30'
  * 'every weekday'
  * 'every weekend'
  * 'at the x:15'
* Add support for multiple settings on a single job.
* Build D8 version that will support simple migration from D7 to D8. (Assumes a
  cron job callback does not call a function that does not exist in D8.)


BONUS FEATURES
--------------
The following modules are not required, but if you have them enabled they will
improve the experience:

  * Markdown - When the Markdown filter is enabled, display of the module help
    for any of the Codit modules and submodules will be rendered with markdown.
  * Elysia Cron - Will present an admin interface for your cron jobs.

MAINTAINERS
-----------
* Steve Wirt (<a href="https://drupal.org/user/138230" target="_blank">swirt<a>)
