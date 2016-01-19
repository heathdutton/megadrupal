####################
# Mr. Asghar Khan  #
####################
- First thanks to use this module.  If you want to customize this module then 
you can hire me.

- Email mr.asghar.khan@gmail.com
- Skype asghar.khan5

##################
# Installation   # 
##################
Step 1:  Download the flowplayer api(https://flowplayer.org/latest/) 
Step 2: Unzip the flowplayer api and rename it to flowplayer5 e.g
 - flowplayer5/skin
 - flowplayer5/LICENSE.md
 - embed.mn.js
 .
 ..
 ...

Step 3: Move flowplayer5 directory in libraries directory e.g
sites/all/libraries/flowplayer5
Step 4: Install the flowplayer5 module.
Step 5: Go to configurations page and customize player UI. 


####################
#  For custom Use  #
####################
If you want to use player for custom development the follow these steps.
Step 1 - Make sure your module is already installed.
Step 2 - If you want to add play into a div then call this function
<?php 
  $config = array(
    'playlist' => array(
      'webm' => 'http://stream.flowplayer.org/bauhaus.webm',
      'mp4' => 'http://stream.flowplayer.org/bauhaus.mp4',
      'ogg' => 'http://stream.flowplayer.org/bauhaus.ogv'
    ),
    'ratio' => '3/4',
  );
$css_selector = ".class or #id";
flowplayer5_add($css_selector, $config);


#########################
# Use with Video Module #
#########################
If you want to use this module with video module then follow these steps.
1- Make sure flowplayer5 module is installed.
2- Go to video player section and select *HTML5 Player*. Then radio options 
select Flowplay5 option.
3- Enable flowplayer5 player for the desire extensions 


