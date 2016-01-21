----------------
Introduction

This project allows to change theme when a mobile device is detected.
I also does redirect from default domain to mobile domain when appropriate.

Remember mobile_tools project? It allowed to switch theme for mobile devices
and more.
The problem was that it's no more well maintained.

I created my own solution for changing theme for mobile devices which
is lightweight, works well and does exactly what is needed.

----------------
Functionality

- redirect mobile users to mobile domain (like m.example.com)
- redirect desktop users to regular domain if they used mobile domain
- switch theme if domain is mobile domain
- works well with drupal page caching
- detects phones and tablets separately
- for phones, uses mobile url; for tablets it adds 'tablet' css class to <body>,
  also when page caching is turned on
- provides mobile to desktop switch link for users who want to view 'full site'
  while using phones - use print ai_mobile_switch_link();
  to show it in your theme
- users choice is remembered
- device detection is based on great Mobile Detect PHP library
- contains nice admin config form

----------------
Installation instructions

1. Download and extract Mobile Detect library, into sites/all/libraries so 
   that there is a file sites/all/libraries/mobile_detect/Mobile_Detect.php
2. Enable the module as normally
3. Go to admin/config/system/ai-mobile and configure it
4. Make sure that both regular and mobile domains are pointing to the same
   Drupal site within you server.
