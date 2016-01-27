Introduction
------------
The core poll module's behaviour can be troublesome in combination with a 
frontend cache (reverse proxy) like Varnish when you want to allow anonymous
users to vote on the poll. Poll results will be shown to users who have voted,
but depending on which user triggers a cache refresh, this may mean cache 
results end up in cache and other users, who have not voted, see the results 
too and are not able to vote. This may get confusing.

This module sets a cookie when a user votes on a poll. This serves two purposes;
the user may get served pages containing a poll block showing results, and it
will prevent the external cache from caching pages containing poll results.

To submit bug reports and feature suggestions, or to track changes:
https://drupal.org/project/issues/cached_poll

Do note that this functionality means that users who have voted will not get 
pages served from your frontend cache (they will essentially get the same 
treatment as logged in users). If you have many people voting on polls, this 
will make your frontend cache much less effective. If this is a problem for, you 
this is not the module you are looking for.

Requirements
------------
This module has no particular requirements, although it is fairly useless 
without the core Poll module enabled.

Installation
------------
 
*   Install as you would normally install a contributed Drupal module. See
    [Installing modules (Drupal 
    7)](https://drupal.org/documentation/install/modules-themes/modules-7)
    for further information.
*   Make sure your frontend cache is configured to not cache pages when cookies 
    starting with "NO_CACHE". For example, many Varnish configurations contain 
    the following line:
   
        set req.http.Cookie = regsuball(req.http.Cookie, ";(NO_CACHE|SESS[^=]+)=", "; \1=");
       
    You would need to change that to:
       
        set req.http.Cookie = regsuball(req.http.Cookie, ";(NO_CACHE[^=]*|SESS[^=]+)=", "; \1=");

Please do not ask for support with your particular setup; the basic premise is 
pretty simple, you need to configure things in such a way that this cookie 
punches a hole in your frontend cache.
   
Configuration
-------------
The module has no menu or modifiable settings. There is no configuration. When
enabled, the module will add a cookie to requests for users that have voted (or 
at least, are considered to have voted by the core POll module) that may be used
to prevent pages containing poll results from being cached. 

Maintainers
-----------
Current maintainer:

 * Eelke Blok (eelkeblok) - https://www.drupal.org/u/eelkeblok

This project has been sponsored by:

 * Dutch Open Projects  
   DOP is a leading Drupal implementer in the Netherlands and provides complete 
   solutions for their customers, from functional design including wireframing 
   to a working Drupal site including maintenance, training and hosting. 
   Visit http://dop.nu (mostly Dutch) for more information.
