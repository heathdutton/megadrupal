PHP Execute Extended Tools is meant to be an extension to the Devel PHP 
Execute page (/devel/php)

PHP X Tools adds a history table to the devel execute page as well as a 
'Saved Script' section.

Optionally it integrates with the 'CodeMirror' library 
(http://codemirror.net/) transforming the plain textarea used on the 
devel/php page into a lightweight code editor supporting syntax 
highlighting, code formatting, code folding, matching brackets and some more.

The 'HISTORY' feature will simply log all snippets of code you execute using 
the  /devel/php page so you can reuse or review them at any point in the 
future.
I often used the PHP Execute feature to quick test some code and it happened
often that after an error the page reloaded but the code I ran was gone so
I created the history feature.

The 'SAVED SCRIPT' section gives you the ability to give a snippet that you 
write on the devel/php page a name and save it. It will then appear in the 
saved scripts table under the name you gave it, enabling you to load and 
rerun it at any time.
I am often using the 'PHP Execute' page to write some quick test for code I 
am writing and I found myself saving the code snippets in notepad so I could 
run them in a later point in my development process. So I just added the 
ability to save the script and have it accessible at any time directly from 
the 'PHP Execute' page.
It also comes handy if you want to save some code snippet you need to use 
just once for an import for example or just to set some variables or set up 
a feature for the first time in your deployment process.

Optionally the text area used on the PHP Execute page can be turned into a 
lightweight code editor supporting syntax highlighting, code formatting, 
code folding, matching brackets, full screen, search and more.
You will need to download the 3rd party library manually, see installation 
instructions below.

INSTALLATION
------------
You need to have the devel module installed and enabled.
Then just install 'PHP Execute Extended Tools' as any other drupal module 
and enable it. 

Optional (but recommended):
Install the Codemirror library:
1. Download the library from: http://codemirror.net/codemirror-2.38.zip
    NOTE: Only works with 2.x version of the Codemirror library. 3.x is not 
supported for now.
2. Extract the library so that it's contents are accessible under 
'sites\all\libraries\codemirror' (note: folder name is without the version 
number)
    To test if the folder structure check that the folder: 
sites\all\libraries\codemirror\lib exists and contains the codemirror js 
and css file as well as the util folder.

That's it, now go to /devel/php and play with your new developer toys.

USAGE
-----
After following the Installation instruction above go to the page 'devel/php'.
You should now have two new expandable fieldsets ('Saved Script' and 
'History') above the Execute PHP textarea. 
Clicking on any of those will expand and show you a table with saved script 
or the history of executed scripts.

To actually save a user script you will notice there are now two additional 
buttons near the 'Execute' button, below the PHP execute textarea and a new 
textfeld called 'Script Name' above the textarea.
Write your code into he textarea, give the script a name by filling in the 
'Script Name' field and hit 'Save' or 'Save and Execute' to save or save and 
execute the code.

Also, if you installed the Codemirror library you should notice that the 
textarea has turned into a lightweight code editor giving you text 
highlighting, code formatting and bracket matching.
You can also use F11 to go fullscreen or you can CTRL+F to search the code 
or CTRL+SHIFT+F to search and replace.

AUTHOR
------
Cristian Fleischer
cristian.fleischer@gmail.com
