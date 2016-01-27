***********************************************************************
                     D R U P A L    M O D U L E
***********************************************************************
Module Name        : Slideshow Creator
Original Author    : Bruno Massa http://drupal.org/user/67164
Maintainer and D7  : Ron House http://drupal.org/user/171604
General Links:
Project Page       : http://drupal.org/project/slideshow_creator
Support Queue      : http://drupal.org/project/issues/slideshow_creator

***********************************************************************


DESCRIPTION
-----------

Slideshow Creator creates a true slideshows using any image over
internet with many other features. If the user does not have
JavaScript enabled, it degrades to a "regular" slideshow where the
"next" button points to the next image and a whole new page is
loaded.


FEATURES
--------

* (D6 only) CCK: Slideshow Creator has its own widget to CCK
* Automaticaaly extract images from a given directory
* Convenient: can be inserted in any node type
* Customize: add as many images you want, rotate speed and even more
  than one slideshow per page
* Lightweight: the JavaScript file is very small
* Themable: use a CSS to customize the look
* Usable: JavaScript enhance the slideshow, but it is not required
* Valid: the code is XHTML 1.0 Strict


USE
---

In any node, add the string:

[slideshow:VERSION, rotate=ROTATE_SPEED, blend=BLEND, layout=LAYOUT,
 order=ORDER, class=CLASS_NAME, height=HEIGHT, width=WIDTH,
 img=|IMAGE_URL|LINK|TITLE|CAPTION|TARGET|,
 img=|IMAGE_URL|LINK|TITLE|CAPTION|TARGET|]

where:

VERSION [required]:        the slideshow filter version: currently 2

ROTATE_SPEED [optional]:   the speed, in seconds, to rotate images
	     		   (0 to not rotate at all). Corresponds to Cycle
			   plugin's timeout/1000 (since timeout is in
			   milliseconds). You can also timeout directly
			   if you prefer.

CLASS_NAME [optional]:     You can theme your slideshow thru CSS by giving
	       		   it one or more class names. (This option is now
	       		   called 'class', but used to be called 'name'.
	       		   'name' still works, but is deprecated for being
	       		   uninformative.) Do NOT put quotes around the
	       		   argument. To provide more than one class name,
	       		   separate them with SPACES. Class names are added
	       		   to those already specified earlier (for example
	       		   in the global Default Tags list). To remove
	       		   previously specified class names, use "class="
	       		   with an empty list of classes, then provide
	       		   another "class=" argument to specify the class
	       		   names you want.

HEIGHT [optional]: 	   all images will be at the same height

WIDTH [optional]: 	   all images will be at the same width

BLEND [optional]: 	   how long, in seconds, will take the fading
      			   transaction between images). Corresponds to
			   Cycle plugin's speed/1000 (since speed is in
			   milliseconds). You can also speed directly
			   if you prefer.

LAYOUT [optional]:	   (See below)

ORDER [optional]:	   (See below)

LAYOUT and ORDER work in tandem, but either or both may be omitted.
LAYOUT was originally the only option for ordering the pieces of the
slideshow, but due to a very long-standing bug, not all options were
distinct. Fixing this would have broken many slideshows that users
had designed while the bug was in force, so in order to provide all
the missing options, a new directive, ORDER, was designed so that
the bug can be removed without changing the behaviour of existing
slideshows. They now work as follows:

We consider a slideshow to consist of two parts: the slideshow
proper and the 'next/previous' control line. LAYOUT controls the
order of these parts:

LAYOUT: takes options 'top', 'bottom', and 'none' (and still
recognises the buggy old option 'reverse', which is identical to
'top'):

	top:	the slideshow proper appears above the control line
	bottom:	the slideshow proper appears below the control line
	none:	the control line is not displayed

Next, the slideshow itself is considered to consist of three parts,
a title (usually bolded), a caption, and the image. ORDER controls
the position of the image relative to the other two parts. The title
is assumed to normally appear above the caption. ORDER options are:

   	top   	       The image is on top of the title and caption
	middle	       The image is between the title and caption
	bottom	       The image is below the title and caption
	reversetop     As 'top', but the caption is above the title
	reversemiddle  As 'middle', but the caption is above the title
	reversebottom  As 'bottom', but the caption is above the title

