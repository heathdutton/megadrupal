
Module: Tether Stats
Author: Rustin Zantolas <http://drupal.org/user/2745073>


================================================================================
Introduction
================================================================================

Collects site statistics from JS enabled users by using a JSON callback to
track events. Essentially, an AJAX style call from the user is used to
record activity, thus preventing most bots from polluting the data.

Tether Stats is intended for intermediate to advanced Drupal developers. It
behaves like a framework allowing you to relate site activity to your
content structure using Drupal's entity concept.

If you need to collect more data than what Google Analytics or Drupal stats
can provide, this module may help fill in those gaps. The stats tables are
relatively easy to mine and may be generated on a database separate from
your Drupal core database.

Here are the basic type of stats that Tether Stats collects:

* Page Hits
* Link Clicks
* Impressions
(An impression occurs when an item, such as a link, name or image, appears
on a page and is made visible to the user.)


It can be challenging to customize Google Analytics for Link Clicks or
Impressions when Google knows nothing about your internal structure. Tether
Stats can make collecting this type of data as simple as adding classes to
your HMTL structure.


================================================================================
Recommended Modules
================================================================================

Tether Stats also provides a chart module which has classes for the
generation of iterable charts based on collected data. Please refer to the
README.txt file within the modules/tether_stats_charts directory.


================================================================================
Configuration
================================================================================

 * Configure user permissions in Administration » People » Permissions:
   - The 'Administer Tether Stats' is required to update Tether Stats settings
     or use any of the administrative tools.

 * Configure settings for the Tether Stats module at
   admin/config/system/tether_stats


================================================================================
Concepts
================================================================================

Elements
========

Tether Stats simplifies its tables by converting every item for which
activity is recorded to a stats "element." Elements may include module
generated pages, any type of entity within Drupal including any unique
custom entities you may have created, or an element of your own making with a
unique name. You may also create elements derived from other elements for
more flexibility.

Basically, everything that stats are collected for will have an entry in the
tether_stats_element tables, each having an elid or element id. This table
is populated automatically as needed whenever a new event is requested to be
tracked.

Activity
========

When an event is tracked, an entry is added to the tether_stats_activity_log
table. This entry will contain the elid of the element on which the event
occurred as well as the standard information regarding the user that triggered
the event.

Impressions
===========

When a user loads a tracking enabled page, a "hit" entry is added to the
activity log to record that event. By adding classes to the HTML markup, you
may request that other elements be impressed on that page. In this case, an
entry is added to the tether_stats_impression_log for every element to be
impressed.

The impression log table is very simple as it only needs to relate the
element to be impressed to the activity_log entry where the impression was
made.


================================================================================
Identify Sets
================================================================================

Basic Sets
==========

Other than the elid, elements are identified by so called "identify sets."
This is a set of identifiers which Tether Stats can map uniquely to a
specific element.

The tether_stats_get_element function takes this set of identifiers and
returns the matching element. If no entry for that element currently exists,
one can be created.

Excluding derivatives, an element may be identified by three possible
combinations, a unique string name just for that element, an entity_id, or
just by a url.

Examples:

By name,

array(
  'name' => 'my_unique_element_id',
)

By entity,

array(
  'entity_type' => 'node',
  'entity_id' => 1234,
)

By url,

array(
  'url' => 'my_page/url',
)

In the case of a module generated page which can only be identified by the
url, the entry in the element table will populate the 'entity_type' field with
the special 'tether_page' value. However, if the url only identify set was
given for a node based page, the tether_stats_get_element function
automically transform the identify set include the 'entity_type' and
'entity_id'. In this way, page elements can easily be looked up by their
respective entities.

Derivatives
===========

There may be cases where data needs to be collected for things which are not
entities or pages but are related.

For example, suppose you have a link called "promotion" that appears on all
your pages for a node type called "blog". You could create a uniquely named
element to represent that link and then track it how many times it was clicked.
This would group all the link clicks on all blog pages into one stat counter.
This would only tell us how well the link is performing overall, but it would
not indicate how the blogs are fairing individually.

In this case, you can tell Tether Stats to create a different element for the
promotion link on each of the blog pages. This is done by creating a
"derivative."

Each derivative has a unique string identifier and can be created for any type
of stats element. For the above
example, you could create the following derivative:

array(
  'entity_type' => 'node',
  'entity_bundle' => 'blog',
  'name' => 'link-promotion',
  'description' => 'The promotion link on a blog page',
)

Once this derivative is created, you can use the following identify set to refer
to the link element on a blog page:

array(
  'entity_type' => 'node',
  'entity_id' => 1234,
  'derivative' => 'link-promotion',
)

By adding this identify set to your link on each blog page, Tether Stats
will create a new promotion link element for each blog page. Adding the
'derivative' field to the set now refers to an entirely new element with its
own activity.

The 'entity_type' and 'entity_bundle' properties of the derivative are optional
and constrain this derivative so that it can only be applied to entity boung
elements for node pages of type blog.

Derivatives may be created and managed from
admin/config/system/tether_stats/derivatives


================================================================================
How To Track Link Clicks
================================================================================

To track a link click, first add the class "tether_stats-track-link" to the
anchor tag. This indicates that Tether Stats should track when this link is
clicked.

Secondly, add your identify set to the same HTML tag in the form of
data-{$field}="{$value}".

From the example borrowed from the "Derivatives" section above:

<a href="http://www.drupal.org/" class="tether_stats-track-link"
data-entity_type="node" data-entity_id="1234" data-derivative="link-promotion">
My Promotion Link</a>

In this example, you would need to create the "link-promotion" derivative
before any link clicks could be tracked.

