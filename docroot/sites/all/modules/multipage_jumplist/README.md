## About

A block with direct links to Field Group's Multipage pages.

This module extends the functionality of [Field Group]'s Multipage format type
for entities by providing a block that allows you to access all multipages
directly from a list.

By default you only have Previous page and Next page buttons to navigate with,
but for large and complex forms this may be too restrictive, say when you need
to edit a minor detail on page 8 in a 12 page application form.

Having links to all pages in a multipage group provided as a block gives
flexibility to the site builder and themer on both navigation position and list
css.

[field group](https://www.drupal.org/project/field_group)


## Usage

1. Enable this module
2. Go to _Manage fields_ in an entity type (content types, user profile, et.al.)
3. Create a field group of type _multipage group_, and within it a few field
   groups of type _multipage_ and underneath them again, fields.
4. In the multipage group settings, enable the option _Create a jumplist block of
   this group_. This will create a navigation or jumplist block for this
   multipage field group instance and is now available on the block admin page.
5. Head over to the block admin page and place the newly created block to a
   region.
6. Create or edit content of this entity type and use the jumplist block to
   navigate directly to all pages within this multipage group.

Jumplist blocks have a few simple settings you might want to look at and the
list and list items can be theme overridden.
