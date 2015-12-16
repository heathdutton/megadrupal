(function ($) {
  Drupal.behaviors.image_preloader = {
    attach: function (context, settings) {
      var nodeid = Drupal.settings.image_preloader.nodeid;
      var loadericon = Drupal.settings.image_preloader.loadericon;

      for(i = 0; i < nodeid.length; i++) {
        k = nodeid[i].image_preloader_id;
        $(k).image_preloader_icon({
          loadericon : loadericon
        });
      }
    }
  };

  $.fn.image_preloader_icon = function(options){

    var defaults = {
      delay:200,
      preload_parent:"a",
      check_timer:300,
      ondone:function(){ },
      oneachload:function(image){  },
      fadein:500,
      loadericon : '../images/loader-1.gif'
    };

    // Variables declaration and precaching images and parent container.
    var options = $.extend(defaults, options),
    root = $(this) , images = root.find("img").css({"visibility":"hidden",opacity:0}) ,  timer ,  counter = 0, i = 0 , checkFlag = [] , delaySum = options.delay ,

    init = function(){
      timer = setInterval(function(){
        if(counter >= checkFlag.length){
          clearInterval(timer);
          options.ondone();
          return;
        }

        for(i = 0; i < images.length; i++){
          if(images[i].complete == true){
            if(checkFlag[i] == false){
              checkFlag[i] = true;
              options.oneachload(images[i]);
              counter++;
              delaySum = delaySum + options.delay;
            }
            $(images[i]).css("visibility","visible").delay(delaySum).animate({opacity:1},options.fadein,
            function(){ $(this).parent().removeClass("image-preloader-icon"); });
          }
        }
      },options.check_timer)
    };

    images.each(function(){
      if($(this).parent(options.preload_parent).length == 0){
        $(this).wrap("<a class='image-preloader-icon' />");
      }
      else{
        $(this).parent().addClass("image-preloader-icon");
      }

      checkFlag[i++] = false;
    });
    images = $.makeArray(images);

    var icon = jQuery("<img />",{
      id : 'loadingicon' ,
      src : options.loadericon
    }).hide().appendTo("body");

    timer = setInterval(function(){
      if(icon[0].complete == true){
        clearInterval(timer);
        init();
        icon.remove();
        return;
      }
    },100);

  };

}(jQuery));
