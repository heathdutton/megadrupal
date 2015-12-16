
-- SUMMARY --

The Word Link module allows you to automatically convert
specific words into links. It works as text filter.
This can be useful for crossposting your site's pages,
or for the contextual advertising of your partners (SEO).

* Features:
- Convert word in content with a link or just wrap it in with class.
- Can set on which content type it will be affected.
- Can set a list of HTML tags that will be ignored.
- Can specify case sensitivity.
- Can set a path on which words will not be converted or path only on which it will be converted.
- Works for Cyrillic (PCRE 8.10 or higher).
- Import words from taxonomy terms.
- Import words from CSV file.
- Export words to CSV file.
- Bulk delete operation.
- Sortable tableselect with pager.

* Need to know:
Word Link module uses DOMDocument class that build in PHP from version 5.
And for loading HTML it uses loadHTML method. This means that your HTML
does not have to be well-formed, but if it is not, it can cause errors,
especially when using deprecated tags or HTML 5 tags. So it’s better that
your fields/comments HTML meet the requirements of XHTML 1.0.

* Example:
"Nam aliquam egestas congue. Sed at odio odio, quis viverra dolor.
Vestibulum sed mauris id elit vehicula tincidunt. Integer quis magna
tortor, non ultrices elit. Nunc quis amet."

Would become:
"Nam aliquam egestas congue. Sed at odio odio,
<a href="www.drupal.org" target="_blank">quis</a> viverra dolor.
Vestibulum sed mauris id elit vehicula tincidunt. Integer
<a href="www.drupal.org" target="_blank">quis</a> magna tortor,
non ultrices elit.
Nunc <a href="www.drupal.org" target="_blank">quis</a> amet."

-- REQUIREMENTS --

- PHP 5.
- For unicode symbols PCRE (Perl Compatible Regular Expressions) 8.10 or higher.


-- INSTALLATION --

* Put the module in your drupal modules directory and enable it in
  admin/modules.


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:
Go to admin/people/permissions and grant permission to any roles
that need to be able to add and edit words.

* Configure text filter:
Go to admin/config/content/formats and edit text format.
Add word link as filter to the text format and configure it.

* Add a new word:
Just follow the admin/config/content/word-link/add and fill out the form.
Also here you may specify case sensitivity, link title and inner drupal path
on which words will not be converted.
