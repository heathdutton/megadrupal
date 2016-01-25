(function ($) {

    Drupal.behaviors.viewsNivoSliderBehavior = {
        attach: function (context) {
            $('.views-nivo-slider', context).once('nivo-slider', function () {
                var vns = $(this);
                var id = vns.attr('id');
                var cfg = Drupal.settings.views_nivo_slider[id];

                // Fix sizes
                vns.data('hmax', 0).data('wmax', 0);
                $('img', vns).each(function () {
                    $(this).load(function () {
                        hmax = (vns.data('hmax') > $(this).height()) ? vns.data('hmax') : $(this).height();
                        wmax = (vns.data('wmax') > $(this).width()) ? vns.data('wmax') : $(this).width();

                        vns.width(wmax).height(hmax).data('hmax', hmax).data('wmax', wmax);
                    });
                });

                // eval vars as functions
                if (cfg['beforeChange']) {
                    eval("cfg['beforeChange'] = " + cfg['beforeChange']);
                }
                if (cfg['afterChange']) {
                    eval("cfg['afterChange'] = " + cfg['afterChange']);
                }
                if (cfg['slideshowEnd']) {
                    eval("cfg['slideshowEnd'] = " + cfg['slideshowEnd']);
                }
                if (cfg['lastSlide']) {
                   eval("cfg['lastSlide'] = " + cfg['lastSlide']);
                }
                if (cfg['afterLoad']) {
                    eval("cfg['afterLoad'] = " + cfg['afterLoad']);
                }

                vns.nivoSlider(cfg);
            });
        }
    };

})(jQuery);

