Name: Permalink block (permalink_block)
Author: Martin Postma ('lolandese', http://drupal.org/user/210402)
Drupal: 7.x


-- SUMMARY --

A block with an link or HTML code snippet that links to the current page,
providing an easy way for visitors to embed deep links to your pages on their
website.

The URL contains the node or term ID instead of the alias, so the link won't
break if the title changes. Hence the term 'permalink'.

If the BeautyTips or Popup module is enabled, only the text 'Permalink' is
displayed, with a popup that opens on hover.

By default the block is enabled at the bottom of the theme's 'content' region on
all non-admin pages. This shows a permalink to the current page and optionally
a copy box to ease pasting of the necessary HTML into other websites.


-- USAGE --

Despite the absence of a settings page for the Permalink module, display can be
customized through block settings, a CSS and a template file.

To toggle the popup functionality off/on, disable/enable the BeautyTips or
Popup module.

To position the Permalink on the page and exclude it from some pages use the
block settings. You can also select to show only the URL or HTML for a full
link, the behaviour and width of the popup.

To customize the output, edit the module CSS (permalink_block.css) and template
file (permalink_block.tpl.php). Just follow instructions in the code comments.

Best practice is to copy CSS code and template files to your used theme to avoid
them to get overwritten if you update the module.

After making your changes, clear both your browser and site cache.

To override the string "Permalink" or the help text use
http://drupal.org/project/stringoverrides which provides an easy way to replace
any text on your site that's wrapped in the t() function.
