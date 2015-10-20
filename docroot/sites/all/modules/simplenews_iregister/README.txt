DESCRIPTION
-----------

Simplenews publishes and sends newsletters to lists of subscribers.

This module generate the code for an iframe that will show a form to register a
user in a newsletter.

There are some keys and a list of sites to check that the iframe is authorized.

REQUIREMENTS
------------

 * Simplenews

INSTALLATION
------------

 1. CREATE DIRECTORY

    Create a new directory "simplenews_iregister" in the sites/all/modules
    directory and place the entire contents of this simplenews_iregister folder
    in it.

 2. ENABLE THE MODULE

    Enable the module on the Modules admin page.

 3. ACCESS PERMISSION

    Grant the access at the Access control page:
      People > Permissions.

 4. CONFIGURE

    General configuration of Simplenews Iregister has found on the tab under
    Simplenews admin pages:

      Configuration > Web Services > Simplenews.
      http://www.example.com/admin/config/services/simplenews/iregister

    More detailed configuration has required for every newsletter:

      Configuration > Web Services > Simplenews > Newsletters (edit newsletter
      category).
      http://www.example.com/admin/config/services/simplenews/categories/1/edit


CONFIGURATION
-------------

- IFRAME GENERAL KEY

It needs a key to verify the authenticity of the URL inside the iframe. This is
a general key that is combined with newsletters keys.

Once saved the key, it's important never change it to avoid that all URLs into
the old iframe will be rejected.

- IFRAME KEY

For every newsletter inside this new fieldset, 2 new newsletter options are
required:

 * The key to validate the iframe - the newsletter own key, which is combined
   with the general key;

 * The URLs where are the iframes - a separated by commas string of domains or
   IPs where will have put the iframe code. If a site isn't in this list, then
   it will be rejected.

After have saved the page, there will be showed the iframe code to the copy in
the remote site.

- IFRAME CONFIRM URLs

If set Double on Opt-in/out method, it's possible to choice to use an internal 
"anonymous" page instead of the default  pages from simplenews. This "anonymous" 
page will use the same CSS rules set in "IFRAME CSS".

There are also some templates to customize these "anonymous" pages (add logo, 
footer, etc.): 

templates/page--simplenews-iregister-confirm.tpl.php
templates/page--simplenews-iregister-done.tpl.php

It's possible to define every template for a newsletter, renaming the templates 
like 

page--simplenews-iregister-confirm--[tid].tpl.php
page--simplenews-iregister-confirm--add--[tid].tpl.php
page--simplenews-iregister-confirm--remove--[tid].tpl.php
page--simplenews-iregister-done--[tid].tpl.php

It's preferred copy/create them in your theme or subtheme.

Instead of the "done" template, it's possible to set an URL to your site or to 
the site where is the iframe or wherever, as further customized page.

- IFRAME CSS

To optimize the iframe to be similar the remote site theme, it's possible to 
add the CSS rules for the iframe content and for the customized templates.

- NOTES

The form in the iframe can be configured (block title and message) from the 
newsletter block configuration page 

http://www.example.com/admin/structure/block/manage/simplenews/[tid]/configure . 

The code above that you have copied in your site can also get the browser 
language and it send it to the module, so if you configure the language and 
you localize the block title and message, you have the iframe localized.

Like the confirmation pages, also for the iframe there are the templates to 
customize:

page--simplenews-iregister-iframe.tpl.php

or better

page--simplenews-iregister-iframe--[tid].tpl.php

