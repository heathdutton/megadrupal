# Insert Field

Structured Content in an Unstructured World.

## Introduction
Insert Field provides a method of inserting fields into any text field on the
current entity. It does so by using HTML Comments
&lt;!-- field_name[delta] --&gt;
that can easily be ignored if Insert Field is disabled.

Insert Field allows the administrator to assign which fields can be inserted
and which text fields they can be inserted into. The editor is provided with
tabs (closed by default) where the specific field can be selected. An "Insert"
button is provided for editors to be able to easily insert the HTML comment at
the current cursor. If the editor modifies the order of the fields (thereby
changing the "delta"), Insert Field will accommodate for this and alter the
delta in the text field. The admin also has the ability to dynamically control
the display of the inserted field on a per display mode basis. The field may
not even be inserted at all in some display modes (such as "Teaser"), in which
case processing will be skipped almost entirely and the HTML comment will be
printed rather than the field itself.

## Installation
1. Install as you would normally install a contributed Drupal module.
   See: https://drupal.org/documentation/install/modules-themes/modules-7
for further information.
2. Enable `Insert Field UI` which provides the UI documented.
4. Clear Drupal's Cache.
   See: https://www.drupal.org/documentation/clearing-rebuilding-cache


## Requirements
Insert Field requires the following modules:
* [Text](https://www.drupal.org/documentation/modules/text)

Insert Field UI requires the following modules & libraries:
* [Insert Field](https://www.drupal.org/project/insert_field)
* [jQuery Caret Plugin](https://github.com/acdvorak/jquery.caret/)


## Setup
1. Create a Content Type (or use an existing one).
   See: https://www.drupal.org/node/774728
2. Add a Text Field (or use an existing one).
   See: https://www.drupal.org/node/774742
3. Create a field you'd like to be inserted into the text field (or use an
   existing one).
   See: https://www.drupal.org/node/774742
4. From the `Manage Fields` page, move the field that will be inserted beneath
   the text field and slide it to the right. This should look like a Drupal
   hierarchy (Menus, Taxonomy Terms, etc.).
5. From the `Manage Display` go to the view mode (default, teaser, etc.) that
   should have the field inserted. Perform the same action from step 4 on this
   display.
6. Create a new peice of content (or use an existing one) of the same type in
   step 1.
   See: https://www.drupal.org/node/120635
7. There should now be a tab to the top right of your text field, click the tab
   and the insert field will be revealed.
8. Set a value for the field.
9. Click somewhere inside of your text field and click `Insert`. This will
   insert an HTML comment like &lt;!-- field_name[0] --&gt; into your text
   field.
10. Click `Save` at the bottom of the form.

## Maintainers
Current maintainers:
* David Barratt ([davidwbarratt](https://www.drupal.org/u/davidwbarratt))
