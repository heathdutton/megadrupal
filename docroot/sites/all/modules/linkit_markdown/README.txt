- Linkit Markdown:
Provides Linkit support for Markdown editor. The Makdown
editor is a plug-in for BUEditor.

- WHAT'S NEW:
7.x-1.x:
 - Integrates Linkit with the Markdown Editor, allowing site
   builders to add a Linkit Button to their markdown editor.

- HOW TO INSTALL:
1) Install the prerequisite modules:
 - Markdown Editor
   - BUEditor
   - Markdown Filter
      - Libraries API
 - BUEditor Plus - recommended, but not required.
 - Linkit
 - WYSIWYG
2) Copy the linkit_markdown directory to your modules directory.
3) Enable the linkit_markdown module on the module administration page.

- HOW TO USE:
1) Create a Linkit profile for editors: /admin/config/content/linkit
2) Configure BUEditor to make the Markdown Editor profile the
   default profile for the Markdown input filter:
   /admin/config/content/bueditor
3) Edit the Markdown Editor to include the Linkit button:
 - Give the new button a label
 - Enter js: markdownEditor.linkit(); in the content textbox
 - Select the button icon.
 - Select the button key.
