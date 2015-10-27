================================
ABOUT
================================

Creating download request links, is a tedious job.
This module will help you do this task the best way.

It provides a text filter which replaces [ngsl=private.zip] with a correct download request page for
the private.zip file. The output will look like this:
http://www.yourdomain.com/requestlink/private.zip.html

You can put [ngsl=folder/file.zip] in your articles and get download links.

For a step-by-step guide of this, please read on.


================================
STEP-BY-STEP GUIDE
================================

1 - Go to admin/modules.

2 - Enable the "Nginx SecureLink: Text format" module.

3 - Go to Configuration => Content Auhoring => Text Formats
  (admin/config/content/formats)

4 - Create a new text format or edit one of your existing text formats.
  For example I'm going to edit the "Filtered HTML" text format.
  So I go to admin/config/content/formats/filtered_html

5 - Enable (Tick) the "Nginx securelink filter" option

6 - (Optional) Re-order text filters if needed.

7 - Click "Save configuration"

Now It's time to test and see the result.

8 - Go to Content => Add content => Article
  (node/add/article)

9 - Fill the form. And place something like this in your articles body: "[ngsl=file.txt]"

10 - Save the article and you should get something like this:
  http://www.yourdomain.com/requestlink/hello.txt.html


================================
IMPORTANT NOTES
================================

1 - You can't use spaces in your file names. only alpha-numeric and the following chars are allowed:
  -,@=+#./

  For example the following tokens are incorrect:

  [ngsl=folder/long file name.txt]
  [ngsl=folder / file.txt]
  [ngsl=folder/file.txt ]

  But the following tokens are correct:

  [ngsl=folder/long-file-name.txt]
  [ngsl=folder/file.txt]
  [ngsl=folder/file=name.txt]

  The last one shows you that you can use these characters "-,@=+#." (without quotes) in your file name.

  NOTE: Using some of the above chars in a file name is not sane. but we accept it however.

2 - You can add one space after and before the = char.
  For example the following tokens are correct:

  [ngsl=folder/file.txt]
  [ngsl =folder/file.txt]
  [ngsl= folder/file.txt]
  [ngsl = folder/file.txt]

  but the following tokens are incorrect:

  [ngsl =  folder/file.txt]

3 - You can use this token in your links.
  A good example is this:

  <a href="[ngsl=videos/video.mp4]">Download the video</a>


================================
AUTHOR
================================

This module is created and maintained by Ahmad Hejazee
http://www.hejazee.ir/
