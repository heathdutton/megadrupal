(function ($) {
    Drupal.behaviors.add_logo_watermark = {
        attach: function(context, settings) {
            //Let's add the logo div to the bottom of the body.
            $('body').append('<div id="logo-watermark"><a href="' + Drupal.settings.logo_watermark.url + '" title="' + Drupal.settings.logo_watermark.anchor_title + '"><img src="' + Drupal.settings.logo_watermark.filepath + '"></a></div>');

            // Define container and offset vars.
            var logo_watermark = $("div#logo-watermark a"),
                docHeight = $(document).height(),
                docHeightOffset = docHeight *.85,
                windowHeight = $(window).height();

            // Let's show the new logo div if our window is as tall or taller than 85% of the total document height.
            if (docHeightOffset <= windowHeight) {
                logo_watermark.show();
                logo_watermark.css({
                    'position' : 'fixed',
                    'bottom' : '20px',
                    'right' : '20px',
                    'opacity': 1,
                    'z-index' : 3000
                });

                // Now that we see it, let's make it go away if we shrink the window down.
                // Otherwise, we have a shorter browser window so we'll hide the logo.
                $(function(){
                    $(window).bind('resize', function(){
                        // We're resized. Let's look at the new window height, now.
                        var newWinHeight = $(window).height();
                        if (docHeightOffset >= newWinHeight) {
                            logo_watermark.stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 0, function() {
                                logo_watermark.hide();
                            });

                            // We want to see it again in this short format. Let's make that happen when we scroll down 85% of the page.
                            $(window).scroll(function(){
                                var scrollTop = $(window).scrollTop();
                                var position = ((scrollTop + newWinHeight) / docHeightOffset) * 100;

                                if (position >= 75) {
                                    logo_watermark.show().stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 1);
                                    //Now let's hide it when we scroll back up
                                } else {
                                    logo_watermark.stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 0, function() {
                                        logo_watermark.hide();
                                    });
                                }
                            });
                            // Now that we have expanded the whole window again, let's see the logo.
                        } else {
                            logo_watermark.show().stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 1);
                        }
                    });
                });
            } else {
                logo_watermark.hide();
                logo_watermark.css({
                    'position' : 'fixed',
                    'bottom' : '20px',
                    'right' : '20px',
                    'opacity': 0,
                    'z-index' : 3000
                });

                //Let's get that scrolly scrolly action going.
                $(window).bind('scroll', function(){
                    var scrollTop = $(window).scrollTop();
                    var position = ((scrollTop + windowHeight) / docHeight)*100;

                    if (position >= 95) {
                        logo_watermark.show().stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 1);

                    } else {
                        logo_watermark.stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 0, function() {

                            logo_watermark.hide();

                        });
                    }
                });

                // The browser is big now. Let's get the large browser starting point going again.
                $(window).bind('resize', function(){
                    var newWinHeight = $(window).height();
                    if (docHeightOffset >= newWinHeight) {
                        logo_watermark.stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 0, function() {
                            logo_watermark.hide();
                        });

                        // Back to the scroll function
                        $(window).scroll(function(){
                            var scrollTop = $(window).scrollTop();
                            var position = ((scrollTop + newWinHeight) / docHeightOffset)*100;

                            if (position >= 95) {
                                logo_watermark.show().stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 1);
                            } else {
                                logo_watermark.stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 0, function() {
                                    logo_watermark.hide();
                                });
                            }
                        });

                    } else {
                        logo_watermark.show().stop().fadeTo(Drupal.settings.logo_watermark.fade_speed, 1);
                    }
                });
            }
        }
    };
}(jQuery));

