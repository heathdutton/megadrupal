### Introduction ###

This module alters the output of the [Insert](http://drupal.org/project/insert)
module to use [Markdown](http://drupal.org/project/markdown) syntax for
`image_image` and `file_generic` widget types.

For `image_image` widget types, it also adds an optional link field to the
widget, so you can have images link to other content via the UI.

### Requirements ###

- [Markdown](https://drupal.org/project/markdown),
- [Insert](https://drupa.org/project/insert),
- Either the core file or image module enabled,
- A textarea with the markdown formating option enabled.

### Installation ###

- [Enable](https://drupal.org/documentation/install/modules-themes/modules-7)
  the module and selected dependancies as above,
- Create a file or image field on one of your entities,
- In the configuration page for the field, enable both the "Insert" and
  "Markdown Insert" checkboxes under the "Insert" drop down settings,
  and optionally the "Title" and "Alt" fields for images,
- Create or edit an instance of this entity, and upload a file or image,
- Enter the description, title, alt, and link fields as necessary,
- Choose the markdown-text-format-enabled area you would like the text to
  be inserted to and click on "Insert"

### Configuration ###

#### Images: Without a link ####

Inserting an image with a "Title" and "Alt" field and clicking "Insert" will
result in:

`![Alt Text](/path/to/file "Optional Title"`

#### Images: With a link ####

Inserting the same as above but with `/path/to/file` in the link field will
result in:

`[![Alt Text](/path/to/file "Optional Title")](/path/to/file "Optional Title")`

### Files ###

If the "Description" box is selected in the widget settings and is given a
value, it will be the text for the link.

For example, `[Description](/path/to/file)`.
