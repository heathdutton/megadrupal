Ctools Plugin Stub is a Drush plugin that can help you generate code stub files
for ctools plugins. I wrote this plugin after having consistently messed up
names of hook implementations in plugins over a long period. Plugin Stub does
not write actual code that *does* stuff for you, but it sets you up so that all
you have to do to get started is to fill in the capitalized parts of the
generated file and start writing code that *does* stuff.

The plugins generated work for both D6 and D7.

So far it supports these kinds of ctools plugins:
* content_types for Panels
Contributions are very welcome!

To use it, change dirs to the module you would like to be responsible for the
plugin. Then issue the command 'drush plugin-stub content_type somename'.

You can supply the option -w to have Ctools Plugin Stub write an implementation
of the hooks needed for ctools to find the plugin.