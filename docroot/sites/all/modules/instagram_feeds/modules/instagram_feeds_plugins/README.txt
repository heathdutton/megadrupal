INSTRAGRAM FEEDS PLUGINS
------------------------

This module provides a Feeds Fetcher plugin meant for Instagram API
feeds. The plugin is able to use the data returned by the API to continue
to page the results for as long as needed. The API will only return 20
items per-request, so paging is required to return more than that.

Also this module provides a Feeds Processor plugin that allows to check
the uniqueness of imported nodes independently of Feeds URLs.

REQUIREMENTS
------------
    * Feeds 7.x-2.0-alpha5 or higher.

INSTALLATION
------------

1. Install Feeds and this module as you would any other.
2. When creating/editing a Feed, choose "Instagram Pager" fetcher
 and "Instagram Feeds Node processor" processor.
3. Choose the maximum amount of pages to fetch when configuring the
 fetcher. If set to zero, there will be no maximum and the module will
 only stop when there are either no pages available, or if their are no
 new items left to fetch.
4. When settings up the mapping for the import, you *must* select the
 Instagram item ID as the feed item GUID.
