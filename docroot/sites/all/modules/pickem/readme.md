# Sports Pickem Module

Run a sports pickem league using Drupal. It can be used for the NFL, NHL, College Bowl season, March Madness, MLS, etc...
Includes pick the winner style along with point spread support.

## Features

* Import-export of game sets feature so you can share team lists and schedules with other users of this module.
* Multi-league: more than one league can use the same gameset so the commish need only enter the scores once!
* Multi-gameset: you can run more than one sport (or division) at one time.
* Email reminders: let people know when the pick lock-up time will come.
* Charts(*): gotta have pretty charts:) <a href="http://drupal.org/project/open_flash_chart_api">Open_Flash_Chart_API</a>.
* Dynamic timer(*): let people know when the picks are due by <a href="http://drupal.org/project/jstimer">Javascript Timer</a>.
* Automatic score updates(*): Not having to type in those pesky sports scores...priceless. Service provided by <a href="http://drupal.org/project/sports_scores">Sports Scores</a>.

(*) Optional integration will other Drupal modules.

## Installation and Setup

### Install the sports pickem module

1. Download and install the <a href="http://drupal.org/project/pickem">Sports Pickem Module</a>
2. Enable the Sports Pickem Module on the admin/modules page.

### If you want to use the forum module for chat and banter:

1. Enable the Forum on the admin/modules page
2. Add a Forum for chat and league banter.

### If you want to have a rules and regulations page:

1. Add a Standard Page/Story/Node or a Forum Post for an explanation of the rules of the league, and remember the node id of the post.

### Create your first league:

1. Import a gameset or type one in. You'll need teams, weeks, and games.
2. Add a League (/admin/content/pickem/leagues) and choose the linked forum* and the Rules node id* from above.
3. Add users to the league (admin/content/pickem/league_users)
4. Enable and show the Pickem Leagues block (admin/build/block)

This user will then see the league menus under "My leagues".

### If you want a dynamic countdown timer which will countdown to the next week pick lockup time:

1. Download and install Javascript Countdown Timer module. (http://drupal.org/project/countdowntimer)
2. Make sure to configure Javascript Countdown Timer to show on 'Every Page' in its admin screen. (admin/settings/countdowntimer)

### If you want pretty flash charts:

1. Download and install Open Flash Chart API. (http://drupal.org/project/open_flash_chart_api) requires additional steps listed in the modules documentation.

### Remember

* Set the "access pickem" and "play pickem" permissions for your users.
* If you are using the javascript countdown timer, go to its admin page and select "Every Page".
* For a pickem-only site, set the "access denied" page to be some kind of info page explaining that you must login
* For a pickem-only site, set the default front page to "pickem"
* For an nfl-pickem site, set the path to custom logo to "pickem/logos.gif" (deprecated) "pickem/random_logo.png" in the 6.x-1.7+
* For an nfl-pickem site, set the path to customer icon to "sites/all/modules/pickem/favicon.ico"

## Sports Scores Integration

If you want the Sports Scores Module to update the game scores for you, there are a couple steps to perform.

1. pickem gameset sport = sport scores sport.
2. sports scores team names = pickem gameset teams abbrev (OR name).
3. pickem game date = sports scores game date.
4. sports scores game data looks right.  Check out the database and see what is being saved in the sports_scores table.
