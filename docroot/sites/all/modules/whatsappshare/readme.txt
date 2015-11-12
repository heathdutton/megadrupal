Whatsappshare Module
=======================================
Whatsappshare module provide a link to share your page and custom text on every node title page.
Once enable the module then go to /admin/config/services/whatsappshare page.
Whatsappshare module provides a admin page to customized -
1. Text(Share) which will appear after title on every node page.
2. An option to manage the size of whatsapp share button size.
3. The custom text which has to be share.

Original idea is inherited from http://whatsapp-sharing.com/ 

FAQ

1. Everything is configured properly still I am not getting whatsapp share button.
Please check on mobile device bro! Button will not appear in PC browser.
Javascript will detect the device automatic and render the link.

2. Can I add custom CSS and JS.
Try it yourself. Currently it is tested on default theme only.

3. Share button is coming on every node page including login also.
Yes JS script is configured in such a way that it will search id "#page-title" on every page and add whatsapp share link.

4. Can I move the whatsapp share link on the page.
No this feature is not available right now. If you still need to change then open whatsappshare.js file and on the place of #page-title id use your custom id or class of div, where you want to move.
