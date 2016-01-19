// Exposebox.js

(function($){
    Drupal.behaviors.exposebox = {
        attach:	function(context, settings) {
            $('a.exposebox-expose-link', context)
                .each(function(){
                    var q = exposebox_getUrlVars($(this));
                    $(this).html(decodeURI(q['exposeText']));
                    //hide the link if the expose text is not taller
                    //than the css height
                    var linkPosition = q['link_position'];
                    if (linkPosition == 'below') {
                        var $targ = $(this).parent().prev('.exposebox-closed', context).first();
                    }
                    else {
                        var $targ = $(this).parent().next('.exposebox-closed', context).first();
                    }
                    if(!$targ.is(':visible')) {
                        var $checkElement = $targ.clone().appendTo('body').css({'height':'auto'});
                        var actualHeight = $checkElement[0].scrollHeight;
                        setHeight = (q['height'] == undefined) ? $checkElement.height() : q['height'];
                        $checkElement.remove();
                    }
                    else {
                        var actualHeight = $targ[0].scrollHeight;
                        setHeight = (q['height'] == undefined) ? $targ.height() : q['height'];
                    }
                    if(actualHeight <= setHeight) {
                        $targ.css({'height' : 'auto'});
                        $(this).remove();
                    }
                });
            $('body', context).delegate('a.exposebox-expose-link', 'click', function(e){
                var query = exposebox_getUrlVars($(this));
                var linkPosition = query['link_position'];

                if($(this).hasClass('expose-processed')) {
                    if (linkPosition == 'below') {
                        var target = $(this).parent().prev('.exposebox-exposed', context);
                    }
                    else {
                        var target = $(this).parent().next('.exposebox-exposed', context);
                    }
                    target.toggleClass('exposebox-closed').toggleClass('exposebox-exposed');
                    $(this).toggleClass('expose-processed');
                    $(this).html(decodeURI(query['exposeText']));
                    target.first().trigger('exposeboxExposeComplete');
                }
                else {
                    if (linkPosition == 'below') {
                        var target = $(this).parent().prev('.exposebox-closed', context);
                    }
                    else {
                        var target = $(this).parent().next('.exposebox-closed', context);
                    }
                    target.toggleClass('exposebox-closed').toggleClass('exposebox-exposed');
                    $(this).toggleClass('expose-processed');
                    $(this).html(decodeURI(query['closeText']));
                    target.first().trigger('exposeboxExposeComplete');
                }
                e.preventDefault();
            });
        }
    };
    /**
     * provide extraction of get param from url
     * @return array of ['var']=>value
     */
    function exposebox_getUrlVars(element){
        var vars = [], hash;
        //if element is there use it
        //otherwise use window
        if(!(element == null)) {
            var hashes = element.attr('href').slice(element.attr('href').indexOf('?') + 1).split('&');
        }
        else {
            var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
        }
        for(var i = 0; i < hashes.length; i++)
        {
            hash = hashes[i].split('=');
            vars.push(hash[0]);
            vars[hash[0]] = hash[1];
        }
        vars.push('query');
        vars['query'] = hashes.join('&');
        return vars;
    }
}(jQuery));