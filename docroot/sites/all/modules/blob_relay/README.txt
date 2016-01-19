This module tries to provide for a decent download of browser side content.

The data at hand is send back to the server which triggers a download dialog.

CONFIGURATION

- best way to use this module is through the API

- second best is to enable the block '
TECHNICAL

Solutions found online for client side created file content triggering
the browsers normal file download behavior does not provide for 
a decent filename let alone extension.

- http://stackoverflow.com/questions/283956/is-there-any-way-to-specify-a-suggested-filename-when-using-data-uri/6943481#6943481
  - answer no unless Google Chrome and / or WHATWG

- http://stackoverflow.com/questions/3665115/create-a-file-in-memory-for-user-to-download-not-through-server
- http://stackoverflow.com/questions/2897619/using-html5-javascript-to-generate-and-save-a-file
- http://stackoverflow.com/questions/6796974/force-download-an-image-using-javascript
- http://stackoverflow.com/questions/10473932/browser-html-force-download-of-image-from-src-dataimage-jpegbase64

Submitting the form is inspired by
- https://code.google.com/p/download-data-uri/ and it's support script
  - http://download-data-uri.appspot.com/js/download-data-uri.js
