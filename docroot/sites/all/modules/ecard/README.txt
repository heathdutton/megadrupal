******
E-Card
******
This module is for sending E-Cards to another person. For this you will get a
field that creates a form. If the form is completed an e-mail will be send to
the desired person with a special url. With this url the loaded page will
display the message you did enter.


******
Setup
******
The E-Card field can be added to any entity and bundle. Just create the field,
save it's settings and you are basically done. The message will be displayed
right where the form is. This is done by a formatter. So please take a look at

admin/structure/types/manage/[your node type]/display


******
How to display the E-Card text on an image
******
If you add an image to an entity you will find that there is an E-Card formatter
for this field type. This is basically a wrapper for the default image formatter
but it uses the ecard_image.tpl.php to enhance the theming options.


******
Render Text onto an image
******
to be done...

******
HTML E-Cards
******
to be done...

******
Author / Maintainers D7
******
Karsten Frohwein <karsten dot frohwein at comm-press dot de>
Carsten Rhein    <carsten dot rhein at comm-press dot de>
http://www.comm-press.de