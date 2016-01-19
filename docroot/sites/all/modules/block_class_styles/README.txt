SUMMARY: Extends the block_class module to incorporate styles rather than css
classes. Adds style-based tpl suggestions. Allows HTML in your block titles.


OVERVIEW:
Styles are:
* one or more classes with a given Human Name
* defined by admins
* used by content managers
* allow separation of css names from semantic or logical style names
* generate style-based block template suggestions for themers
* assigned to blocks either singularly or in multiple (based on settings)

When you assign a style to a block...
* it receives the classes of that style
* the theme engine checks for a block template for that style

Additional features...
* Allows the ability to set a global text format for custom block titles; this
  allows you to now have HTML in your custom block titles. Yes, you can now have
  block titles that contain HTML markup!


REQUIREMENTS: Requires the block_class module


INSTALLATION:
* Download and unzip this module into your modules directory.
* Goto Administer > Site Building > Modules and enable this module.


API:
* block_class_styles_info()
* block_class_styles_info_alter()


THEME SUPPORT:
You may add styles to your theme.info file if you wish; you would
add something like the following. You must clear caches after editing your
theme's .info file for the changes to take effect!

block_class_styles[legaleeze] = Legal/Disclaimer
block_class_styles[sidebar-callout] = Sidebar Callout
block_class_styles[center] = Centered
block_class_styles[aside] = Right Sidebar

--------------------------------------------------------
CONTACT:
In the Loft Studios
Aaron Klump - Web Developer
PO Box 29294 Bellingham, WA 98228-1294
aim: theloft101
skype: intheloftstudios

http://www.InTheLoftStudios.com
