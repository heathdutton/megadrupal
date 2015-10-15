/**
 * FloatLabels
 * Version: 1.0-patched-for-D7
 * URL: http://clubdesign.github.io/floatlabels.js/
 * Description: 
 * Author: Marcus Pohorely ( http://www.clubdesign.at )
 * Copyright: Copyright 2013 / 2014 http://www.clubdesign.at
 */

;(function ( $, window, document, undefined ) {

        var pluginName = "floatlabel",
            defaults = {
                slideInput                      : true,
                labelStartTop                   : '20px',
                labelEndTop                     : '10px',
                transitionDuration              : 0.3,
                transitionEasing                : 'ease-in-out',
                labelClass                      : '',
                typeMatches                     : /text|password|email|number|search|url/
            };

        function Plugin ( element, options ) {
            
            this.$element       = $(element);
            this.settings       = $.extend( {}, defaults, options );

            this.init();
        
        }

        Plugin.prototype = {

            init: function () {

                var self          = this,
                    settings      = this.settings,
                    transDuration = settings.transitionDuration,
                    transEasing   = settings.transitionEasing,
                    thisElement   = this.$element;
                
                var animationCss = {
                    '-webkit-transition'            : 'all ' + transDuration + 's ' + transEasing,
                    '-moz-transition'               : 'all ' + transDuration + 's ' + transEasing,
                    '-o-transition'                 : 'all ' + transDuration + 's ' + transEasing,
                    '-ms-transition'                : 'all ' + transDuration + 's ' + transEasing,
                    'transition'                    : 'all ' + transDuration + 's ' + transEasing
                };

                // RdB: comment out as we wish to target <textarea> and 
                // <select multiple="multiple"> as well.
                //if( thisElement.attr('tagName').toUpperCase() !== 'INPUT' ) { return; }
                //if( !settings.typeMatches.test( thisElement.attr('type') ) ) { return; }

                

                var elementID = thisElement.attr('id');

                if( !elementID ) {
                    elementID = Math.floor( Math.random() * 100 ) + 1;
                    thisElement.attr('id', elementID);
                }

                var placeholderText     = thisElement.attr('placeholder');
                var floatingText        = thisElement.data('label');
                var extraClasses        = thisElement.data('class');

                if( !extraClasses ) { extraClasses = ''; }

                if( !placeholderText || placeholderText === '' ) { placeholderText = "You forgot to add placeholder attribute!"; }
                if( !floatingText || floatingText === '' ) { floatingText = placeholderText; }

                this.inputPaddingTop    = parseFloat( thisElement.css('padding-top') ) + 10;

                thisElement.wrap('<div class="floatlabel-wrapper" style="position:relative"></div>');
                thisElement.before('<label for="' + elementID + '" class="label-floatlabel ' + settings.labelClass + ' ' + extraClasses + '">' + floatingText + '</label>');

                this.$label = thisElement.prev('label');
                this.$label.css({
                    'position'                      : 'absolute',
                    'top'                           : settings.labelStartTop,
                    'left'                          : thisElement.css('padding-left'),
                    'display'                       : 'none',
                    '-moz-opacity'                  : '0',
                    '-khtml-opacity'                : '0',
                    '-webkit-opacity'               : '0',
                    'opacity'                       : '0'
                });

                if( !settings.slideInput ) {
                    
                    thisElement.css({
                        'padding-top'                   : this.inputPaddingTop,
                    });

                }

                // RdB: use bind(), not on(), for compatibility with jQuery 1.4.x
                thisElement.bind('keyup blur change', function( e ) {
                    self.checkValue( e );
                });


                window.setTimeout( function() {

                    self.$label.css( animationCss );
                    self.$element.css( animationCss );

                }, 100);

                this.checkValue();

            },

            checkValue: function( e ) {

                if( e ) {

                    var keyCode         = e.keyCode || e.which;
                    if( keyCode === 9 ) { return; }
                
                }

                var thisElement  = this.$element, 
                    currentFlout = thisElement.data('flout');

                if( thisElement.val() !== "" ) { thisElement.data('flout', '1'); }
                if( thisElement.val() === "" ) { thisElement.data('flout', '0'); }



                if( thisElement.data('flout') === '1' && currentFlout !== '1' ) {
                    this.showLabel();
                }

                if( thisElement.data('flout') === '0' && currentFlout !== '0' ) {
                    this.hideLabel();
                }

            },
            showLabel: function() {

                var self = this;

                self.$label.css({
                    'display'                       : 'block'
                });

                window.setTimeout(function() {

                    self.$label.css({
                        'top'                           : self.settings.labelEndTop,
                        '-moz-opacity'                  : '1',
                        '-khtml-opacity'                : '1',
                        '-webkit-opacity'               : '1',
                        'opacity'                       : '1'
                    });

                    if( self.settings.slideInput ) {

                        self.$element.css({
                            'padding-top'               : self.inputPaddingTop
                        });

                    }

                }, 50);

            },

            hideLabel: function() {

                var self = this;

                self.$label.css({
                    'top'                           : self.settings.labelStartTop,
                    '-moz-opacity'                  : '0',
                    '-khtml-opacity'                : '0',
                    '-webkit-opacity'               : '0',
                    'opacity'                       : '0'
                });

                if( self.settings.slideInput ) {

                    self.$element.css({
                        'padding-top'               : parseFloat( self.inputPaddingTop ) - 10
                    });

                }

                window.setTimeout(function() {
                    self.$label.css({
                        'display'                       : 'none'
                    });
                }, self.settings.transitionDuration * 1000);

            }
        };

        $.fn[ pluginName ] = function ( options ) {
            return this.each(function() {
                if ( !$.data( this, "plugin_" + pluginName ) ) {
                    $.data( this, "plugin_" + pluginName, new Plugin( this, options ) );
                }
            });
        };

})( jQuery, window, document );