After specifying the initial options, then for each image you want
to insert, use an "img=" tag with these fields in this order:

IMAGE_URL [required]:      the image itself

LINK [optional]:           if you want to be clicky to provide a link to
     			   some page, put the URL here

TITLE [optional]: 	   often the bold text over the image

CAPTION [optional]:        often the text under the image

TARGET [optional]: 	   where the linked page will show:
       			   Default is to show in the same window,
			   _blank will show in another window,
			   _parent and _top is only used when pages
			   have frames, _self shows on the very
			   window or you can use a window name.

Or you can automatically extract images from a given directory:


All images in a directory:
--------------------------
You can create a normal slideshow, but you can scan for more images
using

dir=|DIR_IMAGE|RECURSIVE|LINK|TITLE|CAPTION|TARGET|

where:

DIR_IMAGE [required]:      the directory that you want to scan for images.
	  		   The folder base is the default site files
	  		   folder, generally "sites/default/files".

RECURSIVE [optional]:      "yes" if you want to scan recursively or
	      		   leave it blank

LINK [optional]: 	   "yes" if you want link all images to their own
	  		   path, or leave it blank, or provide a URL
	  		   that all images will link to.

Overriding TITLE, CAPTION, LINK and TARGET for particular images:

When using "dir", you may only specify one TITLE, CAPTION, LINK and
TARGET in the slideshow code and it applies to every image found in the
directory. To override, include in the same directory a control file with
the same name as the image file, but with the image extension replaced by
".sscctl". For example, the image "fred.jpg" should be accompanied by a
control file called "fred.sscctl". In this control file, include any or
all of these lines in any order (examples):

Title: The title to apply to this image
Caption: The caption to apply to this image
Link: http://example.com
Target: _blank

The "Link:" line may also be:
Link: yes
meaning to link back to this image itself.

Unrecognised and empty lines in the ".sscctl" file are silently ignored.


Example of a slideshow:
-----------------------

