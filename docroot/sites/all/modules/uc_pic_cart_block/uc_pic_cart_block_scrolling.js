
/**
 * @file
 * Scrolling script for our block.
 */

(function($) {


$(document).ready(function () {
    if (Drupal.settings.ucPicCartBlockUseScroll == true) {
      var scrollArea = $("#uc_pic_cart_block_scroll_area");
      var contentArea = $("#uc_pic_cart_block_content");

      var newHeight = 0;
      var delta = 0;

      if (Drupal.settings.ucPicCartBlockOrientation == 0) {
        
        // Average height of items multiplied by scroll count
        newHeight = contentArea.height() / Drupal.settings.ucPicCartBlockProductCount * Drupal.settings.ucPicCartBlockScrollCount;
        delta = $("tr:last", contentArea).height(); // height of row with count and sum
        
        var arrowUp = $(".uc_pic_cart_block_scroll_up_def");
        var arrowDown = $(".uc_pic_cart_block_scroll_down_def");
        
        scrollArea.addClass("uc_pic_cart_block_scroll_area_vert");
        
        arrowUp.removeClass("uc_pic_cart_block_scroll_up_def").addClass("uc_pic_cart_block_scroll_up_scroll");
        arrowDown.removeClass("uc_pic_cart_block_scroll_down_def").addClass("uc_pic_cart_block_scroll_down_scroll");
        
        scrollArea.height(newHeight);
        
        arrowUp.click(function () {
            scrollArea.scrollTop(scrollArea.scrollTop() - delta);
          }
        );
        arrowDown.click(function () {
            scrollArea.scrollTop(scrollArea.scrollTop() + delta);
          }
        );
      }
      else {

        var div = $("div.uc_pic_cart_block_item_hor");
        var wdt = div.outerWidth(true);
        var wdtNeed = (wdt + 2) * Drupal.settings.ucPicCartBlockProductCount;
        
        if (wdtNeed < scrollArea.width()) return;
        
        newHeight = div.outerHeight(true);
        delta = wdt / 2;

        var arrowLeft = $(".uc_pic_cart_block_scroll_left_def");
        var arrowRight = $(".uc_pic_cart_block_scroll_right_def");
        
        arrowLeft.removeClass("uc_pic_cart_block_scroll_left_def").addClass("uc_pic_cart_block_scroll_left_scroll");
        arrowRight.removeClass("uc_pic_cart_block_scroll_right_def").addClass("uc_pic_cart_block_scroll_right_scroll");
        
        arrowLeft.height(newHeight);
        arrowRight.height(newHeight);
        
        var img = $("img", arrowLeft);
        img.css({'position' : 'relative', 'top' : newHeight / 2 - img.height()});
        img = $("img", arrowRight);
        img.css({'position' : 'relative', 'top' : newHeight / 2 - img.height()});
        
        scrollArea.addClass("uc_pic_cart_block_scroll_area_hor");
        contentArea.height(newHeight);
        contentArea.width(wdtNeed);

        // setting absolute position and width - need for crossbrowsing :(
        var lft=arrowLeft.position().left + arrowLeft.outerWidth(true) + 1;
        var wdtBlock = arrowRight.position().left - lft-arrowRight.outerWidth(true) + arrowRight.innerWidth() + 1;
        scrollArea.css({"position" : "absolute", "top" : arrowLeft.position().top, "left" : lft});
        scrollArea.width(wdtBlock);

        arrowLeft.click(function () {
            scrollArea.scrollLeft(scrollArea.scrollLeft() - delta);
          }
        );
        arrowRight.click(function () {
            scrollArea.scrollLeft(scrollArea.scrollLeft() + delta);
          }
        );
      }
    }
  }
)
})(jQuery)