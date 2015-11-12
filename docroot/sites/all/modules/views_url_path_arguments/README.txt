
-- SUMMARY --

Simple module to convert a view contextual filter node id into its url path.

Extended use case: A website has a view that uses node reference or entity 
reference to filter the results set.  For example, a regional location node
is a node reference on a bunch of news stories.  The view is setup to filter
and show only news stories in the Chicago region and not any news stories
from the Kansas City region.  Rather than using nid for the contextual
filter (which isn't SEO friendly) these plugins allow the ability to use
SEO friendly url path / path alias.

For a full description of the module, visit the project page:
  http://drupal.org/sandbox/lucashedding/1872408

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/1872408


-- REQUIREMENTS --

Views 3.x.


-- INSTALLATION --

* Install as usual, for further information see 
  http://drupal.org/documentation/install/modules-themes/modules-7.


-- CONFIGURATION --

There are two plugins available in this module. The default argument plugin 
should be used with blocks or things that don't naturally have arguments
available to them. The validator plugin should be used to convert the nid for
views that naturally have arguments available to them, such as pages. They 
can be used together if a view is rendered in both a block and a page.  Or 
seperately if the site's view is rendered only as a page or only as a block.

* Configure contextual filters in Administration » Structure » Views » view:
  Add a contextual filter i.e. Content: Nid

    * Argument not available to views?
       When the filter value is not available » Provide default value »
       URL path alias from URL
    * Argument available to views?
        When the filter value is available or a default is provided »
        Specify validation criteria » Validate URL path alias


-- FAQ --

Q: Why would I use this module?

A: For SEO purposes you want to use the url path a.ka. path alias as an 
   argument to a view contextual filter.


-- CONTACT --

Current maintainer:
* Lucas Hedding (heddn) - http://drupal.org/user/1463982
