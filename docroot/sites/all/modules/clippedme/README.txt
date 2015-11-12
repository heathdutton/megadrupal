Clipped.me (clippedme) module

SUMMARY

Clipped (http://clipped.me) is a service that provides bullet point summaries
of documents and articles. This module uses it to provide a content preview of
your links on hover.

PREREQUISITES

Clipped.me depends on Libraries module.

To display bullet point summaries, Clipped.me uses Poshy Tip jQuery library
(http://vadikom.com/tools/poshy-tip-jquery-plugin-for-stylish-tooltips/).
Download it from http://vadikom.com/files/?file=poshytip/poshytip-1.1.zip and
place in sites/all/libraries/poshy_tip.

HOW TO USE IT

Bullet point summaries will be provided to links that have target CSS class
(default - '.clippedme') on <a> element.

Clipped works best on news articles, can also be used on blog posts and
commentary. On pages containing lists and tables summaries often make little
sense.

CONFIGURATION

You can configure the target CSS class for links as well as select a theme from
one shipped with this module and three Poshy Tip themes that more or less
decently work with this.

If you choose to use no theme and write CSS yourself, then it is up to you to
add it, too.

KNOWN ISSUES

Cross-browser compatibility has been little tested.
