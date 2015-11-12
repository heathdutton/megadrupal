(function ($) {
  Drupal.behaviors.muchomenu = {
    attach: function(context) {
      var timeout				= 500;
      var sizewait			= 250;
      var hoverwait     = 400;
      var hovertimer    = null;
      var sizetimer 		= null;
      var closetimer 		= null;
      var hoverParent 	= null;
      var hoverBin 			= null;
      var hoverSlots 		= null;
      var megaSlots = null;
      var megaParents = null;
      var hideOffset = -9000;

      var megaParents = $('.muchomenu-menu').find('.muchomenu-parent');
      var megaParentTitles = $('.muchomenu-menu').find('.muchomenu-parent-title');
      var megaBins = $('.muchomenu-menu').find('.muchomenu-bin');
      var oldParentIdx = -1;
      var hoverParentIdx = -2;

      megaBins.css('top', hideOffset);
      megaBins.hide();

      $('a.disable-click', megaParents).attr('href', 'javascript:void(0)');

      function muchomenu_open(){
        muchomenu_canceltimer();

        if ($(this).hasClass('muchomenu-parent-title')) {
          hoverParentIdx = megaParentTitles.index($(this));
        }
        else if ($(this).hasClass('muchomenu-bin')) {
          hoverParentIdx = megaParents.index($(this).parents('.muchomenu-parent'));
        }


        hoverParent = megaParents.eq(hoverParentIdx);

        if (hoverParentIdx != oldParentIdx) {
          muchomenu_close();
          muchomenu_hovertimer();
        } else {
          muchomenu_display();

        }
      }

      function muchomenu_display() {
        if (hoverParent) {
          hoverParent.addClass('hovering');
          hoverBin = hoverParent.find('.muchomenu-bin');
          /* display position */
          //hoverBin.css('top', 'auto');
          /* display position end */
          animateIn(hoverBin);
        }
      }

      function muchomenu_close(){

        if (hoverParent) {
          oldParentIdx = $('.muchomenu-parent').index(hoverParent);
        }
        for ( var i=0 ; i < megaParents.length ; i++ ) {
          megaParents.eq(i).removeClass('hovering');
        }
        animateOut(hoverBin);
      }

      function checkHorizontalPosition(item) {
        var parentOffset = item.parents('.muchomenu-parent').offset();
        var screenWidth = $(document).width();
        var itemPosition = item.position();
        var padding = 30;

        var p = item.width() + itemPosition.left + parentOffset.left + padding;
        if (p > screenWidth) {
          var newOffset = screenWidth - p;
          item.css('left', newOffset + 'px');
        }

      }

      function animateIn(item) {
        if(item && !item.is(':visible'))
        {
          item.css('top', 'auto');
          checkHorizontalPosition(item);

          switch(Drupal.settings.muchomenu.animationEffect) {
            case 'fade':
              item.fadeIn();
              break;

            case 'slide':
              item.slideDown();
              break;

            default:
              item.show();
              break;
          }
        }
      }

      function animateOut(item) {
        if(item)
        {
          switch(Drupal.settings.muchomenu.animationEffect) {
            case 'fade':
              item.fadeOut();
              break;

            case 'slide':
              item.slideUp();
              break;

            default:
              item.hide();
              break;
          }
        }
      }

      function muchomenu_closeAll(){

        if(hoverBin) {
          animateOut(hoverBin);
        //hoverBin.css('top', hideOffset);
        }
        for ( var i=0 ; i < megaParents.length ; i++ ) {
          megaParents.eq(i).removeClass('hovering');
        }
        oldParentIdx = -1;
      }

      function muchomenu_stopPropagation(event){
        event.stopPropagation();
      }

      function muchomenu_timer(){
        muchomenu_canceltimer();
        muchomenu_cancelhovertimer();
        closetimer = window.setTimeout(muchomenu_closeAll, timeout);

      }

      function muchomenu_canceltimer(){
        if (closetimer) {
          window.clearTimeout(closetimer);
          closetimer = null;
        }
      }


      function muchomenu_hovertimer(){
        muchomenu_cancelhovertimer();
        hovertimer = window.setTimeout(muchomenu_display, hoverwait);
      }

      function muchomenu_cancelhovertimer(){
        if (hovertimer) {
          window.clearTimeout(hovertimer);
          hovertimer = null;
        }
      }

      function muchomenu_sizetimer(){
        /* waits to resize on initial call to accomodate browser draw */
        sizetimer = window.setTimeout(muchomenu_sizer, sizewait);
      }

      function muchomenu_sizer(){

      }


      $('.muchomenu-parent-title').bind('mouseover', muchomenu_open);
      $('.muchomenu-parent-title').bind('mouseout', muchomenu_timer);

      $('.muchomenu-bin').bind('mouseover', muchomenu_open);
      $('.muchomenu-bin').bind('mouseout', muchomenu_timer);

      $("body").bind('click', muchomenu_closeAll);
      $(".muchomenu-menu").bind('click', muchomenu_stopPropagation);

      $(window).bind('resize', muchomenu_sizer);
      muchomenu_sizetimer();
    }
  };
})(jQuery);