ABOUT
-----

This is a simple filter module. It handles <code></code> and <?php ?> tags so
that users can post code without having to worry about escaping with &lt; and
&gt;


INSTALLATION
------------

1. Copy the phplighter folder to your website's sites/all/modules or
   sites/yoursite.com/modules directory.

2. Download the PHPLigther package from https://github.com/brandonwamboldt/phplighter
inside the module folder. You will have something like sites/all/modules/phplighter/PHPligther

Instead of download a zip, you can just clone the entire repository:

cd sites/all/modules/phplighter
git clone git://github.com/brandonwamboldt/PHPLighter.git

3. Enable the phplighter.module on the Modules page.

4. Go to Configuration > Text formats. For each format you wish to add Code
   Filter to:

  a. Click the "configure" link.

  b. Under "Enabled filters", check the phplighter checkbox.

  c. Under "Filter processing order", rearrange the filtering chain to resolve
     any conflicts. For example, to prevent invalid XHTML in the form of
     '<p><div class="phplighter">' make sure "Code filter" comes before the
     "Convert line breaks into HTML" filter.

  d. Click the "Save configuration" button.

5. (optionally) Edit your theme to provide a div.codeblock style for blocks of
   code.

6. (optionally) You can configure which theme use for this module by adding a
   custom css directly your settings.php file, i.e:

   $conf['phplighter_theme'] = 'sites/all/themes/yourtheme/phplighter.css';

   or using one of the provided by the PHPLighter package:

   $conf['phplighter_theme'] = 'sites/all/modules/phplighter/PHPLighter/css/twilight.css';


CREDITS
-------

This module is based on http://drupal.org/project/codefilter
