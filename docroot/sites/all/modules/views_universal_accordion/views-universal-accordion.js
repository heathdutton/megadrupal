Drupal.behaviors.views_universal_accordion = {
    attach: function (context, settings) {
        (function ($) {
            $.each(settings.views_universal_accordion, function (id) {

                /* the selectors we have to play with */
                var displaySelector = id + ' .view-content .top-slider';
                var tabWidth = this.tabwidth;
                var localspeed = parseInt(this.speed);
                var localtimeout = parseInt(this.timeout);
                var lochorizontal = (this.horizontal == 1 ? true : false);
		var loceasing = (this.easing == 'none' ? null : this.easing);
                $(displaySelector, context).uAccordion({
                    horizontal: lochorizontal,
                    easing: loceasing,
                    invert: this.invert,
                    errors: this.errors,
                    pause: this.pause,
                    tabOffset: this.tabwidth,
                    trigger: this.trigger,
                    auto: this.autoplay,
                    speed: localspeed,
                    timeout: localtimeout,
                    slideClass: 'slider',
                    buildComplete: function () {
                        var pLeft = parseInt($(displaySelector, context).find('div.wrapper').css('padding-left'));
                        var pRight = parseInt($(displaySelector, context).find('div.wrapper').css('padding-right'));
                        $(displaySelector, context).find('li.slider-closed div.wrapper').width(tabWidth - pLeft - pRight);
                    },
                    animationStart: function () {
                        $(displaySelector, context).find('li.slider-open div.wrapper').css('display', 'none');
                        $(displaySelector, context).find('li.slider-previous div.wrapper').css('display', 'none');
                    },
                    animationComplete: function () {
                        var pLeft = parseInt($(this).find('div.wrapper').css('padding-left'));
                        var pRight = parseInt($(this).find('div.wrapper').css('padding-right'));
                        $(this).siblings('li.slider-closed').find('div.wrapper').width(tabWidth - pLeft - pRight);
                        $(this).find('div.wrapper').width($(this).width() - pLeft - pRight);
                        $(displaySelector, context).find('li div.wrapper').fadeIn(600);
                    },
                    width: this.totalwidth,
                    height: this.totalheight
                });
            });
        })(jQuery);
    }
};
