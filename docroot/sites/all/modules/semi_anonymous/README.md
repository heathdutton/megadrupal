Semi Anonymous
==============

**Know more about your anonymous users.**

This module integrates the Groucho anonymous user behavior tracking library with Drupal. With just a few simple configs you can establish great front-end unique-user intellegence on top of heavily cached pages.

### Dependencies
1. [Groucho](https://github.com/tableau-mkt/groucho) - client-side user profiling [7k].
1. [Data Layer](https://drupal.org/project/datalayer) - Google's meta data standard.
 *  [Data Layer Helper](https://github.com/google/data-layer-helper) - access dataLayer properties [2k].
1. [jStorage](http://jstorage.info) - localStorage abstraction library [30k].
 * [JSON2](https://github.com/douglascrockford/JSON-js) - browser compatible JSON methods (if you care) [17k].

## Installation
1. Install this module and the Data Layer module.
1. Add the jStorage, Groucho, data-layer-helper, and (optional) JSON2 libraries.
1. Configure dataLayer output via module admin.
1. Configure Groucho storage options via module admin.
1. Generate favorites and use tracking front-end. Use a browser console to play.

### [Full Groucho docs](https://github.com/tableau-mkt/groucho/blob/master/DOCS.md)

Please post any problems or feature requests to the
[Drupal project issue queue](https://drupal.org/project/issues/semi_anonymous).
