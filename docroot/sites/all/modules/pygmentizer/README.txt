Provide an input filter which highlights inline codes by using <a href="http://pygments.org/">Pygments</a>.

Prerequisite
============
You need a working Pygments on your host.

sudo apt-get install python-setuptools
easy_install Pygments

See <a href="http://pygments.org/download/">how to install Pygments</a> for more details.

Installation
============
* Enable this module.
* Download PHPygments from https://github.com/capy/PHPygments/archive/1.0.1.zip and put it in sites/all/libraries. Ensure directory name keeps named "PHPygments"
* Go to admin/config/content/formats page, configure the text format in which you want to enable Pygmentizer and select the filter.
* At bottom of configuration page switch to settings tab of Pygmentizer.

Usage
=====
Just Put your code inside [LANGUAGE] [/LANGUAGE] token!
 eg:
 [javascript]
 /**
  * Drag and drop table rows with field manipulation.
  */
 Drupal.behaviors.tableDrag = {
   attach: function (context, settings) {
      //......
   }
 };
 [/javascript]

 [php]
    <?php
    function echobig($string, $bufferSize = 8192)
    {
        $splitString = str_split($string, $bufferSize);

        foreach($splitString as $chunk)
            echo $chunk;
    }
    ?>
 [/php]

WARNINGS
========
If the rendered code its broken, maybe the text format have "Limit allowed HTML tags", "Display any HTML as plain text" enabled? should not be.
If you have enabled "PHP evaluator" be sure this is below "Pygmentizer syntax highlighter" in "Filter processing order" if you dont want run the code instead highlight it!
You can use "Correct faulty and chopped off HTML", "Convert URLs into links" and "Convert line breaks into HTML (i.e. <br> and <p>)" nith no problem but be sute it is below "Pygmentizer syntax highlighter"