/**
 * @file
 * Base trigger file for field_slideshow_threesixty.js.
 */

(function($){
  Drupal.behaviors.field_slide_show_j360 = {
    attach: function(context, settings) {
      window.onload = init;
        function init(){
        // @todo get all slideshow threesixty emelements from a page.
        // In case views page rendered more the one node at a page.
        // We will get atleast 20.
        for(var i = 1; i <= 20; i++){
          var product = $("div").hasClass("product-" + i);
            if(product == true){
            var t_frame = $("ol.threesixty_images-" + i + " li").length;
            var e_frame = t_frame;
            var img_list = ".threesixty_images-" + i;
            var prog = ".spinner-" + i;
            // @todo we set field display settings values as class names.
            // Now we get back and pass to the javascript libarary.
            // @see images_rotate.tpl.php.
            var className = $(".threesixty_images-" + i).attr('class');
            var classArray = className.split(' ');
            for(var s = 0; s < classArray.length; s++) {
              var width_str = classArray[s].match(/width-([0-9]{2,4})/);
              var height_str = classArray[s].match(/height-([0-9]{2,4})/);
              var navigation = false;
              if (null !== width_str) {
                var width = parseInt(width_str[1]);
                $(".threesixty_images-" + i).removeClass(classArray[s]);
              }
              if (null !== height_str) {
                var height = parseInt(height_str[1]);
                $(".threesixty_images-" + i).removeClass(classArray[s]);
              }
              if(classArray[s] == 'navigation-yes') {
                navigation = true;
                $(".threesixty_images-" + i).removeClass(classArray[s]);
              }
              if(classArray[s] == 'navigation-no') {
                navigation = false;
                $(".threesixty_images-" + i).removeClass(classArray[s]);
              }
            }
            // Base trigger for javascript library.
            $('.product-' + i).ThreeSixty({
              totalFrames:t_frame,
              endFrame: e_frame,
              currentFrame: 1,
              imgList:img_list,
              progress:prog,
              height: height,
              width: width,
              navigation: navigation,
              prod_id:i
            });
          }
        }
      }
    }
  };
})(jQuery);
