=======================================================
Entity External Rating

Overview:
=======================================================
  This module can be used to implement a rating system based on external
sources. What that means is that the actual rate, vote or whatever action,
happens on an external application (for example on Facebook), and the results
are fetched into your system.

Description:
=======================================================
  The module by itself provides a mechanism to register new external sources,
that are plugins and are managed using ctools, and it also gets the rating
results from the external sources. It has no frontend interface, so other
modules can and should use its API (actually, there are just a few functions at
the moment that can be called to register an entity for rating, to get a total
rating, etc..).

  Any type of entity can be rated. Moreover, there can be multiple ratings per
each entity (for example you can rate the same entity every day, so that you can
have a statistic per day for the ratings).

  As mentioned, the module by itself does not have any frontend interface, it
has only a backend configuration. By default, it implements two external rating
sources: Facebook likes and Twitter tweets. To install the module, just go to
the Modules page and check the checkbox near it. After the module is installed,
a configuration form can be found here: Structure -> Entity External Rating.
From here, you can enable or disable certain plugins, and do some basic
configurations. Each plugin can have its own configuration form (click on the
edit link after each plugin name). The two default plugins have only the
API url that is used to get the ratings.

  There is also an example module that demonstrates how to create a new plugin
in your custom modules.

  The first step for using this module is to add entities into the rating queue.
You can do that using the function: entity_ext_rating_add_to_queue(). By doing
this you basically register an entity so that the module will try to get its
rating periodically at the next cronjobs. A very important parameter is the
$rating_id. This is different than the entity_id because you can have more
ratings for the same entity. Then, the $params contain any parameters that the
external rating system uses for rating. For example, for Facebook and Twitter,
the URL is the parameter. So, a real example of how to call the function: 

entity_ext_rating_add_to_queue('node_100_1', 100, 'node',
  array('url' => 'http://www.example.com/node/100?day=1'), time()+86400);

The last parameter is a timestamp that is used to determine when the entry will
be deleted from the queue, so no more calls will be made to the external rating
sources.

  Then, you can get the total rating for a rating id, using the
entity_ext_rating_get_total_rating() function and you get get the html widgets
to be printed into the page, to allow your users to rate, vote, share, etc..
