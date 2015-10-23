This module will allow Country -> Province exposed filters to work with AJAX.
In case you have installed also Location Region module, the ajax will work
for the Country -> Region -> Province fields.

Example for France:
Country = France
Region = Ile de France
Province = Paris(75)

In order to make this module work, you will have to patch the views mdoule
to allow #ajax on exposed forms.

You can find the patch here:
https://www.drupal.org/node/1183418#comment-8476481