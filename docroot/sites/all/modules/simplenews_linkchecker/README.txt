
DESCRIPTION
-----------

Simplenews Linkchecker is an addon module for simplenews. It checks if there are any broken links 
within your newsletter texts. This is done before a mail is sent: a list of all contained links/references 
is displayed on the "Newsletter" tab of your newsletter content type.


REQUIREMENTS
------------

 * Simplenews module installed
 * HTML-format newsletters and/or newsletters HTML content.
 * The Full HTML text format has to be enabled on related fields of your newsletter content type. 

INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews_linkchecker" in the sites/all/modules directory and
    place the entire contents of this simplenews_linkchecker folder in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page.
    
USAGE
-----

Choose a node of a Simplewnews newsletter content type and visit the "Newsletter" tab.
If your mailbody contains any links they will be listed and analyzed on the tab page. If 
there are any broken links they will be displayed in a red row and you will have to correct
them in order to send the newsletter.

You can still send emails with broken links by enabling the checkbox "Ignore broken links".

What you could try is embedding a <base> tag. Unfortunately this won't be interpreted by most mail
clients and the tag will not be in the <head> section of the resulting HTML mail. If you know how to
get a <base> tag to work within HTML mails please let me know by opening an issue on
http://drupal.org/project/issues/simplenews_linkchecker?categories=All

NOTES
-----

This module will extract HTML links/references from the following possible elements/their uri attributes:

  - Anchors <a>: href attribute
  - Images <img>: src attribute
  - Links <link>: href attribute,
  - Scripts <script>: src attribute
  - Iframes <iframe>: src attribute
  - Areas <area>: href attribute
  
See http://www.w3.org/TR/html401/intro/intro.html chapter 2.1.3 for further information on the link topic.
  
Absolute links as they are considered in this module have to follow one of those patterns:

  - protocol://www.example.com
  - protocol://www.example.com/path/to/file
  - protocol://www.example.com/path/to/file?querystring
  - mailto:example@example.com
  
Everything else will be considered as relative link. Relative links as the name says are relative to the path of 
the file that they appear in. In conclusion, if you have a HTML file (in this case similar to a drupal node) that
contains relative links, the links can only be followed if the HTML links are clicked on the original server which
where the file resides. If you send this piece of HTML within a mail the relative links will be broke because the
linked resources won't be around (it's just a mail and not a website folder structure with all files, images etc.).

For further informations about the definition of a relative/absolute link please read the following resources:

http://www.ietf.org/rfc/rfc1808.txt

http://www.w3.org/TR/html401/intro/intro.html
http://www.w3.org/TR/html401/struct/links.html
http://www.w3.org/TR/html401/types.html#type-uri

URI
http://www.ietf.org/rfc/rfc1630.txt

URL
http://www.ietf.org/rfc/rfc1738.txt

URL (Relative)
http://www.ietf.org/rfc/rfc1808.txt