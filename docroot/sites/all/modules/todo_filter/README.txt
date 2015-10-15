 Todo Filter
====================================
This module provides input filter to display check-boxes. You can add Todo
checkboxes when editing node content. When viewing node content, you can mark
each todo task as "done" by clicking on the checkbox to toggle it.

: Concept
Sometimes you need to have a todo list in a node (like a checklist, or a simple
task list) without having to use a task management system. This module provides
a new input filter - that changes specific syntax into a checkbox with a toggle
feature so you can click on the checkbox to mark a task that's done.

This module uses AJAX to dynamically save the Todo task state as seen on the
node view. User permissions are checked and only those with node edit access
can permanently set the toggle state from the node view.

Light weight, no database to be fat!
It uses the existing node content and works with other input filters.
Just select the right input format, that's it!

: Syntax
Display an empty checkbox
    [_] Todo task

Display a checked checkbox
    [/] Todo task

: Setup
1. Enable the "Todo Filter" module under the "Input Filters" group
2. Select an existing Input Format in "Configuration -> Input formats" or
   create a new input format
3. Enable "Todo Filter" under the "Filters" group

: Using
1. Select the input format that includes the "Todo Filter"
2. In a text field use the [_] and [/] to enable a Todo checkbox for that line
3. Click "save" - you will get the checkbox and can click on it to change a
   task status

: Technical notes
This module also directly alters the content of each text area that has the
  "Todo filter" input filter enabled whenever a Todo checkbox is toggled.
This module also performs a node_save(); whenever a Todo checkbox is toggled.
There may be a config option to disable this action someday.

