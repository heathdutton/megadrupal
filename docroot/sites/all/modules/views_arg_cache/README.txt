VIEWS ARGUMENT CACHE
=============================

Background
---------------------
This module provides an "Argument-based" cache plugin for Views. It should
only be used with Views that take arguments.

Traditional cache plugins generate cache keys based on everything available
during the Views build process. This makes it impossible to flush a cache
for a View only for specific arguments.

Take this example..

We have two node types: School and Class. A class node has a node-reference
field back to a school. When viewing a school node, we use a view to show all
classes that are linked to that school. Before this module, the best cache
plugin would be Views content cache, which allows us to clear the cache only
when a certain node-type is added/updated. We would set the cache to flush
whenever a class node was created. But, why empty every single school cache
just becuase one class was created?

With this module, you can target the Views cache for a given view, display,
and provided arguments. The plugin has options to specify how many of the
arguments you want to use to generate the cache key.


How to use it?
---------------------
Be aware, that these caches can *only* be flushed by custom code. Otherwise,
they will never expire. See example below:

function hook_node_presave($node) {
  views_arg_cache_flush('school_classes_view', 'block', $node->field_school_ref['und'][0]['target_it']);
}

views_arg_cache_flush() takes three arguments. 
1) View id
2) Display id
3) Array of arguments or single argument


How does it work?
---------------------
This plugin is basically a clone of the standard views_cache_plugin but
it generates keys in a different manner.

Format: [view_id]:[display_id]:[md5-of-arguments]:[cache-type]:[md5-of-builddata]

This allows us to flush the cache using the first 3 pieces as a wildcard.


Sponsored by
---------------------
http://workoutspots.com
