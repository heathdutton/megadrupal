(function ($) {
    'use strict';
    Drupal.behaviors.wmToolbar = {};
    Drupal.behaviors.wmToolbar.attach = function (context) {
        $('#wm-toolbar:not(.processed)').each(function () {
            var toolbar = $(this);
            toolbar.addClass('processed');

            // Set initial toolbar state.
            Drupal.wmToolbar.init(toolbar);

            // Admin toggle.
            $('.wm-toggle-button', this).click(function () {
                Drupal.wmToolbar.toggle(toolbar);
            });
            
            $(window).resize(function () {
                Drupal.wmToolbar.scroll(toolbar);
            });
        });

        //Drupal.wmToolbar.scroll(toolbar);
        $('.scroll-pane .item').hover(function () {
            $(this).addClass('hover');
        }, function () {
            $(this).removeClass('hover');
        });

    };

    /**
     * Wm toolbar methods.
     */
    Drupal.wmToolbar = {};

    /**
     * The width of the toolbar.
     */
    Drupal.wmToolbar.SIZE_GIVEN = 260;

    /**
     * Set the initial state of the toolbar.
     */
    Drupal.wmToolbar.init = function (toolbar) {
        // Set expanded state.
        var expanded = this.getState('expanded');
        if (parseInt(expanded, 10) === 1) {
            $(document.body).addClass('wm-expanded');
            Drupal.wmToolbar.scroll(toolbar);
        } else {
            $(document.body).addClass('wm-collapsed');
        }

        // prevent scroll over borders
        $('.scroll-pane').mousewheel(function (event) {
            event.preventDefault();
        });

        // reset filter on focus
        $('#filter').focus(function () {
            $('input#filter').val('');
        });

        // filter
        $('#filter').keyup(function (event) {
            $('.scroll-pane li').each(function () {
                $(this).show();
            });

            var t = $('input#filter').val().toLowerCase();
            $('.scroll-pane li.item').each(function () {
                var name = $('.item-text a', this).html().toLowerCase(),
                    header;
                if (name.indexOf(t) === -1) {
                    $(this).hide();
                    header = $('.header', $(this).parent());
                    if ($('li:visible', $(this).parent()).size() === 1) {
                        header.hide();
                    }
                }
            });

            event.preventDefault();
        });

        // set tooltip
        $('#wm-toolbar a[title]').qtip({
            position: {
                my: 'left center',
                at: 'right center'
            }
        });
    };


    Drupal.wmToolbar.scroll = function (toolbar) {
        $('#wm-toolbar').css('height', $(window).height());
        $('.scroll-pane').css('height', ($(window).height() - $('.non-scroll-pane').height() - $('.wm-toggle').height()));
        $('.scroll-pane').jScrollPane();
    };

    /**
     * Toggle the toolbar open or closed.
     */
    Drupal.wmToolbar.toggle = function (toolbar) {
        var size = '30px';
        if ($(document.body).is('.wm-expanded')) {
            $('div.wm-blocks', toolbar).animate({
                width: size
            }, 'fast');
            $(document.body).animate({
                marginLeft: size
            }, 'fast', function () {
                $(this).toggleClass('wm-expanded');
                $(this).toggleClass('wm-collapsed');
            });


            this.setState('expanded', 0);
        } else {
            size = this.SIZE_GIVEN + 'px';
            
            $('div.wm-blocks', toolbar).animate({
                width: size
            }, 'fast');

            $(document.body).animate({
                marginLeft: size
            }, 'fast', function () {
                $(this).toggleClass('wm-expanded');
                $(this).toggleClass('wm-collapsed');

                // add scrollbars
                Drupal.wmToolbar.scroll(toolbar);
            });

            if ($(document.body).hasClass('wm-ah')) {
                this.setState('expanded', 0);
            } else {
                this.setState('expanded', 1);
            }
        }
    };

    /**
     * Get the value of a cookie variable.
     */
    Drupal.wmToolbar.getState = function (key) {
        if (!Drupal.wmToolbar.state) {
            Drupal.wmToolbar.state = {};
            var cookie = $.cookie('DrupalWmToolbar'),
                query = cookie ? cookie.split('&') : [],
                i,
                values;
            if (query) {
                for (i in query) {
                    if (query.hasOwnProperty(i)) {
                        // Extra check to avoid js errors in Chrome, IE and Safari when
                        // combined with JS like twitter's widget.js.
                        // See http://drupal.org/node/798764.
                        if (typeof (query[i]) === typeof ('string') && query[i].indexOf('=') !== -1) {
                            values = query[i].split('=');
                            if (values.length === 2) {
                                Drupal.wmToolbar.state[values[0]] = values[1];
                            }
                        }
                    }
                }
            }
        }
        return Drupal.wmToolbar.state[key] || false;
    };

    /**
     * Set the value of a cookie variable.
     */
    Drupal.wmToolbar.setState = function (key, value) {
        var existing = Drupal.wmToolbar.getState(key),
            query = [],
            i;
        if (existing !== value) {
            Drupal.wmToolbar.state[key] = value;
            for (i in Drupal.wmToolbar.state) {
                if (Drupal.wmToolbar.state.hasOwnProperty(i)) {
                    query.push(i + '=' + Drupal.wmToolbar.state[i]);
                }
            }
            $.cookie('DrupalWmToolbar', query.join('&'), {
                expires: 7,
                path: '/'
            });
        }
    };

}(jQuery));
