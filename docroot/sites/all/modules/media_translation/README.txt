CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration
  * Known issues


INTRODUCTION
------------
The Translatable Media Files module allows you to specify a language for the 
files you upload through the media module, then it allows you to bundle files 
together in translation sets.

Translations sets can later be used by the additional module Translatable Media 
Files Display Override to detect if a referenced file belongs to a translation
set and replace it with the appropriate file for the language being displayed.


INSTALLATION
------------
1. Enable the module "Translatable Media Files" (under the "Media" section)
2. Go to Configuration -> Media -> File Types
3. For each media type to support translations: (i.e. images)
   a. Click on "manage fields" next to the media type
   b. Click on the "Translate" tab
   c. Set the Translation mode to "Translate" and click Save
4. To verify that everything worked:
   a. Go back to Content -> Media and add a file of the type(s) you
      enabled for translation in step 3.
   b. Then find it in the list and click the edit link next to it.
      If you have set up internationalization (I18n) correctly and have multiple
      languages you will notice there is a new Language field below the file.
      It will be set to Language neutral by default. You will need to change
      this to one of the available languages in order to enable the Translate 
      tab. Set it to English for example.

      After setting a language click the edit link again. Notice there is a 
      Translate tab now.

      The translate tab allows you to bundle this file with another file in a
      different language and create a Translation Set.
   c. Now go back to content -> media and upload the matching file for your 
      other language repeating steps 4.a through 4.b.
   d. Now that you have two files one flagged as English and one flagged as your
      other language, edit either one of them and then click the Translate Tab.
   e. You will see a simple form with two auto-complete reference fields. Your
      file will be pre populated according to the language you selected for it.

      Go ahead and type the name of the second file you uploaded in the 
      alternative language and then hit the Save button.


CONFIGURATION
-------------

1. Upload files to the media library
2. Edit the file and select the language for that file

   To associate files in different languages so that a user can see/select 
   the file in different languages:

1. Make sure that all of the language specific files have been uploaded and 
   their language set accordingly (see How to use)
2. Edit one of the files that you want to group together
3. Click on the "Translate" tab
4. In the field for each language the file is available in, start typing the
   filename of the file and select it from the list.
5. Click Save


KNOWN ISSUES
------------

The file selection list will only show files in the language associated with the
field (i.e. if you're in the Spanish field you'll only see files flagged as 
being in Spanish) and that have not already been grouped with another file.  

Each file can only be in one group (Translation Set).

