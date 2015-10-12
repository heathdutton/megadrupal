
LABEL TRASH QUICK INSTALL

Label Trash comes pre-installed with a patched version of the floatlabel.js
library. The patched version is the default choice. It was designed to run out
of the box as a backport to D7's fairly old version of jQuery.
All you have to do is visit /admin/config/content/label_trash to assign
floatlabels to the input fields that need them. No need to download anything.
Because of the way the required Libraries module works, Label Trash will
auto-create an empty directory sites/all/libraries/floatlabels. Please don't
remove it while you use this module, even when left empty.

If you wish to use the original floatlabel.js, you'll need to first upgrade to 
jQuery 1.8 or later, using the jQuery Update module, https://drupal.org/project/jquery_update.
Then hit the download button here: http://clubdesign.github.io/floatlabels.js
Uncompress the downloaded package into sites/all/libraries, changing the name
of the directory to floatlabels, if necessary. After this you should have this
file: sites/all/libraries/floatlabels/floatlabels.min.js

Visit admin/reports/status and verify that the Label Trash section does not
report any warnings.

SUPPORTED HTML ELEMENTS
Standard text fields are targeted with this CSS selector: input:text
Password fields: input:password
Multi-select boxes: select[multiple="multiple"]
Multi-line text fields: textarea
Edit tab body content area specifically: #edit-body textarea
Comment area specifically: #edit-comment-body textarea

For CKEditor on the content Edit tab, use the Label Trash UI to also add this
label positioning CSS: margin-top:79px !important;

BROWSER SUPPORT
Works on all modern browsers. Old versions of Internet Explorer? Not sure...

THEME SUPPORT
Not all themes are the same. Depending on the theme you're using, some
corrective CSS may be required to place the floatlabels in exactly the right
position. An example can be found in css/label_trash.css, included with the
module.
For the Bartik theme, correct positioning of the float labels for the Login and
Search blocks is achieved by entering a "Floatlabel vertical offset" of 0.4em
with the following CSS selector:
"#user-login-form input, #search-block-form input"

ACCESSIBILITY
In terms of accessibility the placeholder attribute is not an alternative to the
label element. Label Trash implements the advice from
http://blog.paciellogroup.com/2011/02/html5-accessibility-chops-the-placeholder-attribute
to "always provide an explicitly associated text label for a text field, using
either the label element or the title attribute."

REFERENCES
Arguably the first: http://mattdsmith.com/float-label-pattern and
http://dribbble.com/shots/1254439--GIF-Float-Label-Form-Interaction

Further discussion and demo:
http://bradfrostweb.com/blog/post/float-label-pattern

jQuery implemenation used by Label Trash:
http://clubdesign.github.io/floatlabels.js (less than 3k when minimized)

Interesting variations:
http://codepen.io/mariusz/pen/CEADo
Nice, mainly CSS, gracefully degrading:
http://codepen.io/psorowka/full/JrKbE
LESS-based version without JS:
http://codepen.io/krnlde/pen/Huexr
Peter Sorowka's  Simple, CSS only (but funky CSS):
http://codepen.io/psorowka/full/JrKbE

An older alternative
http://css-plus.com/examples/2011/09/userfriendly-input-placeholder/

Twitter:
https://twitter.com/floatlabel
