Starting from 29.11.2010, HEAD works with D7 core, D6 version should be taken from Drupal-6--1 branch.

D7 version  makes use of DBTNG database abstraction  layer (@see  http://drupal.org/node/310075 for a reference)

This module is modelled after the WordPress plugin of the same name; I'd call
it a port, but there are only a few sections of code that actually
remain the same. So while it's not actually technically original, it's
about 80% different. That said, I must give credit to the original author
who did a lot of work in getting the WordPress plugin working:

http://ebroder.net/livejournal-crossposter/

To install:

1) drop the entire directory into your modules or sites/*/modules directory
2) go to administer >> modules and enable the module
3) go to administer >> settings >> ljxp and enable the content types you want
   to use
4) If you want more than just UID #1 to use this functionality, Go to 
   administer >> access control and set the 'can crosspost to livejournal'
   permission for a role.
5) go to 'my account' and choose edit; enter your LiveJournal info there.
   Each user must have his or her own crossposting information.

TODO in D7 version:
  * select community per post, not as global settings.
  * correct work of tags.