------------------------------------------------------------------------------
  external_redirect module Readme
  http://drupal.org/project/external_redirect
  by David Herminghaus (doitDave) www.david-herminghaus.de
------------------------------------------------------------------------------

Contents:
=========
1. ABOUT/WHAT
2. TECHNICAL/HOW
3. REQUIREMENTS/RECOMMENDATIONS
4. QUICK REFERENCE
5. IMPORTANT

1. ABOUT/WHAT
=============

This module helps you organizing (and obfuscating) outgoing links
from your site. There are different reasons why one would want
to do so, e.g.:

* You do not want to know the target sites' owners where exactly from
  your site their visitors come.
* You want to add an explicit disclaimer to all links (a function very
  appreciated by site owners in internet and link phobic countries
  or such with too many "internet lawyers", such as Germany).
* You are polite enough to wave good bye each single user.

2. TECHNICAL/HOW
================

This module consists of two components:

1) An input text format filter for standard text fields.
2) Additional display formats for link fields.

Both check for links to be considered external (you may configure
the criteria globally) and, if they find some, these will be transformed
to no longer link directly to the external URL but to a gateway page the
text of which can be set up individually.

To avoid abuse, a referer Check is also done so that if someone links to
your gateway page even with the appropriate parameters, there will not be
displayed a clickable link (but only the external URL in case the client's
browser blocks HTTP_REFERER headers).

3. REQUIREMENTS/RECOMMENDATIONS
===============================
Link detection will only work with links which are really links. That means
that only structures like 

<a href="protocol://domain.tld/..."> 

will be modified. Thus, if you want everything that looks like a link to be
treated just as one, the URL filter is recommended for your text formats
and it should be placed before the external link gateway filter.

4. QUICK REFERENCE
==================
1) INPUT FORMAT FILTER
* Set up the global options at admin/config/content/external_redirect and
  note especially the "host list" part. Here you may enter all
  domains not to use the gateway page for.
* Update your text formats by activating the "External link gateway" filter.
  Make sure to also get to the "rearrange" tab and place this filter near the
  end of your filter chain (if possible) but at least behind the URL filter.
  Ideally it would take place right before the HTML correction filter.

2) FIELD DISPLAY FORMAT
After installing the module, you will find some additional display options
for every link field you have set up (assuming you have also installed the
link field module, of course). Please check the
admin/structure/types/manage/[node-type]/display page to alter these.
The additional types are based on the link field standard format types but
extended with a "gateway" suffix. They behave almost the same way their role
models do but first redirect the user to the gateway page.

3) USER PROFILES
Since Drupal 7, user profiles are made with the field API - so see 2).

5. IMPORTANT
============
A. Views integration
The bad news: If you have set up views to use link fields, you will also need to
alter their display format in every view.
The good news: After that, views will do fine as well!

B. Overall filtering
This option is only available in the 6.x-1.x branch. D7 works far more structured
and leaves far less space for "hacks" like these. Which is, IMHO, the right way.
So, if you do not want to bother reconfiguring every existing field and text
format, either create yourself a quick and dirty Apache output chain filter or
pray for the day that I do release the one I use for some of my servers. 
Oh, needless to mention: An adequate monetary compensation would not only
speed up that process but also leave such a global solution more or less
exclusively to you. In other words: If you want such special things,
just ask for an offer ;)
