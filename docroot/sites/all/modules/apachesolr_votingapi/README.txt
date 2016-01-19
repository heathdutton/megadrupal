Description
----------------

This module integrates the Voting API results with Apache Solr sorting.



Customization
----------------

You want to set the variable 'apachesolr_votingapi_criteria' in your settings.php. This variable is responsible for picking out which tally for a node is used by Apache Solr for sorting. You might want to use a custom function or custom content type here.

The default value of the variable 'apachesolr_votingapi_criteria' is suitable for a Fivestar ratings implementation. The default value is:

array(
  'title' => 'Rating',
  'content_type' => 'node',
  'value_type' => 'points',
  'function' => 'average',
  'tag' => 'vote',
)




Original Author
---------------
Moshe Weitzman <weitzman AT tejasa.com>


Current Maintainer
------------------
Yonas Yanfa <yonas AT fizk.net>
