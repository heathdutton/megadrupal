/**
 * JQuery Pic360 Plugin.
 * Version: 1.0.0 (2011.5.22)
 * @requires jQuery v1.4.4 or later.
 * @author Yongliang Wen.
 * @email wenzongxian@gmail.com
 *
 * Dual licensed under the MIT and GPL licenses.
 *   http://www.opensource.org/licenses/mit-license.php
 *   http://www.gnu.org/licenses/gpl.html
 */
;(function($) {
  $.fn.pic360 = function() {
    var first_img = this.find('img:first');
    var all_img = this.find('img');
    var img_count = all_img.length;
    if (img_count == 0) {
      return;
    }
    var img_width = first_img.width();
    // Induction zone width.
    var chg_width = parseInt(img_width / img_count);
    var imgs_left = first_img.offset().left;
    this.toggle();
    all_img.toggle();
    first_img.toggle();
    // Set width of the container.
    this.width(img_width);
    // Set container height.
    this.height(first_img.height());
    var mouseX = 0;
    var start = false;
    var step = 0;
    // Current sensing area.
    var curr_step = 0;
    // Current picture.
    var curr_img = 0;
    // Move the mouse DIV.
    this.mouseover(function(e){
      start = true;
      if (start) {
        mouseX = e.screenX;
        // Get the current sensing area.
        curr_step = parseInt((mouseX - imgs_left) / chg_width);
        step = curr_step;
      }
    })
    this.mouseout(function(e){
      start = false;
    })
    this.mousemove(function(e){
      if (start) {
        curr_step = parseInt((e.screenX - imgs_left) / chg_width);
        if (curr_step != step) {
          // Hide the current image.
          $(all_img[curr_img]).toggle();
          if (curr_step > step) {
            curr_img = curr_img + 1;
            if (curr_img >= img_count) {
              curr_img = 0;
            }
          }
          else {
            curr_img = curr_img - 1;
            if (curr_img < 0) {
              curr_img = img_count - 1;
            }
          }
          $(all_img[curr_img]).toggle();
          step = curr_step;
        }
      }
    })
  };
})(jQuery);

/**
 * Initialize all Pic360 objects.
 */
var JJ = jQuery.noConflict();
JJ(document).ready(function(){
  JJ('.PIC360').each(function() {
    JJ(this).pic360()
  })
})
