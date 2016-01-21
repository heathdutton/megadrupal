ABOUT THE MODULE
----------------
This module acts as a query handler for Views 3, allowing you to work with remote data from the last.fm API as if it were local content. The last.fm API is organized around the entities you are querying - for instance there is

user.getEvents
user.getTopAlbums
user.getTopTracks

artist.getEvents
artist.getTopAlbums
artist.getTopTracks

etc

This is a little different from views, which is organized around the entities you are displaying, or in terms of the last.fm API, the entities being returned. Therefore the module is organized somewhat differently than the API. As an initial stab at implementation, I have added event support, which encompasses the following functions

user.getEvents
user.getPastEvents
venue.getEvents
artist.getEvents
artist.getPastEvents
geo.getEvents

You can control which of these is queried through the use of the view arguments or filters. Which argument or filters you choose controls which method gets called. For instance, adding the 'artist' argument will make this view call artist.getEvents, and the argument has a checkbox option to specify whether or not to retrieve past events. Some combinations of arguments will not be valid (like you can't combine 'artist' and 'user') and these will throw errors. Filters work the same way. Combining arguments and filters will yield unpredictable results. Arguments will always take precedence over filters if both are present.

While I realize this seems a little strict, I can only work within the confines of the API and what it provides me. Perhaps later when the module is more mature, I can start to work some of these issues out.


Last.fm user submodule
-----------------------------

This module allows users on a Drupal site to connect thier account to an
account on Last.fm. The module adds a tab under "My Account" where a user
can authorize the Drupal site to retrieve information them from the Last.fm.

The module doesn't actually retrieve any information, but provides an
interface for managing the authorization of accounts, and an implementation of
the authorization process.

## Setup

Install this module in the usual manner. Get API keys from Last.fm and enter
them in the module's configuration page; `admin/settings/lastfm`.
