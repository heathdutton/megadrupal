(function ($) {
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','//www.google-analytics.com/analytics.js','ga');

  ga('create', Drupal.settings.personalize_ga.trackingId, 'auto', {'name': 'personalize_ga'});

  // Ensure we only send an event / add the listener once.
  var processedEvents = {}, processedListeners = {};

  /**
   * Adds client side integration between personalize modules and Google Analytics.
   */
  Drupal.behaviors.personalize_ga = {
    attach: function (context, settings) {
      trackPersonalizedDecisions(settings);
      addActionListener(settings);
    }
  };

  /**
   * Track personalized decisions events.
   */
  function trackPersonalizedDecisions(settings) {
    if (settings.personalize.hasOwnProperty('adminMode') || Drupal.settings.personalize_ga.sendPersonalizeDecisions === 'none') {
      return;
    }

    $(document).bind('personalizeDecision', function(event, $option_set, chosen_option, osid) {
      // Handle Decisions
      var category = 'Personalize',
        label = Drupal.settings.personalize.option_sets[osid].label,
        agent = Drupal.settings.personalize.option_sets[osid].agent,
        decision = agent + ': ' + label,
        decisionId = decision + '--' + chosen_option;

      if (processedEvents[decisionId] ||
          Drupal.settings.personalize_ga.sendPersonalizeDecisions === 'some' && !Drupal.settings.personalize_ga.trackedPersonalizeDecisions.hasOwnProperty(osid)
      ) {
        return;
      }

      ga('personalize_ga.send', 'event', category, decision, chosen_option, 1);
      processedEvents[decisionId] = 1;
    });
  }

  /**
   * Add an action listener for client-side goal actions.
   */
  function addActionListener(settings) {
    if (settings.personalize.hasOwnProperty('adminMode') || !Drupal.hasOwnProperty('visitorActions') || Drupal.settings.personalize_ga.sendVisitorActions === 'none') {
      return;
    }

    var actions = {}, hasAction = false;
    for (var action in settings.personalize.actionListeners) {
      if (!settings.personalize.actionListeners.hasOwnProperty(action) || processedListeners.hasOwnProperty(action) ||
          Drupal.settings.personalize_ga.sendVisitorActions === 'some' && !Drupal.settings.personalize_ga.trackedVisitorActions.hasOwnProperty(action)
      ) {
        continue;
      }
      actions[action] = settings.personalize.actionListeners[action];
      processedListeners[action] = 1;
      hasAction = true;
    }

    if (!hasAction) {
      return;
    }

    var category = 'Visitor Actions',
      callback = function(action, jsEvent) {
      if (actions.hasOwnProperty(action)) {
        ga('personalize_ga.send', 'event', category, action, action, 1);
      }
    };
    Drupal.visitorActions.publisher.subscribe(callback);
  }

})(jQuery);
