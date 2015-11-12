/**
 * @file
 * A JavaScript file for the module.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - http://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {

  // Wrapper
  $(function() {

    ;(function(){

      var widthPx, heightPx, widthEm, heightEm;

      var init = function(){

        // Inject the Div
        setupFitWeird();

        // Gather elements to update
        widthPx = document.getElementById('fitweird-width-px');
        heightPx = document.getElementById('fitweird-height-px');
        widthEm = document.getElementById('fitweird-width-em');
        heightEm = document.getElementById('fitweird-height-em');

        // Fill the Div
        fillVars();

        // Update the Div
        window.addEventListener('resize', fillVars);

      }

      var fillVars = function(){
        widthPx.innerHTML = window.innerWidth;
        heightPx.innerHTML = window.innerHeight;

        // TODO: Calc EM size based off of body font size (i.e user zooming)
        widthEm.innerHTML = window.innerWidth / 16;
        heightEm.innerHTML = window.innerHeight / 16;

      }

      var setupFitWeird = function(){

        if ( !document.getElementById('fitweird-width-px') ) {
          var newDiv = document.createElement('div');
          var newContent = '<span id=fitweird-width-px></span>px &times; <span id=fitweird-height-px></span>px ';
          newContent += ':: <span id=fitweird-width-em></span>em &times; <span id=fitweird-height-em></span>em';
          newDiv.style.position = 'fixed';
          newDiv.style.bottom = '0';
          newDiv.style.right = '0';
          newDiv.style.backgroundColor = 'rgba(58, 58, 58, 0.8)';
          newDiv.style.padding = '0.4em 1em';
          newDiv.style.color = '#00CC00';
          newDiv.style.fontFamily = 'monospace';
          newDiv.style.zIndex = '9999';
          newDiv.innerHTML = newContent;
          document.body.appendChild(newDiv);
        }

      };

      init();

    })(window);

  });

})(jQuery, Drupal, this, this.document);
