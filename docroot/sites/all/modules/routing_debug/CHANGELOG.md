[Routing Debug - v1.0.0 (2015-06-09)](https://github.com/davidlukac/routing_debug/releases/tag/v1.0.0)
--------------------------------------------------------------------------------
- First community approved public release of the module (7.x-1.0).

[Routing Debug - v0.2.2 (2015-05-19)](https://github.com/davidlukac/routing_debug/releases/tag/v0.2.2)
--------------------------------------------------------------------------------
- Changed module links to `devel/routing_debug` to be present among other
  Devel tools.
- Main module page is now also present under _Reports -> Routing Debug_.
- Several minor formatting, commenting and documentation changes - see
  (module application)[https://www.drupal.org/node/2484543#comment-9937723] for details.
- Fixed module permissions implementation - module defines one permission:
  _`Access routing information`_ which is checked when visiting module pages
  or access the information in block provided by the module.

[Routing Debug - v0.2.1 (2015-05-16)](https://github.com/davidlukac/routing_debug/releases/tag/v0.2.1)
--------------------------------------------------------------------------------
- Implemented debug block that shows relevant information for current path.

[Routing Debug - v0.1.0 (2015-05-10)](https://github.com/davidlukac/routing_debug/releases/tag/v0.1.0)
--------------------------------------------------------------------------------
- Added link to 'devel/menu/item' and actual menu route (if valid) from Routing
  Debug page.

[Routing Debug - v0.0.2 (2015-05-07)](https://github.com/davidlukac/routing_debug/releases/tag/v0.0.2)
--------------------------------------------------------------------------------
- Fixed _'devel'_ dependency.
- Added _'debug log'_ toggle variable.
- Moved menus to tabs for better navigation.
- Preparations for Drupal.org module review:
  - removed license file,
  - added readme file,
- Added uninstall hook to clean up module's variables.
- Added install hook to display link to module pages after installation.

[Routing Debug - v0.0.1 (2015-04-28)](https://github.com/davidlukac/routing_debug/releases/tag/v0.0.1)
--------------------------------------------------------------------------------
- Initial and very first release of the module.
