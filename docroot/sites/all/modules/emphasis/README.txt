OVERVIEW
================================================================================

Emphasis provides dynamic paragraph-specific anchor links and the ability to
highlight text in a document, all of which is made available in the URL hash so
it can be emailed, bookmarked, or shared.


INSTALLATION
================================================================================

The emphasis module adds new new field formatter, which is available for the
text, text_long, and text_with_summary field types. To enable emphasis
functionality, you must apply the emphasis formatter to a given field.

1. Download emphasis and libraries modules.
2. Download madmatter23's fork of emphasis library from:
https://github.com/madmatter23/Emphasis
3. Place in sites/all/libraries/emphasis.
4. Enable emphasis module.
5. Navigate to 'Manage Display' for a given entity type.
6. Choose an emphasis capable field formatter for a text field. By default
   this includes text_default, text_plain, text_trimmed, and
   text_summary_or_trimmed.
7. Modify the formatter options and check 'Enable emphasis' option.


CONFIGURATION
================================================================================

The emphasis module may be configured at admin/config/user-interface/emphasis.


USAGE
================================================================================

For a more detailed description of usage, please visit:
http://open.blogs.nytimes.com/2011/01/11/emphasis-update-and-source/

Here is an instructional excerpt from that page:

1. Find an article such as the following (or you can use this blog post!):
http://www.nytimes.com/2011/01/04/science/space/04telescope.html

2. Double-tap the Shift key. Paragraph symbols ¶ will appear next to each
paragraph — just as they did in the original version. Click any paragraph symbol
to generate a link to that paragraph. The link will appear in your browser’s
address bar.

3. Click a paragraph. A light gray shading will be applied to the text of the
paragraph, indicating that it’s the currently selected paragraph. If you don’t
see this, it’s possible you still have the previous version in your browser
cache — try refreshing the page a few times.

4. Click a sentence within the currently selected paragraph. This toggles
highlighting on each sentence (click a few different sentences to see how it
works).

5. Click a different paragraph. You’ll see the gray shading again, confirming
that the new paragraph is the active paragraph.

6. Copy the URL from your browser’s address bar. The URL will automatically be
updated as you click around, so it always reflects the current appearance of the
page.
