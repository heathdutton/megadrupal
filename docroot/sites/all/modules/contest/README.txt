Module: Contest
Author: bkelly bkelly@265918.no-reply.drupal.org

CONTENTS OF THIS FILE
====================
 * Description
 * Requirements
 * Installation
 
 
Description
===========
Allows your site to host a sweepstakes, (contestants win prizes via a random 
drawing).

 * For a full description of the module, visit the project page:
   https://drupal.org/project/contest
	
 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/contest

   
Requirements
============
This module requires the following modules:

 * Block (https://drupal.org/project/block)
 * Date (https://drupal.org/project/date)
 * List (https://drupal.org/project/list)
 * Text (https://drupal.org/project/text)

 
Installation
============
		1. Copy the 'contest' directory in to your Drupal modules directory.
		
		2. Go to /admin/config/regional/settings and configure the country and time
		   zone.
		
		3. Go to /admin/config/media/file-system and configure the private file
		   system path.
		
		4. Enable the module.
		
		5. Flush the cache.
		
		3. Go to /admin/settings/contest and configure the contest settings.
		
		6. Complete a profile for the contest host, (defaults to user 1). This 
		   information is used in the contest view, (particularly the rules fieldset).
		
		7. Complete a profile for the contest sponsor, (if host and sponsor are the 
		   same you can skip this). Again, this information is used in the contest
		   view, (particularly the rules fieldset).


MAINTAINERS
===========
Current maintainers:
 * Bill Kelly (bkelly) - https://drupal.org/user/265918

Much of the 7.x-1.x Contest module development was sponsored by:
 * WEYMOUTH DESIGN
   Friendly Drupal experts providing professional development, media and branding
   services. Visit https://www.weymouthdesign.com for more information.