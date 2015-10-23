
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installing
 * How can you contribute


INTRODUCTION
------------

Current Maintainer: gianfrasoft <http://drupal.org/user/363149>

Nice Taxonomy Menu is useful to generate well formed vertical menu blocks
to represent taxonomy trees and make access quickly to their related contens.

Anytime you generate a new Nice Taxonomy Menu, the module generated a new 
related block for displaying it.

Leaf terms in the tree (that belong to a specific vocabulary) have a link
you can customize: for example, you create a simple view (with a page
visualization) on nodes which have a field that is a "term reference" to
the same vocabulary, with a contextual filter related to the sema field.
Then you put the URL of the visualization in the "Link URL" of the Nice
Taxonomy Menu. Every time you click on a leaf, the page loads your view's
visualization page passing the correct tid for filtering.

Javascript code inside the menu generates a minimal animation for 
collapsing and expanding the intermediate elements.

Every Nice Taxonomy Menu contains CSS classes that make the display
customization very easy.



INSTALLING
----------

Install normally. Remember this module is an extension for Taxonomy module. 

Once Nice Taxonomy Menu is installed and enabled, you can access to the 
/admin/structure/taxonomy/nice_taxonomy_menu page.

On the administration page you can declare the number of Nice Taxonomy Menu 
you need to use (you can increasing or decreasing this number anytime) and 
associate them to your links (views or otherwise).



HOW CAN YOU CONTRIBUTE?
-----------------------

Report any bugs, feature requests, etc. in the issue tracker.

Write a review for this module at drupalmodules.com.
