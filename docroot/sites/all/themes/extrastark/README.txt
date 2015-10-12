#READ ME FOR EXTRASTARK THEME

This theme is for Drupal v.7

##CONCEPT:
This theme is not a fully fledged Drupal theme, but more of a proof of concept. Quite so many designers new to Drupal complain about system CSS and how it makes styling much harder. My experience actually proved absolutely opposite: many of core styles are just helpers, so you don't have to start from scratch but rather reuse many existing styles.

The theme is created for those purists to demonstrate how easy it is to reset any core Drupal CSS style to zero. It resets margins, paddings, font properties etc., and it does the job much more radical way then original Stark theme.

The theme is not intended to be used on production sites (it's too bare for that), though you can use it as basis for more graphical theme.

##FEATURES:
The main attraction is reset.css, which by popular demand would include slightly remixed HTML5Boilerplate CSS reset as well. But there is more: 
- form elements reset (so you'll miss nice enough default D7 buttons), it supplies only borders to make them visible.
- original Stark layout (layout.css) has no margins.
- responsive layout (template.php now adds <meta name="viewport" content="width=device-width, initial-scale=1.0"> for CSS to work properly on mobile devices).
- the theme does reset front end styles, but not the backend.
- one anti-reset: font is set to sans-serif (in style.css, just hate small serifs).

##VALIDATION:
- W3C CSS Valid (CSS level 3)
- XHTML + RDFa Valid

##BROWSER COMPATIBILITY:
IE (6,7,8), WebKit (Safari, Chrome), Mozilla (FireFox, Camino, SeaMonkey), Opera (8,9,10).

##DISCLAIMER:
As usual, the theme was tested with Drupal 7 core modules only. If you have contrib modules installed then you might need more resets. Please DO submit an issue to module developers if you see some excessive or unneeded CSS.

##MORE TO READ:
A nice and clear write up on Drupal CSS concepts by Jacine Luisi http://jacine.net/post/978688556/drupal-css

Enjoy the reset madness!

Created by Andrey from  http://nood.org/drupal-themes