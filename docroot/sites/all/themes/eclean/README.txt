For more information please visit http://drupal.org/node/456

CONTENTS OF THIS FILE
---------------------

 * About Drupal
 * About eClean theme
   - Specifications
   - Observations
 * Installing Drupal Themes
 

*** About Drupal
------------
Drupal is an open source content management platform supporting a variety of
websites ranging from personal weblogs to large community-driven websites. For
more information, see the Drupal website at http://drupal.org/, and join the
Drupal community at http://drupal.org/community.

Legal information about Drupal:
 * Know your rights when using Drupal:
   See LICENSE.txt in the same directory as this document.
 * Learn about the Drupal trademark and logo policy:
   http://drupal.com/trademark


*** About eClean theme
---------------------------------------
The theme is intended for various types of sites. The main characteristics of eClean are responsivity and that
it is built with HTML5 and CSS3. The layout is multi-column with fluid width and tableless.

- Specifications
  1. Max-width: 1140px.
  2. Min-width: 270px.
  3. Responsivity width steps:  920px
                                840px
                                480px
                                380px

  4. Works fine on: Chrome
                    Firefox 9+
                    Internet Explorer 7+
                    Opera 11+
                    Safari 5+
                    Smartphones
                      Android
                      iPhone

- Observations
  1. eClean is tested in Internet Explorer and the layout shows fine, but a lot of effects does not appear there.
     Therefore, the compatibility with the Internet Explorer is not 100%
  2. When the RDF module is enabled, the doctype is changed to HTML+RDF. The W3C Validator is not compatible with
     this doctype and returns a lot of erros.
  3. If you want to modify some css, I recommend you to use LESS (http://lesscss.org/) as the CSS for this theme
     was build with it and all LESS files are available in the folder "less".


*** Installing Drupal Themes 
---------------------------------------
1. Access your Web server using an FTP client or Web server administration tools.
2. Create a folder for your specific theme under "<YourSiteFolder>/sites/all/themes/" folder within Drupal installation.
   For example: <YourSiteFolder>/sites/all/themes/<MyNewTheme>
3. Copy or upload theme files into the newly created <MyNewTheme> folder.
4. Login to your Drupal Administer.
5. Go to Drupal Administer -> Appearance (www.YourSite.com/?q=admin/appearance)
6. In the bottom of the page you will find the theme you've downloaded to the location of your site.
7. Click "Enable and set default" below the theme.
For more information please visit: http://drupal.org/node/456


---------------------------------------
by Dhavyd Vanderlei - http://www.dhavyd.com