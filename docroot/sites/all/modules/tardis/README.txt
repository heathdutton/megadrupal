TARDIS
======

The TARDIS module allows you to organize your recent nodes chronologically 
using blocks that display links in the format "YYYY/MM" and pages that show
those nodes filtered by node type.

There's several options available, so install as usual(*) and visit 
example.com/structure/tardis to set up TARDIS blocks and pages. 

____________________________________

(*) Download the module from http://drupal.org/project/tardis and extract in 
your Drupal installation under sites/all/modules. 

Some notes on the more obscure settings: 


Tardis block creation
---------------------

- "How far back?" means how many links will be displayed in the TARDIS block. 
So if you put 36 months, it will attempt to retrieve 36 months' worth of posts 
(months in which there is no content don't count.)

- "Stop looking at the year" means the TARDIS won't look beyond a certain year 
in the past. 
As far as I'm concerned, there's no reasonable way to know how long have people 
been posting, unless you look into the entire node table. That's quite a lot of 
queries, so I thought I could put a year beyond which you're sure it's 
pointless to look. Defaults to 2010.

- "Block link" also lets you link to a view. The TARDIS page is pretty simple: a 
bunch of node teasers, filtered by year (YYYY) and month (MM) as separate 
arguments, and that's it. So if you have a view, say, 
example.com/recent/nodes/view, put "recent/nodes/view" here and the TARDIS 
will take care of the rest. 


Tardis page creation
--------------------

Everything here is pretty self-explanatory, so play with the settings until 
they suit your needs. Please note: if you change either the page title or 
address, you'll have to rebuild the cache. The TARDIS will display a handy 
link where you can do just that. 


Special thanks
--------------

- My mom (hi mom!)
- Prometheus6 for the original idea: http://drupal.org/project/montharchive
- The jolly good chaps at Drupalize.me. If you're looking to build a module and
don't know where to start, ask them!
- The generous and patient reviewers, including, but not limited to, ycshen,
Samuel Joos, FlakMonkey46, 20th, klausi and cweagans.
