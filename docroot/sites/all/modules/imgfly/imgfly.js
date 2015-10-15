/**
 * @file
 * Javascript file that performs the image path manipulations.
 */
(function ($) {
  
  // Attaching to Drupal behaviors
  Drupal.behaviors.imgfly_js = {
    loadwatch: function() {
      return 0 + $(window).scrollTop() + $(window).height() + parseInt(Drupal.settings.imgfly.loadvalue);
    },
    imagereplace: function(){
      $('.imgfly-placeholder').each(function() {
        var imgSrc = $(this).attr('data-src');
        var aspect = $(this).attr('data-aspect');
        var filesPath = imgSrc.match(/\/sites\/.+\/files\//);
        var imgWidth = $(this).width();
        var imgHeight = (aspect > 0) ? Math.round(imgWidth / aspect) : $(this).height();
        var newPath = '/imgfly/' + imgWidth + '/' + imgHeight + '/';
        if (imgSrc.indexOf(filesPath[0]) != -1) {
          if ($(this).attr('src').indexOf(imgSrc.replace(filesPath[0], newPath)) == -1) {
            if (Drupal.settings.imgfly.lazyload){
              if ($(this).offset().top < Drupal.behaviors.imgfly_js.loadwatch()) {
                $(this).attr('src', imgSrc.replace(filesPath[0], newPath));
              }
            } else {
              $(this).attr('src', imgSrc.replace(filesPath[0], newPath));
            }
          }
        }
      });
    },
    attach: function (context, settings) {
      Drupal.behaviors.imgfly_js.imagereplace();
      $(window).on('scroll', function(){
        Drupal.behaviors.imgfly_js.imagereplace();
      });
    }
  };
}(jQuery));







