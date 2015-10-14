Bootstrap Theme Shortcode Module provides a bridge between the Shortcode module and the Twitter Bootstrap Theme.


Dependencies:
Shortcode Module - https://drupal.org/project/shortcode
Filter (Drupal core)
Bootstrap Theme - https://drupal.org/project/bootstrap (although any theme that imports the twitter bootstrap css/js should work)


Installation.
1) Install dependancies, activate.
2) Install Bootstrap Theme Shortcode Module, activate.
3) Configure text formats (admin/config/content/formats) to enable which text formats you want to be able to use the shortcodes.
4) Within each text format settings page, scroll to the bottom and enable which shortcodes specifically you want to be made available (this means you could limit the shortcodes per text format)
5) When entering your content, use the shortcodes as described below
6) Voila - you have just funked your site to another level of funk. Eat some cake and look like a hero.


Shortcodes are a way to include extra functionality without having to know the html or having to mess about within the theme layer.
Please open an issue on the project page if you run into any trouble.


Current shortcodes included in the Bootstrap Theme Shortcodes Module are:

Tooltips
Usage:
[tooltiptop title="Your tooltip text"]text[/tooltiptop] Inserts a tooltip at the top of an element, on hover with your custom text.
[tooltipright title="Your tooltip text"]text[/tooltipright] Inserts a tooltip at the right of an element, on hover with your custom text.
[tooltipbotttom title="Your tooltip text"]text[/tooltipbottom] Inserts a tooltip at the bottom of an element, on hover with your custom text.
[tooltipleft title="Your tooltip text"]text[/tooltipleft] Inserts a tooltip at the left of an element, on hover with your custom text.

Popovers
Usage: This module outputs a title as default, if you don't require a title area, please put <none> as the title.
[popovertop title="Your popover title" contents="This is the content for the popover-top"]Trigger[/popovertop] Inserts a popover at the top of an element, on hover with your custom text.
[popoverright title="Your popover title" contents="This is the content for the popover-right"]Trigger[/popoverright] Inserts a popover to the right of an element, on hover with your custom text.
[popoverbottom title="Your popover title" contents="This is the content for the popover-bottom"]Trigger[/popoverbottom] Inserts a popover to the bottom of an element, on hover with your custom text.
[popoverleft title="Your popover title" contents="This is the content for the popover-left"]Trigger[/popoverleft] Inserts a popover to the left of an element, on hover with your custom text.


Wells
Usage:
[well]This is your normal well area, it has no settings[/well] Wraps content in a styled well.
[well_large]This is your large well area, it has no settings[/well_large] Wraps content in a large styled well.
[well_small]This is your small well area, it has no settings[/well_small] Wraps content in a small styled well.

Glyphicons
Usage: Please see http://getbootstrap.com/components/#glyphicons for a list of the icons available. To insert the icon of choice just enter the last part of the given name. ex. glyphicon-cloud = name="cloud" (simply omit the glyphicon-)
[glyphicons name ]See the icon?[/glyphicons] Prefixes content with an icon.

Jumbotron
Usage:
[jumbotron]This is your normal jumbotron area, it has no settings[/jumbotron] Wraps content in a styled jumbotron.

Blockquote
Usage: This is for a normal "left to right" styled blockquote
Source is for the citation, ie. the person/place you want to attribute the quote too.
[blockquote source="Frank 2014"]This is your normal blockquote area, it has no settings[/blockquote] Wraps content in a styled blockquote.

Blockquote Reverse
Usage: This is for "right to left" styled blockquotes
Source is for the citation, ie. the person/place you want to attribute the quote too.
[blockquote_reverse source="Frank 2014"]This is your normal blockquote area, it has no settings[/blockquote_reverse] Wraps content in a styled blockquote.

Abbreviation
Usage: Title holds the abbreviation FULL NAME that pops up on hover.
[abbreviation title="HyperText Markup Language"]HTML[/abbreviation] Wraps content as a styled abbreviation
Usage:
