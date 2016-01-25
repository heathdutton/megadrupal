Setting Up Your League
----------------------
After you install the module, you will need to enter your MyFantasyLeague
league ID and year number in the configuration page
(/admin/config/system/myfantasyleague). To find your league ID, go to your
league home page and look at the URL:
http://www.myfantasyleague.com/2011/home/xxxxx. The \'xxxxx\' part is your
league ID. Once you save this data, then you can view your MyFantasyLeague
league information on your Drupal site. There are two pages provided:
Standings (/myfantasyleague/standings) and Weekly Results
(/myfantasyleague/weeklyresults). There are also three blocks which you can
enable using the directions below.

Setting Up User Leagues (2.x version only)
------------------------------------------
New in the 2.x branch is the ability for users on the website to configure
their own MyFantasyLeague league. The administrator needs to grant permissions
to roles to let them set up their own league. Once that is done, then the user
can set up their league at user/%user/edit/myfantasyleague. When a user has
their own league set up, the blocks will show their league information,
otherwise it will show the global league.

MyFantasyLeague Menu
--------------------
When installing the module, a MyFantasyLeague menu is created that has links
to the Standings and Weekly Results pages.

Enabling Blocks
---------------
There are three blocks associated with this module: Standings, Weekly Results,
and Live Scoring. To enable one or more of these modules, navigate to the
blocks admin page (/admin/structure/block) and move the blocks that you want
into the region that you want.

About
-----
The MyFantasyLeague module integrates your MyFantasyLeague fantasy football
league into Drupal. It provides pages for you to view league standings and
weekly results. There are also three different blocks that you can enable for
league standings, weekly results, and live scoring.

League Standings
----------------
There is a league standings page and a league standings block. The league
standings page can be customized on the configuration page by changing the
sort order of the columns as well as choosing which columns to show on the
page. The league standings block can be enabled by navigating to the blocks
page and enabling the block in whatever region you want. This only shows the
league standings as they currently are. You can't navigate back to previous
weeks.

Weekly Results
--------------
There is a weekly results page and a weekly results block. The weekly results
block can be enabled by navigating to the blocks page and enabling the block
in whatever region you want. Weekly results only updates daily, so it doesn't
update the scores live as the live scoring block does. This feature is
intended to show previous weekly results.

Live Scoring
------------
There is a live scoring block, which can be enabled by navigating to the
blocks page and enabling the block in whatever region you want. Live scoring
is just meant to track scoring as it happens, so you can't navigate to
previous weeks.
