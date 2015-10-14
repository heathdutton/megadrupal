(function ($) {

    var spot = -1;

    jQuery.xhrPool = [];
    jQuery.xhrPool.abortAll = function() {
        jQuery(this).each(function(idx, jqXHR) {
            jqXHR.abort();
        });
        $.xhrPool.length = 0
    };


    Drupal.behaviors.spotlight = {
        attach: function (context, settings) {
            jQuery('.spotlight-input').blur(function(){
                setTimeout("jQuery('.spotlight-result-wrapper').hide()", 150);
            });

            jQuery('.spotlight-input').focus(function(){
                jQuery('.spotlight-result-wrapper').show();
                jQuery('.spotlight-result-wrapper:empty').hide();
            });

            jQuery('.spotlight-result-wrapper:empty').hide();

            jQuery(document).delegate('.spotlight-item', 'click', function(){
                var url = Drupal.settings.spotlight.ajaxURL + '/spot';
                var key = jQuery('.spotlight-input').val();
                var href = jQuery(this).attr('href');
                var title = jQuery(this).find('.spotlight-item-title').text();
                var icon = jQuery(this).find('i').attr('class');

                console.log('clicked: ' + href);

                jQuery.ajax({
                    url: url,
                    data: {key: key, title: title, icon: icon, path: href},
                    success: function(response) {
                        console.log(response);
                    }
                });
            });


            jQuery('.spotlight-input').keydown(function(e){
                var items = jQuery('.spotlight-item');

                var x = 0;

                if (e.keyCode == 40) {
                    x = 1;
                } else if (e.keyCode == 38) {
                    x = -1;
                }

                spot += x;

                if(spot < 0) {
                    spot = 0;
                }

                if(spot >= items.length) {
                    spot = items.length - 1;
                }

                items.removeClass('active');
                items.eq(spot).addClass('active');

                if (e.keyCode == 13) {
                    //items.eq(spot).focus();
                    items[spot].click();
                }
            });

            // Use jQuery.debounce() when available to prevent triggering a
            // search on every single keystroke.
            var spotlight_search_trigger = function () {
                var key = jQuery(this).val();
                spotlight_search(key);
            };
            if (jQuery.isFunction(jQuery.debounce)) {
                jQuery('.spotlight-input').bind('input', jQuery.debounce(250, spotlight_search_trigger));
            }
            else {
                jQuery('.spotlight-input').bind('input', spotlight_search_trigger);
            }

            // workaroudn for IE 9
            var spotlight_search_trigger_ie9 = function (e) {
                if (e.originalEvent.propertyName == "value") {
                    var key = jQuery(this).val();
                    spotlight_search(key);
                }
            };
            if (jQuery.isFunction(jQuery.debounce)) {
                jQuery('.spotlight-input').bind('propertychange', jQuery.debounce(250, spotlight_search_trigger_ie9));
            }
            else {
                jQuery('.spotlight-input').bind('propertychange', spotlight_search_trigger_ie9);
            }

            jQuery('.spotlight-wrapper').ajaxStart(function() {
                jQuery(this).find('.spotlight-spin').css('visibility', 'visible');
            });

            jQuery('.spotlight-wrapper').ajaxStop(function() {
                jQuery(this).find('.spotlight-spin').css('visibility', 'hidden');
            });
        }
    };


})(jQuery);

function spotlight_search(key) {
    var url = Drupal.settings.spotlight.ajaxURL;

    if(key == '') {
        jQuery('.spotlight-result-wrapper').html('').hide();
        return;
    }

    var request = jQuery.ajax({
        beforeSend: function(jqXHR) {
            //jQuery.xhrPool.abortAll();
            //jQuery.xhrPool.push(jqXHR);
        },
        method: 'GET',
        cache: true,
        async: true,
        url: url,
        data: {key: key},
        dataType: 'json',
        success: function(response) {
            if(response == '') {
                setTimeout("jQuery('.spotlight-result-wrapper').html('').hide()", 150);
                return;
            }

            jQuery('.spotlight-result-wrapper').html(response);
            jQuery('.spotlight-result-wrapper').show();
            // Don't do a full Drupal.attachBehaviors() here since that can be
            // slow, and Spotlight results are supposed to appear as fast as
            // possible. Instead, trigger an event so other JavaScript can
            // respond to the search results being added if it needs to.
            jQuery('.spotlight-result-wrapper').trigger('spotlight.add_results');
        },
        complete: function(jqXHR) {
            /*var index = jQuery.xhrPool.indexOf(jqXHR);
            if (index > -1) {
                jQuery.xhrPool.splice(index, 1);
            }*/
        }
    });
}