Textcomplete
------------------------------------------------------------------------------
Provides the user with text autocomplete functionality within textareas. By
default it comes shipped with a basic html autocomplete and emoji (emoticons)
autocomplete.

However the true power of textcomplete is when you install entityreference and
advanced entity tokens. It will allow you to perform autocomplete searches on
entities within your Drupal install.

For example if you want to link to users within comments, you can define a view
or basic search which will then allow you to perform an autocomplete search
within a textarea. When a user is selected it will use advanced entity tokens
and replace it with a link to the user profile.


Dependencies
------------------------------------------------------------------------------
This module is dependent on Jquery Update and Libraries. You must have at least
jQuery version 1.7 installed.

It also requires a 3rd party javascript plugin which can be found here:
https://github.com/yuku-t/jquery-textcomplete


Installation
------------------------------------------------------------------------------
1. Make sure you have installed textcomplete, jquery_update libraries.
2. Download the textcomplete plugin from the following location
   https://github.com/yuku-t/jquery-textcomplete
3. Extract the plugin to your libraries directory and rename the folder
   textcomplete.
4. Make sure you are using jquery version 1.7.
5. Navigate to manage fields inside a content type.
6. Create or edit a new or existing textfield, change the settings inside the
   fieldset textcomplete to suit your needs.
7. To replace the emoji tokens you will need to enable the filter inside
   admin/config/content/formats

If you wish to use this as an autocomplete with entityreference please follow
these instructions.

1. Enable entityreference, aet and token_filter
2. Navigate to admin/config/content/formats and enable the replace tokens
   filter.
3. Navigate to manage fields inside a content type.
4. Create or edit a new or existing textfield and enable the entityreferece
   status inside the textcomplete settings fieldset.

Issues
------------------------------------------------------------------------------
Currently their is an issue with spaces in the regular expression for matching
the word you are currently on. At the moment you can use underscores instead
of spaces.

Coming features
------------------------------------------------------------------------------
* Fix regex expression to accommodate spaces.
* Use content editables instead of textareas so the user does not see the
  raw html code.
