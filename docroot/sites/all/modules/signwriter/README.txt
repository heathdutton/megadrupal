------------------------The Signwriter Drupal Module-------------------------

The Signwriter module allows you to use custom fonts in headings. It does this
by replacing html headings with an image generated from a TrueType font file
which you provide. It also has a number of additional features:

 - Multiple profiles allow you to have different settings for different
   headings.

 - Profiles can provide input filters to replace text matching a regular
   expression.

 - Images generated can be transparent or opaque.

 - Text can be positioned within a background image.

 - Text can be left, right, or center aligned.

 - Font size can be automatically reduced to fit the text within a specified
   maximum width.

 - Images are cached to improve performance under high load.

 - Generated images can be gif, png, jpeg, or bmp.

 - Themes can use profiles configured by a user, or create their own.

From 5.x-1.2:

 - Drop shadows can be added to the text.

 - Multiple lines of text can be made into an image.

Signwriter is made for drupal 4.7, but it can be used by themes in earlier
versions of drupal without needing to enable the module. Signwriter requires
the GD library to work properly (read on for more info).



---------------------------------Installation---------------------------------

Installation is trivial, simply untar the module:
cd /path/to/drupal/modules
tar xzf /path/to/signwriter-4.7.0.tar.gz
Next, enable the module under drupal under admin>>modules.


-----------------------------The GD Image Library-----------------------------

Signwriter uses the GD Image library (http://php.net/image). GD comes
installed by default in php >= 4.3, but can be enabled at compile time in
earlier versions. If your php installation is on windows, try uncommenting the
line which reads 'extension=php_gd2.dll' in your php.ini.

------------------------------------Usage-------------------------------------

There are two main scenarios under which you may wish to use signwriter:

1. As a filter to replace headings or custom tags in node text, and

2. To generate headings in a theme.

In the second scenario, you can use signwriter profiles administered under
drupal, or you can create your own in php.

------------------------Creating Profiles Under Drupal------------------------
A signwriter profile is a collection of settings which can be used to create an
image. To manage your profiles, go to admin>>settings>>signwriter. From here
you can add, delete, or edit profiles. If you add or edit a profile, you will
be taken to a profile settings page. On each profile settings page there are a
number of options which may be set, each of which is explained on that page.



--------------------------Scenario 1: Using Filters---------------------------

If a signwriter profile has a pattern defined then it will be available as an
input filter under admin>>input formats. After enabling the filter, any text
matched by the pattern will be replaced with a signwriter image.

-----Example: Replacing h2 headings in nodes-----

1. Create a profile under admin>>settings>>signwriter called 'H2 Filter', making
sure to enter /<h2>.*?<\/h2>/ under 'Pattern to Match When Used as
a Filter'. There are some example patterns on the page.

2. Go to admin>>input formats and edit your input format of choice. There
should be an option to enable 'H2 Filter'.

3. After enabling the filter, create a new page using your input format of
choice. Make sure you put in at least one h2 heading:
<h2>signwriter filter test</h2>.

4. Your headings should be replaced by signwriter images in the submitted page.



-------------------------Scenario 2: Theme Development------------------------

To use signwriter in a theme you have two options to configure it. You can
enable the module in drupal and configure it on the admin>>settings>>signwriter
page by adding one or more profiles, or you can create profiles yourself using
php.

-----Creating and Loading Profiles in php-----

If you are administering your profiles under drupal, then in your theme code
you will want to load one like this:
<?php $profile = signwriter_load_profile('Example Profile'); ?>
Note that you can pass a profile name or id to signwriter_load_profile().

As a php object, a signwriter profile has the following fields, most of which
correspond to a setting on the edit profile form:
<?php
$profile->text //The text to display. Can contain html entities.
                //For example, &amp; will be displayed as &
$profile->fontfile
$profile->fontsize
$profile->foreground
$profile->background
$profile->width
$profile->height
$profile->maxwidth
$profile->imagetype
$profile->cachedir // defaults to <drupaldir>/signwriter-cache
$profile->textalign
$profile->transparent
$profile->bgimage
$profile->xoffset
$profile->yoffset
?>

Any of these fields can be overridden in the theme after loading the profile.

If you want to create a profile from scratch then you will at least need to
define $profile->fontfile all other settings will revert to
defaults if not set. An advantage to creating the profile from scratch is that
the module doesn't need to be enabled, and meddlesome users can't ruin your
nice headings.

After you have loaded or created a profile you can use signwriter_title_convert
or signwriter_text_convert to replace, for example, your $title in page.tpl.php
(if you're using phptemplate).

-----Example 1: Replacing the page title in your theme-----

1. Create a signwriter profile in drupal called 'Theme Heading', and assign
the other settings to your liking.

2. In your phptemplate theme add the following code to your page.tpl.php where
you want to print the page title:
<?php
if ($title != '') {
    $profile = signwriter_load_profile('Theme Heading');
    // at this point you could override any settings. for example:
    // $profile->fontsize = 43; // override the font size
    print signwriter_title_convert($title, $profile);
}
?>

3. Refresh the page in drupal and your title should appear in your custom font.

-----Example 2: Using signwriter without configuring it in drupal-----

1. Add your custom font to your theme directory. In this example we'll use Arial.ttf.

2. In your phptemplate theme add the following code to your page.tpl.php:
<?php
if ($title != '') {
    $profile->fontfile = 'Arial';
    $profile->fontsize = 15;
    $profile->foreground = 'ff0000'; // red
    $profile->background = 'ffffff'; // white. If your text is jagged then change this to your page background colour
    $profile->maxwidth = 600;
    $profile->transparent = true;
    print signwriter_title_convert($title, $profile);
}
?>

3. Refresh the page in drupal and your title should appear in red Arial size 15 text.

-------------------------------About Signwriter-------------------------------
Signwriter was created by Agileware (http://www.agileware.net).
And ported to Drupal6 by Catorg (http://www.catorg.co.uk) and Agileware.

