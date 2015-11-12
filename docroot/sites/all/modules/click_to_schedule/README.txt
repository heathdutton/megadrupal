INTRODUCTION
------------

Allows visitors to your web site to schedule appointments for phone calls or
meetings with your sales or service team. You control which time slots are
offered and the duration. Appointments are distributed in a round robin manner
across your team. Includes two-way sync with team member's personal calendars to
prevent double booking.


INSTALLATION AND USE
--------------------

1. Make sure you have the module Libraries API installed.


2. Download the code for the Fancybox plugin from
   http://fancybox.googlecode.com/files/jquery.fancybox-1.3.4.zip.
   Extract the plugin archive and copy the folder with the plugin files
   (containing jquery.fancybox-[version].js file) to sites/all/libraries
   (recommended) so the actual plugin can be found at
   sites/all/libraries/fancybox/jquery.fancybox-[version].js.

3. Download the module code to the sites/all/modules directory within your
   Drupal installation.

4. At admin/modules enable the module.

5. Go to admin/config/user-interface/click-to-schedule and sign in to an
   existing TimeTrade account or click the Create New Account to create a free
   TimeTrade account.

6. Go to admin/structure/block/add-click-to-schedule-block to create a block
   containing a button that, when clicked, will show the site visitor a calendar
   for scheduling appointments.