Note, Tether Stats will not record the link click if the page is excluded from
tracking.


================================================================================
How To Track Impressions
================================================================================

To track an impression, first add the class "tether_stats-track-impress" to an
HTML tag, most likely the tag that contains the things you want to impress.

Secondly, add your identify set to the same HTML tag in the form of
data-{$field}="{$value}" as you would for Link Clicks.

For example, suppose you want to impress how many times a user has appeared on
other pages. If you have a teaser that you use for your users, then you could
add the class and attributes to that teaser.

<div class="tether_stats-track-impress" data-entity_type="user"
data-entity_id="123">
<img src="{$user_picture_url} /> {$user->name}
</div>

Note, Tether Stats will not record the impression if the page is excluded from
tracking.


================================================================================
Mapping Pages To Stat Elements
================================================================================

When a page request gets processed by Drupal, a corresponding stats element is
created for the page unless the request is filtered by the URL filter rules.
The default behaviour is to map the page to an element with a URL based
identity set which is simply the following:

- url: {drupal system URL}

Node pages, however, have this behaviour overridden by the following entity
bound identity set:

- entity_type: "node"
- entity_id: {nid}

The url is still included, but only the entity_type and entity_id parameters
uniquely define an element. In this way, our tether_stats_element table has
richer data as we can now search for stats by node nid.

This node mapping occurs because the tether_stats_tether_stats_url() method
implements the hook_tether_stats_url hook and substitutes its own identiy set
for node pages. See hook_tether_stats_url() for more information.


================================================================================
Tables
================================================================================

There are five tables generated for the Tether Stats module.

tether_stats_element
====================

Each entry in this table represents an element or an item for which activity is
tracked.

elid
- The primary key of the table or the element id.
entity_id
- The same as Drupal's entity_id, must be combined with the entity_type field.
entity_type
- The type of entity referred to by entity_id, or 'tether_page' if this element
- refers to a non-entity page.
name
- A unique identifier which can be used to identify custom one-off elements.
url
- The url if this element can refer to a page.
query
- The query string of the url. Only used if query strings are allowed to spawn
- new page elements.
derivative_id
- If this element is a derivative, then this id maps to the corresponding
- derivative in the tether_stats_derivative table.
count
- The total number of times a hit or click event has occurred for this element.
created
- The time the element was first created.
changed
- The last time the element was updated. Does not include count updates.
last_activity
- The last time the activity count increased on the element.

tether_stats_derivative
=======================

This table stores the derivatives which are used to generate more uniquely
identified elements in the elements table.

id
- The primary key of the table
entity_type
- The type of entity to which this derivative refers.
entity_bundle
- The bundle of the entity to which the derivative refers. May be NULL to
- refer to any bundle for the entity_type.
name
- The unique string identifier for this derivative.
description
- A human readable description of this derivative.
disabled
- A boolean to indicate whether this derivative may be used to generate new
- elements.
weight
- The ordering of this derivative for listing purposes.

tether_stats_activity_log
=========================

This table is where the relevant site statistics are stored. Every time an
event occurs, an entry is added to this table. As a result, expect this table
to grow rapidly.

alid
- The primary key of the table or the activity log id.
elid
- The element on which this event has occurred.
type
- The type of event which has occurred such as a 'hit' or a 'click'.
uid
- The user which triggered the event if the user was not anonymous.
referrer
- The url of the page which referred this event. Useful for page hits to
- determine where a user originated from. This value comes from the javascript
- document.referrer property and may not always be available.
ip_address
- The ip address of the user that triggered the event.
sid
- The session_id of the user that triggered the event.
browser
- The browser string of the user that triggered the event.
data
- Optional custom data to include with the event. Not used currently.
hour
- The timestamp for the start of the hour (local time) on which the event
- occurred. Used to group activity over hourly periods.
day
- The timestamp for the start of the day (local time) on which the event
- occurred. Used to group activity over daily periods.
month
- The timestamp for the start of the month (local time) on which the event
- occurred. Used to group activity over monthly periods.
year
- The timestamp for the start of the year (local time) on which the event
- occurred. Used to group activity over yearly periods.
created
- The time when the event occurred.

tether_stats_impression_log
===========================

This table records when an element was impressed. For example, when an element
appeared on a page.

ilid
- The primary key of the table or the impression log id.
alid
- The activity on which the impression occurred.
elid
- The element that was impressed.

tether_stats_hour_count
=======================

This is a non-essential table that keeps a aggregated event count for elements
on an hourly bases. This data can be purely extracted from the activity log,
but performing such queries can be costly. This table can be used instead for
a more efficient way of compiling data to generate charts

hcid
- The primary key of the table or the hour count id.
elid
- The element for which the count applies to.
type
- The type of activity for which the count applies such as a 'hit' or 'click'.
count
- The total number of times an event has occurred for an element during a
- specific hour.
hour
- The timestamp for the start of the hour (local time) on which the activity
- occurred.
day
- The timestamp for the start of the day (local time) on which the activity
- occurred. Used to group activity over daily periods.
month
- The timestamp for the start of the month (local time) on which the activity
- occurred. Used to group activity over monthly periods.
year
- The timestamp for the start of the year (local time) on which the activity
- occurred. Used to group activity over yearly periods.
timestamp
- The time of the last activity.


================================================================================
Maintainers
================================================================================

Current maintainers:
 * Rustin Zantolas (rzan) <http://drupal.org/user/2745073>

This project has been sponsored by:

* The American Society For Aesthetic Plastic Surgery

  This module is based on work done for the society's public website
  www.smartbeautyguide.com.

  They have benefited a lot from the Drupal Community and were pleased they
  could help give something back.
