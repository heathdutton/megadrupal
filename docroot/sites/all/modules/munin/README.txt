Munin Drupal plugin
Author:   McGo (Mirko Haaser)
Website:  http://drupalist.de

Refactored by Alex Savin (http://www.videophpblog.com):
 - Drush integration instead of DB credentials in scripts (enhanced the
modifications done by thebuckst0p (http://newleafdigital.com))
 - Now there is only one script regardless of the number of plugins you create.
Each plugin will have to have a symbolic link to the drupal_ script. This makes
the enabling process much easier.
 - Possibility to read the return of callback and not only sql commands (gives
the possibility  to create more intelligent plugins). Set $plugin['#callback'] (
and alternatively also $plugin['#file'] where the callback resides) that has to
return the value of the plugin.
 - Active plugins saved in settings.

THIS IS NOT A FINAL VERSION!

A lot of additions are planned for this module but this is a good start I think
in reviving it and make it a good tool for webmaster to have a good overview
over their Drupal websites.

Some things that you'll maybe see in future releases of this module:
- CTools integration
- Export/Import plugins
- More intelligent default plugins targeting webmasters
- ... waiting for more ideas (don't be shy on using module issue queue to give
some feedback).

WHAT IS THIS?
This module acts as a plugin generator for munin[1]. It uses a gui that lets
you choose which information will be shown in the statistics. With this
information, the module generates a shell script that you can directly use as a
plugin for munin.

ATTENTION!
The shell script requires Drush (http://drupal.org/project/drush) to work.
Database credentials are NO LONGER STORED in the shell scripts.

INSTALLATION
1. Drop the module in sites/all/modules/contrib/
2. Enable the munin api module to get the basic functionality
3. Go to admin/config/development/munin/script and copy the script contents in
   /usr/sadmin/config/develophare/munin/plugins/drupal_ and make this script
   executable (chmod +x drupal_)
4. If you want to use the defaults, shipped with this module enable the munin
   default module as well.
5. Go to admin/config/development/munin and choose which statistics
   you want to see in your munin setup.
6. Save, than create a symbolic link of drupal_ script in
   /etc/munin/plugins/drupal_name_of_your_plugin.
   For example for users plugin the command would be:
   $ ln -s /usr/sadmin/config/develophare/munin/plugins/drupal_ \
   /etc/munin/plugins/drupal_user
7. Restart the munin-node /etc/init.d/munin-node restart
8. Wait some minutes (10-20) and check your new statistics
   in your munin setup.

For new plugins just repeat from step 6.

YOUR OWN STATISTICS
This module shipps with an API. So you can easily add your own statistics or
alter existing ones. Just create a module and implement hook_muninplugin(). The
API is not yet documented. Just have a look at munin_default.module.

[1] http://munin-monitoring.org/