[slideshow: 2, height=240, width=300,
img=|http://drupal.org/files/druplicon.small_.png|drupal.org|Drupal|The
ultimate CMS. Download it now!|Drupal|,
img=|http://www.mysql.com/common/logos/mysql_100x52-64.gif|http://www.mysql.com|MySQL|Free
and reliable SQL server and client.|_self|,
dir=|files/|yes||Generic Photos|Aren't they great?||]


Default Link, Title, Caption, and Target:
-----------------------------------------

Starting with versions 6.x-1.44 and 7.x-1.8, it is possible to specify a
default value for these four slide options. This is most useful for the
title, as it is common for a slideshow to have the same title on each
slide. This is done by putting any of these options in the slideshow
settings:

link=URL
title=A Title
caption=A caption
target=TARGET

Example:
[slideshow: 2, height=350,width=406,
title=Our Big\, Cute Title,
caption = Our Caption,
link=http://example.com/demos,
img=|http://example.com/files/images/ssh_1488.jpg|http://otherplace.org||||,
img=|http://example.com/files/images/ssh_1499.jpg|||Special Caption||,
img=|http://example.com/files/images/ssh_14xx.jpg|||||,
title=A Replacement Title,
img=|http://example.com/files/images/ssh_14yy.jpg|||||,
img=|http://example.com/files/images/ssh_14zz.jpg|||||]

Note that link, title, caption, and target attributes only apply for all
img and dir tags that follow the attribute, and the values can be
overridden simply by providing a non-blank value in the normal way. In
the above example, the default caption is "Our Caption", but the second
image overrides this for that image only with the value "Special
Caption". Likewise the default link is example.com/demos, but the first
image overrides this with otherplace.org.

Also note that the default values set by these attributes can be
changed part way through by including another link, title, caption,
or target attribute, as with the title attribute in the above example.

Finally, note that these values are not quoted, but if they include a
comma, it must be preceded by a backslash.


ADDITIONAL FEATURES
-------------------

The module recognises all of the following standard jQuery Cycle plugin
options:

    fx,
    timeout,
    continuous,
    speed,
    speedIn,
    speedOut,
    next,
    prev,
    prevNextClick,
    pager,
    pagerClick,
    pagerEvent,
    pagerAnchorBuilder,
    before,
    after,
    end,
    easing,
    easeIn,
    easeOut,
    shuffle,
    animIn,
    animOut,
    cssBefore,
    cssAfter,
    fxFn,
    height,
    startingSlide,
    sync,
    random,
    fit,
    pause,
    autostop,
    autostopCount,
    delay,
    slideExpr,
    cleartype,
    nowrap.

These are described at: http://jquery.malsup.com/cycle/options.html
But note that not all necessarily make sense in the context of the
display layout as set up by Slideshow Creator. They are not
supported and you should try them out for yourself to find ones that
work well.

To use them just include them along with the other options at the
start of the slideshow, for example, to use the fx and delay
attributes:

[slideshow: 2, layout=bottom, height=382, fx='scrollUp', rotate=1,
blend=2, delay=1500, ...etc...]

But be careful: those cycle attributes that take string arguments
must be quoted. However, for cycle option fx (and only for fx!) the
quotes may be omitted if the option is one of those listed at
http://www.malsup.com/jquery/cycle/browser.html. This special case
exists to accommodate inconsistent behaviour in old versions of
this module.


THE ADMIN PAGE
--------------

Default settings may be provided on the Slideshow Creator admin
page. You can set:

Width
Height
Layout
Order
Current Slide String as shown next to the number of the slide
	      	     (defaults to the word 'slide')

Also you can specify any default tags you like as a comma-separated
string. This works just as if you had included this string at the
start of your slideshow options for every slide, except that any "]"s
should NOT be preceded by a "\". Any of the values in this default can
be overridden simply by including the same option again on the
individual slideshow (the exception being "class", which appends to
any names specified here, as explained earlier).

For example, if you want all your slideshows to use the jQuery
turnDown effect by default, and you want all images to link to
example.com, you could write:

link=example.com,fx='turnDown'


EXPERIMENTAL FEATURE: THUMBNAIL PAGER
-------------------------------------

Please see the separate README file for details of this new feature.


INSTALLATION
------------
1* Just copy the folder to your /modules/ folder
2* Activate the module in your Drupal installation.
   Go to administer > site building > modules
3* Go to administer > site configuration > input formats
   and add slideshow filter in any filter type your site have.


KNOWN ISSUES AND SPECIAL SITUATIONS
-----------------------------------

1* the "target" feature is only available thru JavaScript, in order
   to maintain the page XHTML 1.0 Strict

2* blend feature don't work on Konqueror browser 3.x, unfortunately.
   Normal on Konqueror 4.x.

3* The CCK field feature has been removed from the Drupal 7 version.
   Since this is not the critical feature of the module, it will
   only be included if someone contributes the updates from the D6
   working version.

4* To include commans or right square brackets inside text elements,
   backslash them: "\," "\]".

5* The 'name=css-class-names' feature was renamed to 'class=...' in
   versions 7.x-1.12 and 6.x-1.48. 'name' was too vague a name when
   such an obvious informative alternative is available. 'name=' still
   works, but is deprecated.

Lastly, since this is an input filter it can be affected by other
filters. In particular, line break filter, automatic URL to link
filter, etc., can mess with the slideshow details before Slideshow
Creator gets a chance to render it. If you have a problem,
re-ordering the filters might fix it. Slideshow Creator seems to
work best when placed as close to the beginning of the input filter
list as possible.


SECURITY NOTE
-------------

The filter provided by Slideshow Creator is very powerful and should
only be used in text filters that are restricted to trusted users. In
particular, do <em>not</em> add it to "filtered HTML", which can be
used by, for example, anonymous commenters, including spammers.


DEVELOPMENT
-----------

If you have suggestions or complains, don't hesitate to post your
suggestions as an issue on drupal.org. USE THE MODULE ISSUE QUEUE!
Bug reports added as comments to the how-to page and similar places
will probably never see the light of day.

If you know PHP and Drupal and want to help on this module, post
your code now! Translations are also welcome.

This module was originally sponsored by the brazilian company Titan
Atlas (titanatlas.com).
