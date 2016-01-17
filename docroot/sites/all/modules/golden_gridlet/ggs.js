(function ($) {

  Drupal.behaviors.golden_gridlet = {
    attach: function (context, settings) {
      var guideColor = Drupal.settings.golden_gridlet.guideColor;
      var guideInnerColor = Drupal.settings.golden_gridlet.guideInnerColor;
      var guideOpacity = Drupal.settings.golden_gridlet.guideOpacity;
      var switchColor = Drupal.settings.golden_gridlet.switchColor;
      var switchOpacity = Drupal.settings.golden_gridlet.switchOpacity;
      var baseFontSize = Drupal.settings.golden_gridlet.baseFontSize;
      var baselineGridHeight = Drupal.settings.golden_gridlet.baselineGridHeight+'em';
      var eightColBreakpoint = Drupal.settings.golden_gridlet.eightColBreakpoint+'em';
      var sixteenColBreakpoint = Drupal.settings.golden_gridlet.sixteenColBreakpoint+'em';
      var gutterSize = Drupal.settings.golden_gridlet.gutterSize;

      /*
      * Note that the script might not work as expected if
      * the <body> element of your page has a set width and
      * position: relative;, because the guides are appended
      * inside <body>, but positioned in relation to <html>.
      *
      * Also note that the baseline grid doesn't really align
      * up anymore after zooming the baseline grid in or out,
      * because of rounding errors.
      */

      function setHeights() {
//        if ($('body').hasClass('ggs-hidden')) {

          var docHeight = $(document).height();

          /* Give guides the new height */
          $('.ggs-guide').each(function() {
            $(this).css('height', docHeight);
          });

          /* Calculate the amount of lines needed and append them */
          var lines = Math.floor(docHeight/24);
          $('#ggs-baseline-container').empty();
          for (var i=0; i<=lines; i++) {
            $('#ggs-baseline-container').append('<div class="ggs-line"></div>');
          }

          /* Set the baseline container to the same height as the guides, so there's no overflow */
          $('#ggs-baseline-container').css('height', docHeight);
  //      }
      }

      $(document).ready(function () {

        /* Add control classes and switch element */
        $('body').addClass('ggs-hidden ggs-animated').append('<div id="ggs-switch"><div class="ggs-switchBar"></div><div class="ggs-switchBar"></div><div class="ggs-switchBar"></div></div>');

        /* Create CSS */
        var styles = '\
          html{height:100%;position:relative;}\
          #ggs-switch{position:fixed;top:20px;right:0;z-index:9500; cursor:pointer; width: 24px; padding: 18px 18px 14px; opacity:'+switchOpacity+'; -webkit-transform: rotate(-90deg); -moz-transform: rotate(-90deg); -ms-transform: rotate(-90deg); transform: rotate(-90deg); -webkit-transition: all 0.145s ease-out; -moz-transition: all 0.145s ease-out; -ms-transition: all 0.145s ease-out; transition: all 0.145s ease-out;}\
          .ggs-switchBar {background: '+switchColor+'; height: 4px; margin-bottom: 4px;}\
          .ggs-animated #ggs-switch {-webkit-transform: rotate(0deg); -moz-transform: rotate(0deg); transform: rotate(0deg);}\
          .ggs-guide{position:absolute;top:0;z-index:9000;height:100%;margin-left:-'+(gutterSize/2)+'em;border:solid '+guideColor+';border-width:0 '+(gutterSize/2)+'em;background:'+guideColor+';opacity:'+guideOpacity+'; -webkit-transition: all 0.235s ease-out; -moz-transition: all 0.235s ease-out; -ms-transition: all 0.235s ease-out; transition: all 0.235s ease-out;}\
          .ggs-animated .ggs-guide {-webkit-transform: scale(0, 1); -moz-transform: scale(0, 1); -ms-transform: scale(0, 1); transform: scale(0, 1); opacity: 0;}\
          .ggs-animated #ggs-baseline-container {opacity: 0;}\
          .ggs-hidden .ggs-guide, .ggs-hidden #ggs-baseline-container {display: none;}\
          .ggs-0{left:0;}\
          .ggs-1{left:11.11111111111111%;}\
          .ggs-2{left:16.666666666666664%;}\
          .ggs-3{left:22.22222222222222%;}\
          .ggs-4{left:27.77777777777778%;}\
          .ggs-5{left:33.33333333333333%;}\
          .ggs-6{left:38.888888888888886%;}\
          .ggs-7{left:44.44444444444444%;}\
          .ggs-8{left:50%;}\
          .ggs-9{left:55.55555555555556%;}\
          .ggs-10{left:61.11111111111111%;}\
          .ggs-11{left:66.66666666666666%;}\
          .ggs-12{left:72.22222222222221%;}\
          .ggs-13{left:77.77777777777777%;}\
          .ggs-14{left:83.33333333333333%;}\
          .ggs-15{left:88.88888888888889%;}\
          .ggs-16{right:0;}\
          .ggs-0,.ggs-16{width:5.555555555555555%;padding-right:0.75em;border:0;margin:0;}\
          .ggs-guide div{background:'+guideInnerColor+';width:2px;height:100%;position:absolute;left:-1px;top:0;}\
          .ggs-0 div{left:auto;right:0.75em;}\
          .ggs-16 div{left:0.75em;}\
          #ggs-baseline-container {opacity: '+guideOpacity+'; position: absolute; left:0; top:0; z-index: 8000; width: 100%; height: 100%; -webkit-transition: opacity 0.235s ease-out; -moz-transition: opacity 0.235s ease-out; -ms-transition: opacity 0.235s ease-out; transition: opacity 0.235s ease-out; overflow-y: hidden;}\
          .ggs-line {border-top: 1px dotted '+guideColor+'; height: '+baselineGridHeight+'; -webkit-box-sizing: border-box; -moz-box-sizing: border-box; -o-box-sizing: border-box; box-sizing: border-box;}\
          @media screen and (max-width: '+(eightColBreakpoint)+'){.ggs-2,.ggs-6,.ggs-10,.ggs-14{display:none;}}\
          @media screen and (max-width: '+(sixteenColBreakpoint)+'){.ggs-1,.ggs-3,.ggs-5,.ggs-7,.ggs-9,.ggs-11,.ggs-13,.ggs-15{display:none;}}\
        ';

        /* Create guides */
        for (var i=0; i<=16; i++) {
          $('body').append($('<div class="ggs-guide ggs-'+i+'"><div></div></div>'));
        };
        $('body').append($('<div id="ggs-baseline-container"></div>'));

        /* Append CSS */
        $('head').append('<style type="text/css">' + styles + '</style>');

        /* Resize guides when window size changes */
        $(window).resize(setHeights);

        /* Add listeners for switch element */
        $('#ggs-switch').click(function(){
          if ($('body').hasClass('ggs-hidden')) {
            $('body').removeClass('ggs-hidden');
            setHeights();
            setTimeout(
              function () {
                $('body').removeClass('ggs-animated');
              },
              20
            );
          }
          else {
            $('body').addClass('ggs-animated');
            setTimeout(
              function () {
                $('body').addClass('ggs-hidden');
              },
              300
            );
          }
        });
      });
    }
  };
}(jQuery));
