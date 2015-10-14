------------------------
DAILYTWITTER MODULE README
------------------------

This is a module that pulls a list of twitter posts done by a particular user
over the course of a day, puts them in a nice little list, and adds them
to said user's blog.  Sound like LoudTwitter?  That's precisely what it's
goal is, except run within your Drupal site instead of an external site.

This module does not have a home on Drupal's site yet, therefore the following
is just a stub for what I hope will soon be the case:

Send comments via the issues queue on drupal.org:
http://drupal.org/project/issues/dailytwitter

------------------------
REQUIREMENTS
------------------------

This module requires a supported version of Drupal and cron to be 
running.  For this to work reasonably, cron needs to be running at
least once a day.

------------------------
INSTALLATION
------------------------

1. Extract the dailytwitter module directory, including all its subdirectories,
   into your sites/all/modules directory.

2. Enable the dailytwitter module on the:
   Administer >> Site building >> Modules page.
   The database tables will be created automagically for you at this point.

3. Modify permissions on the Administer >> Users >> Permissions page.

4. To enable daily twitter posts, click on the "Daily Tweets" section in
   your "My account" page. Or, similarly go to another user's
   account page at user/<user_id_here> to modify his or her personal settings.

------------------------
AUTHOR / MAINTAINER
------------------------

Daniel Henninger <daniel@vorpalcloud.org> is the author and maintainer.

------------------------
RELATED PROJECTS & ALTERNATIVES
------------------------

http://drupal.org/project/twitter

------------------------
WISHLIST
-----------------------

- Options for the way the summaries look.
