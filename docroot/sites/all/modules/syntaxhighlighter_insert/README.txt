$Id:

Description
===========
This module allows you to insert syntaxhighlighter tags into a textarea. It supports both plain text and wysiwyg textareas.

This module contains three modules:

- Syntaxhighlighter Insert: Provides general syntaxhighlighter insert functionality (e.g. permissions).
- Syntaxhighlighter Insert Text: Add a fieldset under the selected textarea (you can choose this when editing the field).
- Syntaxhighlighter Insert WYSIWYG: Adds an extra button to wysiwyg editors and opens a popup to select the syntaxhighlighter tag options to insert.

Dependencies
============
- Syntaxhighlighter

Usage
=====

Syntaxhighlighter Insert Text
-----------------------------
1) Go to the content types page (admin/structure/types) or ANY other entity edit page where you can manage its fields.
2) Click on 'manage fields' for the content type/entity that has the field for which you want to enable syntaxhighlighter insert.
3) Click on 'edit' next to the field for which you want to enable syntaxhighlighter insert.
4) In the 'Syntaxhighlighter insert' fieldset, check the checkbox ('Use Syntaxhighlighter Insert for this field').
5) When adding the node/entity there should be a link below the field that says 'Insert Syntaxhighlighter tag'.
6) When you click it a form pops up with all the options you need.
7) When you've configured all the options press 'Insert' at the bottom.
8) Paste your code within the pre tags that have been inserted into the textfield.
9) Save the node/entity and you're code should be highlighted.

Syntaxhighlighter Insert WYSIWYG
--------------------------------
1) Go to the WYSIWYG Profiles page (admin/config/content/wysiwyg/profile)
2) Press 'edit' next to the profile for which you have enabled the WYSIWYG editor.
3) In the 'Buttons and Plugins' fieldset, check the 'Insert syntaxhighlighter tag' checkbox.
4) The new button should now appear in the editor.
5) When you click it a form pops up with all the options you need.
6) When you've configured all the options press 'Insert' at the bottom.
7) Write your code within the black box in the textfield. Use SHIFT + ENTER to go to a new line or you will go out of the box.
8) Save the node/entity and you're code should be highlighted.
