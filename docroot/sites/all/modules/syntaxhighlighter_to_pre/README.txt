------------------------------------------------------------------------------
                   {syntaxhighlighter} to PRE Converter
------------------------------------------------------------------------------

This is the utility module for Drupal 7 that allows users to convert
old {syntaxhighlighter} tags to <pre> in nodes.

OVERVIEW

The Syntax highlighter [1] module highlights program code in nodes.
We need to add special tags in the content to make it possible.

Drupal 6 version of Syntax highlighter [1] works with special {syntaxhighlighter} tag:

{syntaxhighlighter SYNTAXHIGHLIGHTER-OPTIONS}
  program code
  ...
{/syntaxhighlighter}

Drupal 7 version of Syntax highlighter [1] only supports native HTML tag <pre>:

<pre class="SYNTAXHIGHLIGHTER-OPTIONS">
  program code
  ...
</pre>

It is significant part of D6 -> D7 upgrading process to convert {syntaxhighlighter}
tags to <pre> in the site content.

Note: The module uses batch API for batch processing.


TESTING
------------------------------------------------------------------------------
The module contains automated tests. Try them to make sure it works.


USAGE
------------------------------------------------------------------------------
1. Backup your database!
2. Install module.
3. Set 'Convert old syntaxhighlighter tag to PRE' permission for your user role.
4. Go to the http://your_site/syntaxhighlighter_to_pre page.
5. Press the Convert button and wait. Waiting time can depend on quantity of nodes on your site.
But you will see current progress. Thanks for it to Drupal Batch API.
6. Check node content.
7. Profit.


DEVELOPER
------------------------------------------------------------------------------
Konstantin Komelin [2]


LINKS
------------------------------------------------------------------------------
[1] http://drupal.org/project/syntaxhighlighter
[2] http://drupal.org/user/1195752