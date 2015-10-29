

  ____   _         _____              _      _
 |  _ \ (_)       |  __ \            | |    | |
 | |_) | _   __ _ | |  | |  __ _   __| |  __| | _   _
 |  _ < | | / _` || |  | | / _` | / _` | / _` || | | |
 | |_) || || (_| || |__| || (_| || (_| || (_| || |_| |
 |____/ |_| \__, ||_____/  \__,_| \__,_| \__,_| \__, |
             __/ |                               __/ |
            |___/                               |___/


******************************************************************************************
/* Welcome dear Drupal themer in the beautiful world of BigDaddy!
******************************************************************************************

BigDaddy is not a ready-made theme but a good structured basis for theme developers.
It will facilitate your theme development and give you an optimal start with good rendered HTML and clean CSS.
Everything is well commented and structured to get first-time users up to speed quickly.
All that for a better control of your work environment and to increases your load performance.


------------------------------------------------------------------------------------------
/* MAIN FEATURES
------------------------------------------------------------------------------------------
- HTML5
- SASS
- RESPONSIVE
- SCALABLE, MAINTAINABLE AND FLEXIBLE CSS STRUCTURE
- HELPFUL THEME SETTINGS FOR FASTER THEME DEVELOPMENT
- COMMENTS, COMMENTS and MORE COMMENTS
- CDN JQUERY REPLACEMENT
- ... AND A LOT OF GOOD ADVICES HELPING YOU CREATING A FUTURE FRIENDLY DRUPAL THEME


------------------------------------------------------------------------------------------
/* INSTALLATION
------------------------------------------------------------------------------------------
- Download BigDaddy from http://drupal.org/project/bigdaddy
- Unpack the downloaded file and place the BigDaddy folder in your Drupal installation under one of the following locations:

  * sites/all/themes
  * sites/default/themes
  * sites/example.com/themes

- Log in as an administrator on your Drupal site and go to
  Administer > Appearance (admin/appearance) and make BigDaddy the default theme.

- if you want to change the name of the theme from 'bigdaddy' to another name like 'mytheme', follow these steps (to do BEFORE enabling the theme):

  -> rename the theme folder to 'mytheme'
  -> rename bigdaddy.info to mytheme.info
  -> Edit bigdaddy.info and change the name and the description
  -> In bigdaddy.info, template.php and theme-settings.php, change each iteration of 'bigdaddy' to 'mytheme'


------------------------------------------------------------------------------------------
/* TEMPLATES and .INFO file
------------------------------------------------------------------------------------------
- bigdaddy.info       => provide informations about the theme version, defaul regions, default stylesheets, default javascript, default settings
- template.php        => used to modify drupal's default behavior before outputting HTML (called preprocess functions -> https://drupal.org/node/223430)
- theme-settings.php  => used to create additional settings in the theme settings page (you don#t need to change this file)

- templates/html.tpl.php    => defines the code for the HTML structure (this include the HTML tag, HEAD tag and BODY tag)
- templates/page.tpl.php    => defines the code for the PAGE structure (this include all the element inside your BODY tag, in the most cases, all your Drupal REGIONS)
- templates/node.tpl.php    => defines the codes for NODES structure (this include all elements when creating a new content -> mostly all your node fields)
- templates/block.tpl.php   => defines the code when creatig a new BLOCK (in /admin/structure/block or through the VIEWS Module)


------------------------------------------------------------------------------------------
/* STYLESHEETS: Closer look into the /SASS folder
------------------------------------------------------------------------------------------
The CSS Structure is one of the more important part as front-end developer. By gaining experience, you will learn to organize your CSS files in a more precise and flexible way.
All that to always keep control over your website, regardless how big the project is.


I divided my structure into 3 main groups
-----------------------------------------

/* MASTER FILES
- config.scss   => includes only your global variables used for all the other SASS files
- main.scss     => import all the SASS files and will be used as your main CSS file (check the .info file)
- print.scss    => optimized stylesheet for the print version of your website

/* PARTIALS FILES
/* The partials directory is where you break things down into components. Up to you to add more files if needed.
/* Partials are meant to be imported, so they begin with an underscore (SASS naming convention).
- partials/_base.scss           => only pure HTML elements like body, h1-h6, p, ul, a
- partials/_elements.scss       => global/generic elements like navigation, logo or custom elements
- partials/_mixins.scss         => all your custom mixins
- partials/_layout.scss         => all your Drupal regions
- partials/_drupal.scss         => only Drupal specific elements like Views, Nodes, Fields
- partials/_forms.scss          => only form elements
- partials/_keyframes.scss      => all your CSS3 animation functions
- partials/_theme-settings.scss => used for styling BigDaddy's theme settings

/* VENDOR FILES
/* The vendor directory is for third-party CSS (CSS libraries developped by other front-end developers)
- vendor/_normalize.scss  => Learn everything about Normalize here: http://necolas.github.io/normalize.css/


HOW TO USE SASS IN BIGDADDY
---------------------------
1. Install SASSY (https://drupal.org/project/sassy) and PREPRO (https://drupal.org/project/prepro)
2. Download PHPSASS (https://github.com/richthegeek/phpsass) and copy it into /sites/all/libraries/
3. Change the configuration if needed in /admin/config/media/prepro
4. Let's SASS your files, you're ready to boost your workflow!!

=> To use FireSass, check "Include debug information in output" in the PREPRO module configuration.
   This will allow Firebug (when https://addons.mozilla.org/fr/firefox/addon/firesass-for-firebug/ is installed) to check into your SASS files instead of the compiled CSS files.


------------------------------------------------------------------------------------------
/* LEARNING SASS/COMPASS
------------------------------------------------------------------------------------------
- http://thesassway.com/
- http://sass-lang.com/
- http://compass-style.org/
- Video - http://vimeo.com/11671458
- Drupalize.me - http://drupalize.me/series/learning-sass-and-compass


------------------------------------------------------------------------------------------
/* JAVASCRIPTS: Closer look into the SCRIPTS folder
------------------------------------------------------------------------------------------
- scripts/master.js          => add here all your custom javascript code
- scripts/theme-settings.js  => file used for BigDaddy's theme settings
- scripts/libraries/         => add here all your third-party JS libraries


------------------------------------------------------------------------------------------
/* CHANGING THE LAYOUT
------------------------------------------------------------------------------------------
The layout used in BigDaddy is a very classic and standard HTML output. The purpose of this method is to have a minimal markup for an ideal display.

  1. HEADER
  2. CONTENT TOP
  3. CONTENT
  4. CONTENT BOTTOM
  5. SIDEBAR SECOND
  6. FOOTER

  => You're free to change order and add/remove regions. Don't forget to modify the .info file with the new changes.


------------------------------------------------------------------------------------------
/* UPDATES
------------------------------------------------------------------------------------------
Once you start using BigDaddy, you will massively change it until a point where it has nothing to do with BigDaddy anymore. Unlike ZEN, is not intended to be use as a base theme for a sub-theme (even though it is possible to do so). Because of this, it is not necessary to update your theme when a new version of BigDaddy comes out. Always see BigDaddy as a STARTER, and as soon as you start using it, it's not BigDaddy anymore, but your own custom theme.


------------------------------------------------------------------------------------------
/* BIG THANKS!
------------------------------------------------------------------------------------------
Thanks for using BigDaddy, and remember to use the issue queue in drupal.org for any question or bug report:
- http://drupal.org/project/issues/bigdaddy

And don't forget to SPREAD THE WORD if you like using BigDaddy!

Current maintainer:
* Maxime Rabot - http://drupal.org/user/429474 (maximer)

