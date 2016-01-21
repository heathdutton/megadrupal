The Shared Blocks Module is awesome!

If you have a main site with blocks that you want to syndicate across several
other websites, you need this module!

Features:
* Allows publishing and subscribing of Drupal blocks
* Changes to published blocks are updated on sites with subscription blocks
* Published blocks provides a JSON page at blocks/{module}/{delta}
* Block subscriptions are updated on cron with an adjustable refresh interval


JSON format:
  success: boolean
  title: string
  content: string
  last update: timestamp ?
  site root: string (could be gleaned from subscription URL)

Notes:
* Published blocks always represent the view of an anonymous user. Please be sure to check permissions.
* Relative and root-relative URLs (which start with "/") will need to be rewritten.
* It would be good to provide a JSONP alternative - perhaps by adding extra args to the URL blocks/{module}/{delta}/jsonp/{func}.
* Likewise, we could also return blocks/{module}/{delta}/html with a full rendered version of the block.

Hooks:
- hook_menu()
  - callback for configuration page
  - publish tab, subscribe tab
  - callback for individual published blocks (blocks/{module}/{delta})
- hook_cron()
  - subscribe sites ping publish site on regular basis checking for updates (recent comments)
- hook_block()
  - Create subscription blocks
  
Future enhancements:
- Don't show blocks available to publish that aren't viewable by anonymous. :)

