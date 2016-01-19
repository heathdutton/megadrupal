(function ($) {
  Drupal.behaviors.lazyloaded = {
    attach: function(context) {
      $('.lazyloaded:not(.lazyloaded-processed)', context).addClass('lazyloaded-processed').each(function () {

        var $this = $(this).addClass('lazyloaded-processed'),
        // data = $this.data() <<<< Use this with newer jQuery versions
        data = {
          'nid': $this.attr('data-nid'),
          'plugin': $this.attr('data-plugin')
        };

        $this.find('.button').one('focus mouseover', function () {
          $this.load(Drupal.settings.basePath + '?q=lazyloaded/' + data.nid + '/' + data.plugin);
        });

      });


      // http://stackoverflow.com/a/488073
      function isScrolledIntoView(elem) {
          var docViewTop = $(window).scrollTop();
          var docViewBottom = docViewTop + $(window).height();

          var elemTop = $(elem).offset().top;
          var elemBottom = elemTop + $(elem).height();

          return ((elemBottom <= docViewBottom) && (elemTop >= docViewTop));
      }

      var $lazyloadedVertical = $('.lazyloaded-vertical');

      // check if the vertical sharebar is set
      if($lazyloadedVertical.length) {
        $(window).scroll(function () {
          // sharebar is outside the viewport
          if(isScrolledIntoView($lazyloadedVertical) == false) {
            $lazyloadedVertical.addClass('lazyloaded-sticky');
          }

          // sharebar is inside the viewport
          else if(isScrolledIntoView($lazyloadedVertical) == true) {
            $lazyloadedVertical.removeClass('lazyloaded-sticky');
          }

        });
      }

    }
  };
})(jQuery);