Readme file for the Image CAPTCHA  Ajax module for Drupal
---------------------------------------------

image_captcha_ajax.module is an extesion for Image CAPTCHA module, offering a http/ajax api for image CAPTCHA
.

Submodule image_captcha.module offers an image based challenge.

Installation:
  Installation is like with all normal drupal modules:
  extract the 'image_captcha_ajax' folder from the tar ball to the
  modules directory from your website (typically sites/all/modules).

Dependencies:
  CAPTCHA module, Image CAPTCHA module

Usage:

  A. 
  This module only provide two http/ajax api:
  
  1). 
  Initialize the captcha session firstly: 
  
  URL : http://localhost/ajax/image_captcha_sid
        return the captcha image url and the sid.
  2).
  get captcha image
  URL: http://localhost/ajax/image_captcha_img/[sid]


  B. 
  Another simple way: 
  The client only request the URL:

  http://localhost/ajax/image_captcha?refresh=123
  this url will generate the captcha image, and will save sid to cookie automatically, refresh the url will regenerate the image also.

  Please note, the sid will save to cookie, so the client need support cookie.

  Validate: 
  use this function,
  <code>
  image_captcha_ajax_validate_captcha
 </code>



  
