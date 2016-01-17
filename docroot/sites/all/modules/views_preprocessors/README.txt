
Adds support for preprocessors for each defined view.
A module with the name myModule can implement a preprocessor for a view
called 'myView' by implementing the following preprocessor
(note the double underscore):

myModule_preprocess_views_view__myView(&$vars) { ... }
