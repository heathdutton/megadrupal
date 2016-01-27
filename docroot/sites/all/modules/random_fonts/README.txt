Random Fonts (random_fonts) module

SUMMARY

This module assigns random webfonts to elements of your site. Use it if you
want to spice up your site with some webfonts, but just cannot make a choice.

Just so we are clear - this module is supposed to be used in the development
stage, not on production sites. :)

Shipped with this module is another one, Ransom Note, which lets you create
mixed font displays looking like just that. It has its own README.txt.

PREREQUISITES

Random Fonts requires @font-your-face (http://drupal.org/project/fontyourface).
It also requires at least one of the following font provider submodules to be
enabled: Google Fonts API, Edge Fonts, or both. (These providers allow fonts to
be used without complicated installation procedures.)

HOW IT WORKS

This module randomly assigns random_font_0...random_font_N CSS class to each
<p> (paragraph), <ol> (ordered list), <ul> (unordered list) and <h1>...<h6>
(heading) element of a page; then, a random font is assigned to each of these
classes. Font name is added as a title attribute so that you can find it out by
hovering over the element.

Fonts are different on every page load; as you might expect, most random
combinations will look quite ugly, but once in a while you will find a rough
diamond that will give your site unique look.

CONFIGURATION

-- TARGET SELECTORS

Page elements that will get random font classes added; you can use HTML tag
names, CSS classes, IDs and such here; for advanced options, consult jQuery
documentation (http://api.jquery.com/category/selectors/).

-- NUMBER OF FONTS

By default, Random Fonts uses up to 10 different fonts at once; this number can
be changed. Keep in mind that a large number can negatively affect site
performance.

-- LIMIT FONT DISPLAY

Display of random fonts by default is limited to users with 'administer
@font-your-face' permission in order to minimize damage, should someone enable
the module on a live site without reading the documentation. This restriction
can be waived to display random fonts to everyone.

-- EXCLUDE ADMIN PATHS AND TOOLBAR

By default, random fonts are not displayed on paths starting with 'admin/' and
admin toolbar. Turning this option off will display fonts there, too.

KNOWN ISSUES

None at the moment.

MISCELLANEOUS INFORMATION

-- TITLE ATTRIBUTES ON LINKS

Random Fonts removes title attributes from links so that they would not come in
front of title attributes we set on ul/ol elements.

-- NON-LATIN CHARACTERS

If your site is in a language that uses characters outside Basic Latin
character set, keep in mind that not all fonts may contain needed characters.
For Edge Fonts, you may want to switch 'Subsets to use' option to 'All'.

-- OVERRIDING THEME CSS

To make sure random fonts take priority over theme CSS, we add '!important' to
our style definitions; because of the way @font-your-face wraps family names,
we need to slip it in together with CSS fallbacks. We use 'monospace' as the
default fallback as it has a surplus value of making it easier to spot missing
characters (see 'Non-latin characters' above).

(EVENTUAL) DEVELOPMENT PLAN

The following features could be considered if anyone needs them:
- an option not to remove title attributes from links;
- an option to use Google fonts of a specific subset (might want to wait for
Adobe to improve subset handling for Edge fonts);
- any reasonable feature request.
