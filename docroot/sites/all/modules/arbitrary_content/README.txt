Arbitrary Content is a module that allows users to create and manage small pieces of structured content that can be exposed as blocks or pages.  These content elements are created by defining a set of fields (textfield, textarea or filefield-like in nature, although the module does not use CCK for these fields) and the associated content.  The practical upshot of the module is that this gives users and administrators a way to have consistent one-off block and page elements for their site without needing to build them using raw html and giving them the ability to individually theme each block or page.

The use case may not seem immediately obvious, but consider the following example: Your client has a design that has, on a certain page, one or more blocks with information that can be considered “semi-static;” that is, likely to change at some point in the future, but not enough to really be considered a dynamic piece of the site.  The eventual administrators of the site will want to be able to have access to edit this content in some form or fashion. 

There are a number of possible solutions, each with their own strengths and weaknesses:
      You can create a node with a specific taxononmy, or perhaps even a totally new content type for the content, then use Views to find the specific piece of content and display it as a block.  In many cases, using a full node for a small bit of content will be overkill.

      Create a custom block and use regular html markup to create the content.  Perhaps the most common way of solving this problem, but suffers from a potential lack of supportability when someone later tries to edit the markup with a WYSIWYG editor, ruining your hand-coded markup and breaking styling.  You could also create a module to create the block, which may also be overkill for the small amount of content they’re wanting to display.
      Embed the markup directly in the page.tpl.php - safer than the block, but obviously not nearly as supportable by the end-user.
      Use Panels and it’s Custom Content Panes.  If your site already utilizes Panels, this is an option, but one would hate to implement Panels just to solve this problem.  In addition, Custom Content panes are still just blocks of html markup that your users may not feel able to manage.


The Arbitrary Content module attempts to solve this problem by giving you the ability to create these blocks with any number of fields in a manner that is easy for an end user to manage while still giving you the power of individual templating and theming for each block.  The module is very light-weight in comparison to using full nodes with Views or Panels.  With the theming ability, you can safely entrust the management of the content to non-developers while keeping the display attributes of your custom content safely in code.  It plays nicely with the Drupal Block System, Context and Panels. 

While Block creation may be the most obvious use, it can also generate entire pages of content, registered with the menu system.  Again, sometimes a node might seem to be inadequate (or too heavy handed) for generating some semi-static page specified in an IA document.  One could even conceivably use Arbitrary Content to build a site’s homepage.

Additions coming in the near future include exportability and/or Features compatibility, field weights and token support.
