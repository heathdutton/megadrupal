Profiling
=========

Unintrusive local profiling and timing framework.

This module provides a set of profiling functions to start and stop timers.
Results are being stored into database, only at end of page request, to
ensure database access won't disturb time calculations.

Using these results, the profiling user interface module will provide a set
of computed statistics using large amount of data collected, int order to
display comprehensive over-time profiling charts.

This module is highly developer stuff related, so you might want to start
by reading the DEVELOPERS.txt file.

Installation
------------

Enable this module, it will use the default timers backend that will work
on most environment.

By default, profiling is suspended, to activate it, either enable the UI
module and enable it through the web admin, either put this into your
settings.php file:

  $conf['profiling_enabled'] = 1;

You can enable the query logging by adding this to your settings.php file:

  $conf['dev_query'] = 1;

If you do this, you need to check out this bug: http://drupal.org/node/351844
else you will experience WSOD because or early queries being done.

This is important doing it through the settings.php file and not using the
UI because if you remove the module, you could leave this setting turned on
inadvertently without having the possibility to set it off from the UI again.

You can change the backend using this syntax:

  $conf['profiling_backend'] = "BackendClassName";

Default for this options is "Profiling_Backend_Default". More may come in
the feature and third party integration modules can expose theirs.

Upgrade from 6.x
----------------

Update to the 6.x-1.0-beta3 before upgrading core. Do not use a latest release
this is important this is where the 7.x branch was created, therefore upgrade
will work from there.

Notes
-----

For any feature request, missing statistics, missing comprehensive screen,
missing documentation, bugfix, wanted behavior, charts support, etc, please
contact me on the issue queue.

This is still experimental work, this does not means it isn't stable, it just
means that the UI needs work. and I need statistics guru's help in order to
provide a full set of comprehensive and usefull data.

The core profiling timers information mining is fully functional, performant,
and stable, all we need is good statistics calculations to improve final user
experience.

Any patches are welcome. Statistics gurus are welcome too. Optimizations are
welcome. And most of all, features are welcome!
