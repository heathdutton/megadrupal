
INTRODUCTION
------------

This module provides a functionality to clear cache_form records by session id.


INSTALLATION:
-------------
1. Place the entire session_cache_form directory into your
   Drupal sites/all/modules/ directory.
2. Replace cache class for cache_form table on 'SessionCacheForm'	

	$conf['cache_class_cache_form'] = 'SessionCacheForm';

3. Enable the session_cache_form module by navigating to:

     administer > modules
