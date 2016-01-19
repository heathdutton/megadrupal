Selective Tweets
================
Selective Tweets makes it possible to add Twitter timelines and Twitter tweets
to a Drupal website. When enabled, a new block type is added which displays
Twitter feeds with a certain filter applied to it. The tweets are either
rendered as embedded tweets or as raw HTML, which lets you style them as wished.

Features
================
* It's no embedded timeline widget, but instead individual Tweets representing
  a timeline or a filtered selection of Tweets.
* Render mode 'embedded': Individual embedded tweets are displayed
  (one or more), which gives more flexibility than an embedded timeline widget.
* Render mode 'raw': Individual Tweets are rendered in raw html and can be
  overriden via a template file, which makes them fully themable.
* An optional 'load more' link can be added to ajax load the next Tweets.
* Tweets are not saved to the database, except for cached data to make the
  loading a lot faster
* Due to caching, a delay of minimum 1 minute relative to the Twitter timeline
  has to be taken into account while displaying Tweets.
* Up to 200 tweets can be displayed, paged or all together.

Filters
================
* Most recent Tweets of specific timeline
* All favorited Tweets by specific user
* Tweets addressed to specific user
* Tweets filtered by hashtags (AND/OR relation)
* Tweets filtered by user mentions (AND/OR relation)
* Optionally include retweets

Dependencies
================
* block
* oauth_common (Because the url is not the same as the project name, drush
  can't install this one. Do it manually before enabling Selective Tweets.)

* Twitter developer account

Installation
================
1. Download and install OAuth yourself, as drush can't do it because the project
   url (oauth) differs from the project name (oauth_common).
2. Install Selective Tweets via the standard Drupal installation process.
3. Configure the module at admin/config/services/selective-tweets
4. On the Block administration page, add a new Selective Tweets block and
   configure it. Place the block in the right region.

Note that the first time the Twitter block will be loaded, it will take a
little longer because a stack of tweets is being gathered and cached from the
Twitter API.

Custom theming
================
To theme the Tweets, you select the render mode 'Raw HTML' in the block
settings. Now you can copy the selective-tweet-raw.tpl.php from the templates
directory in the Selective Tweets module to your own theme directory (or
subdirectory). Make the changes in the tpl file, clear the Drupal cache and the
custom template should be applied. Write your own CSS to beautify the result.

FAQ
================
* My block shows no results, what happened?
  In case your Selective Tweets block shows no results, and you do see the block
  title and no further error messages, you've probably chosen the selection mode
  'filter by' in the block configuration. The Twitter API tells us that it's not
  indexing ALL tweets out there in their search index, so there is a chance
  you're not getting any tweets for a specific user. In case you just want to
  show a timeline of a user, you can opt for the first selection mode 'most
  recent tweets'. Unfortunately this is a limitation of the current Twitter API.

* How can I clear the cache of just one Selective Tweets block?
  By saving a Selective Tweets block, the cache is cleared for that specific
  block.

* The load more link is not working?
  You've probably chosen the render mode 'raw HTML Tweets'. Make sure you don't
  remove the 'selective-tweet' class from the outer div in the template file.
  The load more functionality is counting on this class to be there.
  
* None of the above?
  If you don't get any results an no clear error message, you can check your
  site's recent log messages. Often the cause is found there.


Current maintainers:
  Lennart Van Vaerenbergh - http://www.vaerenbergh.com
