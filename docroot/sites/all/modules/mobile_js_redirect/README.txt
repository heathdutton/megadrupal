This is a simple module that adds a JS in the HTML head to perform a redirect
from a desktop site URL to a mobile one or the other way around. This is
useful when Varnish or any other reverse proxy cache is being used to cache
your site and keep a mobile vs desktop redirect working.

Install this module like any other module and set the "Desktop site URL" and
"Mobile site URL" for this to work.

To be able to access the administrative area or avoid redirection at any point, use the query string below.
?stopredirect=true

URL settings - /admin/config/system/mobile_js_redirect
