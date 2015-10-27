
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Configuration


INTRODUCTION
------------

Current Maintainer: Fred Parke <mail@fredparke.co.uk>

The purpose of this module is to nicen-up the local tasks menu (aka tabs).

Adding icons to the tabs alone instantly adds a lot of readability and
distinguishes the tabs from one another nicely.

You can select which icons you want for each tab on the configuration page.

The Fred Tabs theme is an optional setting that adds some extra style and some
basic CSS3 animations to the tabs. On top of this is the option to move the tabs
outside the flow of the content, stacking the vertically on larger screen widths
to reduce the space taken up.

The icon font can also optionally replace the default cog icon used for the
contextual links matching the icons used for the tabs.

Supported in all modern browsers including IE8 and above, however a doctype must
be used for IE8.


INSTALLATION
------------

1. Install the module in the usual way (http://drupal.org/node/895232)


CONFIGURATION
-------------

Go to Administration > Configuration > User interface > Icon tabs
(admin/config/user-interface/icon-tabs)

You can enable or disable the Fred Tabs theme here. Icons will be added to the
tabs either way but most of the styles will not be added unless the theme is
enabled.

You may also choose to vertically stack tabs for larger screen widths.
This will not be ideal for all themes. For best results ensure the block is in a
region that spans the full width of the content area or is at least always on
the left side. For example, the 'content' region is generally not the best
region as it often has sidebar regions either side of it whereas a region like
'preface' may not and is therefore a more suitable region. Note that the Icon
Tabs block must be used for this setting.

You can, if you wish, use your very own icon font instead of the default one
provided by Icon Tabs. You will need an SVG version of your font for this. You
will aslo need to make sure you have a @font-face declaration for your font in
your theme - you will be responsible for making sure this is on the page
wherever your tabs appear. If you enable/disable this setting you will need to
reset the icons you want your tabs to use.

You can chose which icon from the icon font you want for each tab here. Note
that a tab must have been generated at least once to be shown on this list.

Enabling 'Icon Contextual links' will use the icon font to replace the default
cog icon used for the contextual links to match the icons used for the tabs.
