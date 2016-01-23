/**
 * @file
 * Load JavaScript files asynchronously.
 */

(function($) {

  'use strict';

  $(window).load(function() {

    /**
     * Support the forEach method of the Array object in crappy browsers.
     */
    if (!Array.prototype.forEach) {
      Array.prototype.forEach = function(fn, scope) {
        var i, len;
        for(i = 0, len = this.length; i < len; ++i) {
          if (i in this) {
            fn.call(scope, this[i], i, this);
          }
        }
      };
    }

    /**
     * Build async_js object and include async_js settings.
     */
    var async_js = $.extend(true, {

      // Set defaults.
      fade: [],
      javascript: {},
      timeout: 1000,
      totalScripts: 0,
      loadedScripts: 0,

      // Callback to be fired when all scripts are loaded.
      finalCallback: '',

      // Provide delayed fadeIn effect for elements defined in the fade array.
      delayFade: function(async_js) {
        var groupFade;
        async_js.fade.forEach(function(element, index, array) {
          if (index < 1) {
            groupFade = $(element);
          } else {
            groupFade = groupFade.add(element);
          }
        });
        groupFade.fadeIn();
      },

      // Cycle through all defined scripts and process them.
      loadScripts: function(async_js) {
        var addFade = function(element, index, array) {
          async_js.fade.push(element);
        };
        for (var script in async_js.javascript) {
          if (typeof async_js.javascript[script].data !== 'undefined') {

            // Keep track of total number of scripts.
            async_js.totalScripts++;

            // If there are any elements to fade in connected with this script, keep track of them.
            if (async_js.javascript[script].fade.length > 0) {
              async_js.javascript[script].fade.forEach(addFade);
            }

            // Load the script
            async_js.loadScript(async_js, async_js.javascript[script]);
          }
        }
      },

      // Load a script asynchronously.
      loadScript: function(async_js, script) {

        // If our dependencies are not loaded. Do not continue.
        if (!async_js.dependenciesLoaded(async_js, script)) {
          return;
        }

        // If we've already queued this script for loading, don't do it again.
        if (script.queued) {
          return;
        }

        // This script is officially queued for loading.
        script.queued = true;

        (function() {
          var s = document.createElement('script');
          s.type = 'text/javascript';
          s.async = true;
          s.src = script.data;
          s.onload = s.onreadystatechange = function() {
            var rs = this.readyState;
            if (rs && rs !== 'complete' && rs !== 'loaded') {
              return;
            }
            try{

              // Register another loaded script.
              async_js.loadedScripts++;
              script.loaded = true;

              // If we are supposed to fire a callback when this script loads, do it.
              if (typeof script.callbacks == 'object') {
                for (var i = 0, l = script.callbacks.length; i < l; i++) {
                  if ($.isFunction(window[script.callbacks[i]])) {
                    window[script.callbacks[i]]();
                  }
                }
              }

              // If the script has dependents, try to load them.
              if (typeof script.dependents !== 'undefined' && script.dependents.length) {
                script.dependents.forEach(function(element, index, array) {
                  async_js.loadScript(async_js, async_js.javascript[element]);
                });
              }

              // If all scripts have been loaded, fire the final callback.
              if ($.isFunction(window[async_js.finalCallback]) && async_js.loadedScripts === async_js.totalScripts) {
                window[async_js.finalCallback]();
              }

            } catch (e) {
            }
          };
          if (script.container !== null) {
            $(script.container).get(0).appendChild(s);
          } else {
            var x = document.getElementsByTagName('script')[0];
            x.parentNode.insertBefore(s, x);
          }
        })();
      },

      dependenciesLoaded: function(async_js, script) {
        var loaded = true;
        if (script.dependencies.length) {
          script.dependencies.forEach(function(element, index, array) {
            if (!async_js.javascript[element].loaded) {
              loaded = false;
            }
          });
        }
        return loaded;
      }

    }, Drupal.settings.async_js);

    /**
     * Get to it!
     */
    async_js.loadScripts(async_js);
    if (async_js.fade.length > 0) {
      setTimeout(function() {async_js.delayFade(async_js);}, async_js.timeout);
    }

  });

})(jQuery);

