/*
 * http://flesler.blogspot.hu/2007/10/jquerylocalscroll-10.html
 * http://flesler.blogspot.hu/2007/10/jqueryscrollto.html
 */

(function($) {
  function jQueryLocalScrollSetActive(EventObject, TargetedElement, target) {
    $('a.active', $(this.index)).each(function () {
        $(this).removeClass('active');
        $(this).parent('li').removeClass('active');
        $(this).parent('li').removeClass('active-trail');
    });

    $(EventObject.srcElement).addClass('active');
    $(EventObject.srcElement).parent('li').addClass('active');
    $(EventObject.srcElement).parent('li').addClass('active-trail');

    // When using floating block, the offset should be different, than setting.
    // If the menu is floating by module flating_block, the offset will be
    // wrong, because floating_block module also use the scroll event.
    if ($(this.index).hasClass('blockFloat-processed') && !$(this.index).hasClass('floating-block-active')) {
      this.offset = parseInt(Drupal.settings.jquery_localscroll.offset, 10) - $(this.index).height();
      //this.offset = parseInt(Drupal.settings.jquery_localscroll.floating_block_offset, 10);
    }
    else {
      this.offset = parseInt(Drupal.settings.jquery_localscroll.offset, 10);
    }
  }

  Drupal.behaviors.jQueryLocalScroll = {
    attach: function(context) {
      if (Drupal.settings.jquery_localscroll.global == 1) {
        $.localScroll.hash();
      }
      // All process starts on ids set up in drupal admin. The index contains
      // #foo, #bar, html ids.
      $.each(Drupal.settings.jquery_localscroll.ids, function (index, value) {
        // Cleanup active and activetrail classes. If hash is in the url,
        // only the link, contains the hash stay active.
        $('a.active', index).each(function(){
          if (this.hash != window.location.hash) {
            $(this).removeClass('active');
            $(this).parent('li').removeClass('active');
            $(this).parent('li').removeClass('active-trail');
          }
        });

        $(index).localScroll({
          hash: Drupal.settings.jquery_localscroll.hash,
          duration:parseInt(Drupal.settings.jquery_localscroll.duration, 10),
          // Offset will be set on onBefore event.
          offset:0,
          // Add the index, need it to set active links.
          index: index,
          onBefore: jQueryLocalScrollSetActive
        });

        if (Drupal.settings.jquery_localscroll.waypoints == 1) {
          // Start working on links in index.
          $('a', index).each(function(i) {
            var sender = $(this);
            var senderparent =  $(this).parents('div').attr('id');

            // Reach the waypoint in content
            if (this.hash !== '') {
              $(this.hash).waypoint(function(event, direction) {
                var reached = event.target.id;

                if (Drupal.settings.jquery_localscroll.onscroll == 1) {
                  // And go back to the affected menu to change active class
                  $('a', index).each(function () {
                    $(this).removeClass('active');
                    $(this).parent('li').removeClass('active');
                    $(this).parent('li').removeClass('active-trail');

                    if (this.hash == '#' + reached) {
                      $(this).addClass('active');
                      $(this).parent('li').addClass('active');
                      $(this).parent('li').addClass('active-trail');
                    }
                  });
                }
                if (Drupal.settings.jquery_localscroll.onscroll_update_hash == 1) {
                  if ('#' + reached != window.location.hash) {
                    // When reach the element, we set the hash manually, so
                    // we need to prevent trigger the original click event.
                    //$(this).click(function (event) {
                    //  event.preventDefault();
                    //});
                    location.hash = reached;
                  }
                }
                console.log($.waypoints('up'));
              }, {
                offset:function () {
                  return -parseInt(Drupal.settings.jquery_localscroll.offset);
                },
                continuous: false,
              });
            }
          });
        }

      });
    }
  };
})(jQuery);
