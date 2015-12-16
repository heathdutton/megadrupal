
-- SUMMARY --

This module provides entity based blackboards using HTML canvas elements.
Visitors that have the permission can draw onto those blackboards and save their
changes to an image field on a scribble entity.

Besides that, the images can be displayed as slideshows, managed and also down-
loaded by administrators.

Think of it as a plaster on which people write their message when you broke your
leg for example. Now they can do so on your website too using this module.

-- DISCLAIMER --

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

-- REQUIREMENTS --

- The jqscribble JS library. It is available at
  http://github.com/jimdoescode/jqScribble

- The field_slideshow module available at
  http://drupal.org/project/field_slideshow

- The jquery_colorpicker module available at
  http://drupal.org/project/jquery_colorpicker

-- INSTALLATION --

- Download the jqscribble library and place it into the
  sites/all/libraries/jqscribble folder such that the js file path is
  sites/all/libraries/jqscribble/jquery.jqscribble.js

- Download the field_slideshow as well as the jquery_colorpicker modules and
  place them into your sites/all/modules/contrib folder.

- Enable the field_slideshow and jquery_colorpicker modules using drush en
  command or on the modules page.

- Enable the scribble module.

-- USAGE --

The admin overview for scribbles is located at admin/config/media/scribble.

- Add a scribble
  Click "Add scribble" on the admin overview and create a scribble entity.

- View the blackboard to add images
  On the admin overview follow the "Blackboard" link and draw images. You
  might want to link that path in some menu to make it accessible for visitors.

- View and delete scribble images and snapshots
  On the admin overview, follow the "Image list" link. Use the  delete links to
  remove a certain snapshot from all images that were drawn after.

- Download the images of a scribble
  On the admin overview, use the "Download" link to download a zip archive of
  all images of a scribble.

- View the slideshow
  On the admin overview follow the "Slideshow" link and draw images. Again, you
  might want to link that path in some menu to make it accessible for visitors.

-- TROUBLESHOOTING --

If you encounter any problems with the module, please check the issue queue of
the module on the project page on drupal.org.
If your problem isn't covered by an existing issue please create a new one.

-- ROADMAP --

- Fix toolbar width
- Add mobile support
- Add possibility for drawing message

Current maintainer: s_leu
