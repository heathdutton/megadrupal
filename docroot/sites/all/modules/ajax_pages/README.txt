Ajax pages
===============
Enables loading of the site's pages via Ajax.

Installation
------------
* Install this module the usual way.

Use
---
* In your theme's html.tpl.php file, wrap the output of the page_top, page and
  page_bottom regions inside a div with a unique id. For example:

  ...
  <div id="page-wrapper">
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $page_bottom; ?>
  </div>
  ...

* Go to the settings page at Administration -> Configuration -> User interface 
  -> Ajax pages and define the Page wrapper selector.

* Define a selector for all of the links that should trigger an Ajax pages load.
  Put this in the "Ajax links selector" field.

* Enter the paths of any pages that you do not want to be loaded via Ajax. Note
  that Ajax pages skips admin pages automatically.

The contents of the wrapper should now be reloaded via Ajax when clicking page
links without doing complete page reloads.

Adding a persistent region
--------------------------
The module is of little use unless there's persistent content on the page that
doesn't get reloaded as the user clicks links and/or submits forms on the
website. An example of such persistent content would be an audio player that
keeps playing as the user browses the website.

To create a persistent region that will be output outside of the page wrapper:

* Edit your theme's .info file by adding the following line:

  regions[persistent] = 'Persistent'

* Add the following code to your theme's implementation of
  hook_preprocess_html() in template.php:

  $variables['persistent'] = render($variables['page']['persistent']);

  For the Omega theme, the above should look like this:

  $variables['persistent'] = render($variables['page']['#excluded']['persistent']);

* Add the following code to your theme's html.tpl.php OUTSIDE of the wrapper
  you created above:

  <?php print $persistent; ?>

  e.g.:

  ...
  <?php print $persistent; ?>
  <div id="page-wrapper">
    <?php print $page_top; ?>
    <?php print $page; ?>
    <?php print $page_bottom; ?>
  </div>
  ...

* Clear all caches.

* Visit the Structure -> Blocks page to add a block to the newly created
  Persistent region.
