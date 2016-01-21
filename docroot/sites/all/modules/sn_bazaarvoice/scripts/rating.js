/**
 * @file This file is used to represent rating and review boxes.
 */
(function($) {
  $.fn.rating = function(op) {
    var defaults = {
      /** String vars * */
      // path of the icon stars.png
      bigStarsPath : Drupal.settings.basePath + Drupal.settings.sn_bazaarvoice.image_path + '/stars.png',
      // path of the icon small.png
      smallStarsPath : Drupal.settings.basePath + Drupal.settings.sn_bazaarvoice.image_path + '/small.png',
      // can be set to 'small' or 'big'
      type : 'big',

      /** Boolean vars * */
      // if true, mouseover binded star by star,
      step : false,
      isDisabled : false,
      showRateInfo : true,
      canRateAgain : false,

      /** Integer vars * */
      // number of star to display
      length : 5,
      // number of decimals.. Max 3, but you can complete
      decimalLength : 0,
      // maximal rate - integer from 0 to 9999 (or more)
      rateMax : 20,
      // relative position in X axis of the info box when
      rateInfosX : -45,
      // relative position in Y axis of the info box when
      rateInfosY : 5,
      // mouseover
      nbRates : 1,

      /** Functions * */
      onSuccess : null,
      onError : null
    };

    if (this.length > 0) {
      var result = this
          .each(function() {
            /* vars */
            var opts = $.extend(defaults, op), newWidth = 0, starWidth = 0, starHeight = 0, bgPath = '', hasRated = false, globalWidth = 0, nbOfRates = opts.nbRates;

            if ($(this).hasClass('jDisabled') || opts.isDisabled) {
              var jDisabled = true;
            } else {
              var jDisabled = false;
            }

            getStarWidth();
            $(this).height(starHeight);
            // Get the average of all rates
            var average = parseFloat($(this).attr('data-average')),
            // Get the id of the box
            idBox = parseInt($(this).attr('data-id')),
            // Width of the container
            widthRatingContainer = starWidth * opts.length,
            // Width of the color container
            widthColor = average / opts.rateMax * widthRatingContainer,

            quotient = $('<div>', {
              'class' : 'ratingColor',
              css : {
                width : widthColor
              }
            }).appendTo($(this)),

            average = $('<div>', {
              'class' : 'ratingAverage',
              css : {
                width : 0,
                top : -starHeight
              }
            }).appendTo($(this)),

            jstar = $('<div>', {
              'class' : 'jStar',
              css : {
                width : widthRatingContainer,
                height : starHeight,
                top : -(starHeight * 2),
                background : 'url(' + bgPath + ') repeat-x'
              }
            }).appendTo($(this));

            // Adding a variable to store rating.
            $(this).css({
              width : widthRatingContainer,
              overflow : 'hidden',
              zIndex : 1,
              position : 'relative'
            });

            if (!jDisabled)
              $(this).unbind().bind(
                  {
                    mouseenter : function(e) {
                      var realOffsetLeft = findRealLeft(this);
                      var relativeX = e.pageX - realOffsetLeft;
                      if (opts.showRateInfo)
                        var tooltip = $(
                            '<p>',
                            {
                              'class' : 'ratingInfos',
                              html : getNote(relativeX) + ' <span class="maxRate">/ ' + opts.rateMax + '</span>',
                              css : {
                                top : (e.pageY + opts.rateInfosY),
                                left : (e.pageX + opts.rateInfosX)
                              }
                            }).appendTo('body').show();
                    },
                    mouseover : function(e) {
                      $(this).css('cursor', 'pointer');
                    },
                    mouseout : function() {
                      $(this).css('cursor', 'default');
                      if (hasRated) {
                        average.width(globalWidth);
                      } else {
                        average.width(0);
                      }
                    },
                    mousemove : function(e) {
                      var realOffsetLeft = findRealLeft(this);
                      var relativeX = e.pageX - realOffsetLeft;
                      if (opts.step) {
                        newWidth = Math.floor(relativeX / starWidth) * starWidth + starWidth;
                      } else {
                        newWidth = relativeX;
                      }
                      average.width(newWidth);
                      if (opts.showRateInfo) {
                        $("p.ratingInfos").css({
                          left : (e.pageX + opts.rateInfosX)
                        }).html(
                            getNote(newWidth) + ' <span class="maxRate">/ ' + opts.rateMax + '</span>');
                      }
                    },
                    mouseleave : function() {
                      $("p.ratingInfos").remove();
                    },
                    click : function(e) {
                      var element = this;

                      /* set vars */
                      hasRated = true;
                      globalWidth = newWidth;
                      nbOfRates--;

                      if (!opts.canRateAgain || parseInt(nbOfRates) <= 0) {
                        $(this).unbind().css('cursor', 'default').addClass(
                            'jDisabled');
                      }
                      if (opts.showRateInfo) {
                        $("p.ratingInfos").fadeOut('fast', function() {
                          $(this).remove();
                        });
                      }
                      e.preventDefault();
                      var rate = getNote(newWidth);
                      average.width(newWidth);

                      $('.rating-value').attr('value', rate);

                    }
                  });

            function getNote(relativeX) {
              var noteBrut = parseFloat((relativeX * 100 / widthRatingContainer) * opts.rateMax / 100);
              switch (opts.decimalLength) {
              case 1:
                var note = Math.round(noteBrut * 10) / 10;
                break;

              case 2:
                var note = Math.round(noteBrut * 100) / 100;
                break;

              case 3:
                var note = Math.round(noteBrut * 1000) / 1000;
                break;

              default:
                var note = Math.round(noteBrut * 1) / 1;
              }
              return note;
            }

            function getStarWidth() {
              switch (opts.type) {
              case 'small':
                starWidth = 12;
                // Width of the picture small.png
                starHeight = 10;
                // Height of the picture small.png
                bgPath = opts.smallStarsPath;
                break;

              default:
                starWidth = 23;
                // Width of the picture stars.png
                starHeight = 20;
                // Height of the picture stars.png
                bgPath = opts.bigStarsPath;
              }
            }

            function findRealLeft(obj) {
              if (!obj) {
                return 0;
              }
              return obj.offsetLeft + findRealLeft(obj.offsetParent);
            }

          });

      return result;
    }
    return '';
  }

  Drupal.behaviors.snBazaarVoice = {
    attach : function(context, settings) {

      $('.rating-widget', context).rating({
        showRateInfo : false,
        rateMax : 5,
        isDisabled : true
      });
      $('.rating-widget-active', context).rating({
        showRateInfo : false,
        step : true,
        rateMax : 5,
        isDisabled : false
      });
    }
  };

})(jQuery);
