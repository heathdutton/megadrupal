
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Current Maintainer: Bastlynn <bastlynn@gmail.com>

Character sheet provides moderation features to allow for character sheet
moderation in an online RP environment. This system is intended to 
facilitate old fashioned tabletop gaming in an online environment - it is
not intended to operate as a back end for a flash game or other computer
regulated system. (More power to you if you use it as such.)

Users will login to this system and create character sheets. Those sheets
will be submitted for moderator approval, and once approved released on the
site marked as such.

This system also provides an API to allow other modules to create and 
manage additional fields on the character sheet. This API is intended to
serve the purpose of whatever automation or computerized regulation is 
required for a character sheet.

Currently only one example RPG system has been built against this API. This
is character_sheet_basic. Basic is intended as an example system for your own
development work.

I suggest you use field_collection for sets of information, such as skills.
For example:
Skill name / Skill value or Skill name / Skill description / Skill value

Since this system is driven by fields, you will be able to use Views,
Rules, and other modules that are well integrated with fields. Please do so, 
they will allow you to slice into your data in much more refined ways than 
this module currently allows.

I also suggest you use features to tie your implemented system together. If
you convert a commonly used system, contribute it back please! :)

Character sheets may be themed! If you want your sheets to look like the
sheet in your chosen RPG book - do so! You will want to create a template
called node-[type].tpl.php where type is the content type you have made 
into a Character sheet type. Beyond that, theming the fields of a Character 
sheet is just like theming the fields of any other CCK type. More
information can be found at http://drupal.org/node/17565.


INSTALLATION
------------

1. Copy this character_sheet/ directory to your sites/SITENAME/modules directory. 

2. Enable the module and any system modules you wish, and configure your character
   sheet systems at admin/config/system/character_sheet.

3. Then configure individual content types to enable the character sheet per type.