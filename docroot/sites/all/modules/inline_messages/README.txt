================================================================================
Inline Messages Module
================================================================================
The Inline Messages module displays form messages inline with the form instead
of the default behaviour at the top of the page.

You can enable Inline Messages for your site, content editing or administration
pages. Additionally you can also specify forms to include or exclude by id.

It will also work for pages with multiple forms.

An example usage is with the comment form, which is typically placed at the
bottom of the page. If there's an error be when someone submits, they jump back
to the top of the page. So to get back to the comment form, you have to scroll
back to the bottom of the page.

Using jQuery and the jQuery ScrollTo plugin, Inline Messages will
capture the $messages, place them just above your form and move the page
to the top of the form.

--------------------------------------------------------------------------------
For more information on the project and to submit issues and patches
visit the following page: http://drupal.org/project/inline_messages


================================================================================
NOTE: Requires the jQuery ScrollTo plugin
      http://flesler.blogspot.com/2007/10/jqueryscrollto.html
      
      Demo: http://demos.flesler.com/jquery/scrollTo
================================================================================

================================================================================
Credits
================================================================================
Inline Messages contributed by jsfwd / ninjagirl (http://ninjagirl.com)