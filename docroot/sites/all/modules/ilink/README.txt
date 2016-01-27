Oh hai!

iLink, short for "integrated link", is an abstraction layer
and plugin system to consolidate all the required callbacks
to build links. This is not meant for simple links, but
complicated concepts that require a "trigger" to invoke a
series of operations, and optionally handle a response.
Usually this would require a lot of callbacks and misc.
boiler-plate code (think Drupal's ajax system), but iLink
allows you to delegate the logic and callbacks into
separate, manageable plugins, so it is kept out of the main
module file.
