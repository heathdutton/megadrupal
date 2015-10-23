     __  _____________   _______   __________  ____  ______
    /  |/  / ____/ __ \ /  _/   | / ____/ __ \/ __ \/ ____/
   / /|_/ / __/ / / / / / // /| |/ /   / / / / /_/ / __/
  / /  / / /___/ /_/ /_/ // ___ / /___/ /_/ / _, _/ /___
 /_/  /_/_____/_____//___/_/  |_\____/\____/_/ |_/_____/
    MediaCore Drupal Filter & WYSIWYG Editor Button

Two aspects of this module work together to give you a seamless MediaCore
experience from within Drupal. WYSIWYG integration (supports TinyMCE, and
CKEditor) gives you a new button while editing content; click it and your
videos are right there! Just select which one you'd like to embed, and we'll
insert a shortcode for it in your content. The content filter turns these
shortcodes into the appropriate code so that your video appears right within
your page.

===Shortcode example:===
[mediacore:http://demo.mediacore.tv/media/trap-jaw-ants]


==INSTALLATION==

1) Add the 'mediacorechooser' folder to your modules directory.

2) Log into your Drupal site, and navigate to the Modules section to enable the
   “MediaCore Chooser”.

3) Next, navigate to the Modules tab and click “configure” next to the Wysiwyg
   module. Specify an editor for whichever input formats you would like the
   MediaCore chooser available (either TinyMCE or CKeditor). Click edit next to
   the chosen editor. Under “Buttons and Plugins,” check “MediaCore Chooser.”

4) Next, click on the Configuration tab and select Text Formats.  Click on “Add
   text format”, enable the MediaCore filter and click “Save configuration”. Be
   sure that it is placed above the “Filter HTML” filter, or it won’t work.

5) Finally, click on Configuration, select “MediaCore Chooser” and fill in your
   MediaCore site URL(e.g. change it from demo.mediacore.tv to
   yoursite.mediacore.tv).

6) If your MediaCore setup requires URL signing then contact customer support
   at support@mediacore.com to request a Secret Key and Key ID.

==ABOUT==

MediaCore (http://mediacore.com/) is an online video platform for managing,
encoding, monetizing and delivering video to mobile and desktop devices.
MediaCore makes it easy for any organization to share video either publicly or
privately and build an amazing user experience on both desktop and mobile
browsers around their own content.

Who's using Mediacore? More and more MediaCore powered sites are popping up all
over the world. You can learn more about some of these sites here on our
MediaCore showcase: http://mediacore.com/why-mediacore.
