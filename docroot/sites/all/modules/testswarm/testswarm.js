(function ($) {
  Drupal.behaviors.testswarm = {
    attach: function (context) {
      mySettings = Drupal.settings.testswarm;
      
      QUnit.config.reorder = false; // Never ever re-order tests!
      QUnit.config.altertitle = false; // Don't change the title
      QUnit.config.autostart = false;

      var logger = {log: {}, info: {}, tests: []};
      var currentModule = 'default';

      QUnit.moduleStart = function(module) {
        currentModule = module.name;
        if (!logger.log[currentModule]) {
          logger.log[currentModule] = {};
        }
      };

      QUnit.testStart = function(test) {
        currentTest = test.name;
      };

      QUnit.testDone = function(test) {
        logger.tests.push(test);
      }

      QUnit.done = function(data) {
        logger.info = data;
        logger.caller = mySettings.caller;
        logger.theme = mySettings.theme;
        logger.token = mySettings.token;
        logger.karma = mySettings.karma;

        // Write back to server
        jQuery.ajax({
          type: "POST",
          url: "/testswarm-test-done",
          timeout: 10000,
          data: logger,
          error: function() {
            alert(Drupal.t("Error connecting to server"));
          },
          success: function(){
            if (!mySettings.debug || mySettings.debug != 'on') {
              if (mySettings.destination) {
                window.location = mySettings.destination;
              }
              else {
                window.location = '/testswarm-browser-tests';
              }
            }
          }
        });
      };
      QUnit.log = function(data) {
        if (!logger.log[currentModule]) {
          logger.log[currentModule] = {};
        }
        if (!logger.log[currentModule][currentTest]) {
          logger.log[currentModule][currentTest] = [];
        }
        logger.log[currentModule][currentTest].push(data);
      };
    }
  };
})(jQuery);