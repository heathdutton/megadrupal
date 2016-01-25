README.txt
==========

THIS MODULE IS DESIGNED TO IP BLOCK SPIDERS THAT DON'T HONOR ROBOTS.TXT.

This little module brings together functionality that is already available in Drupal 7 when
the Rules module is used. Installation creates a rule that looks at watchdog errors,
and when it sees a "page not found" for a certain bogus URL it blocks the users IP.

The module creates a bogus URL link when the user ID is 0 (anonymous) that no legitimate user would
ever find, and if he did clicking it wouldn't work. This bogus URL is blocked in robots.txt so
good spiders should avoid it.

A user would need to type the bogus URL into their browser to block themselves, and this is a good
way to test that it's working. Be prepared to go into your database and un-block your IP.

Once the module is installed it could be problematic editing the Spider Slap rule in
the Rules UI without editing the module code to allow for that.

The Rule has an email action that fires when an IP block occurs and it's set to the default admin address.
This can be changed in the Rule configuration with no problem. Just delete the email action in the Rule
if you don't want the emails.

The module creates a token used in the email notice to tell you the IP of the offending bot.

Credit for helping create custom token for the Rule: http://www.andypangus.com/drupal-7-making-your-own-tokens


INSTALLATION NOTES
=================

1) The only true dependency is the Rules module (Rules requires Entity tokens module http://drupal.org/project/entity).
The RobotsTxt module (http://drupal.org/project/robotstxt) is a nice addition.

2) Edit your robots.txt either manually or with the RobotsTxt module to exclude your bogus URL by
adding "Disallow: /now/bugoff" to the bottom. Give the good bots like Google enough time to read this
file before activating Spider Slap to avoid blocking them. By examining your server logs you will know how
many days to wait. You can always go into your admin at admin/config/people/ip-blocking and un-block them.

3) Install Spider Slap normally in site/all/modules/spiderslap.

4) Installation will create the rule needed and uninstall will remove it from the database.

5) You should see the added code on your pages with View Source not far below the opening <body> tag.

6) A clear.gif is added for the bogus link and marked display:none.

7) Testing with yourdomain.com/now/bugoff in your browser should get you IP blocked from your site.


UPDATING SPIDER SLAP
===================

If the update has a change to the Rule this module creates you will have to disable and uninstall Spider Slap
to clear the old Rule from the database. Make a note of your custom rule
additions and add them back in after doing the update to Spider Slap. Then install the latest version.

If you already intalled the latest version you can still disable/uninstall/enable to get the latest Rule change.