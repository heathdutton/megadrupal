Contents of this File
------------------
1. Summary
2. Requirements
3. Installation
4. Usage


1. Summary
------------------

The EPS Field module enables Drupal to provide "pseudo-native" handling of the
EPS (Encapsulated PostScript) vector format by providing a custom field to serve
this purpose. This would allow designers working on Drupal projects to upload
design assets received from clients without having to worry about conversions.

Uploaded EPS files are converted to PNG for display and can be processed by
Imagecache just as files uploaded using the regular image field. The module has
been tested with files using CMYK and PMS color spaces and maintains acceptable
consistency in colors between conversion.

NB: The field also accepts png, gif and jpeg formats, so there is no need to
have an extra field in a content type if you want to be able to use those
formats as well.


2. Requirements
------------------

This module requires ImageMagick (http://www.imagemagick.org) to be installed
and enabled on your server.

Consult your server administrator or hosting provider if you are unsure about
these requirements.


3. Installation
------------------

- Install as usual, see http://drupal.org/node/70151 for further information.
- Download the colorprofiles.tar.gz file from http://bit.ly/1bfUuOJ and extract
  to the libraries folder such that it looks like:
  
  sites/*/libraries/colorprofiles/

4. Usage
------------------

After installing the module, the "EPS to Image" field will be available to add
to your content types and used as a regular Image field would be used.
