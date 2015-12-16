This is a tiny module that basically extends Drupal's comment statistics module
by adding comment counts for day, week, month and year. It also integrates with
Views and you can use it to select, filter, order the content as well.

--------------------------------------------------------------------------------
Integration
--------------------------------------------------------------------------------

This module plays well and has been tested with Views. You can use these fields
in Content group
- Comments today
- Comments this week
- Comments this month
- Comments this year

You can also use those fields to order and filter content.

--------------------------------------------------------------------------------
Dependencies
--------------------------------------------------------------------------------

- Comment (Drupal Core)
- Views 3.x (optional)

--------------------------------------------------------------------------------
Installation
--------------------------------------------------------------------------------

Download the module and simply copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).

--------------------------------------------------------------------------------
Configuration
--------------------------------------------------------------------------------

After successful installation, you need to activate the counter by going to
settings page admin/config/system/comment and check on "Count comments".

You also need to set up cron in order to reset the counters.
