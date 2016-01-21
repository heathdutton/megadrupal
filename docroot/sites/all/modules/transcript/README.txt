This module provides a way to integrate YouTube's Video Transcript 
with our site. Using this we can easily show the transcript in our page 
wherever you want to display. Once the module is installed and the Transcript 
block enabled then based on the video's voice the transcript will get
hightligting automatically.

HOW TO USE
======================
1. Install the Transcript module
2. In block list, enable the Transcript block in any pages
3. Thats it. You can see the transcript integrated page.
4. For additional settings, check this
admin/config/transcript

CONFIGURATION IN YOUTUBE SITE
======================
1. First check, whether you can access the transcripts using the Url 
http://www.youtube.com/api/timedtext?v=<video-id>&lang=<lang-code>
Ex: http://www.youtube.com/api/timedtext?v=kswE_n0FNXk&lang=en

2. It will return xml formatted transcript. Thats it. 

* If it doesn't return the xml formatted transcript or if you get blank page, 
please add the below changes in your Youtube video manager    
1. Upload/Add the transcript file (or text) by manually. Then click publish
2. Note: If the transcript generated automatically by Youtube, we can't fetech. 
So Please remove/delete it and then upload the transcript file by manually.

More About Transcript(How to add):
https://support.google.com/youtube/answer/2734799?hl=en

REQUIREMENTS
============
The Drupal 7 version of Transcript needed JQuery 1.5 or later version. 
Its better to use the JQuery Update module.

AUTHOR/MAINTAINER
======================
Manikandan M (manimjs_drupal) - https://drupal.org/user/2776421/
