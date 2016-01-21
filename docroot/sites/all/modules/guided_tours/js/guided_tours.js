/**
 * Guided tours object that holds the settings and the methods for the tour
 */
var GuidedTours = {};

(function($) {
  // Execute when DOM is ready
  $(function() {
    // Check if a tour is started
    if (Drupal.settings.GuidedTours) {
      // Extract needed data from objects
      var active_tour = Drupal.settings.GuidedTours['active tour'];
      var current_step = Drupal.settings.GuidedTours['current step'];

      // Check if everything is correct
      if (active_tour === null || current_step < 0 || current_step > active_tour.configuration.steps.length) {
        $.jGrowl(Drupal.t('Configuration of guided tour is invalid!'), {header: Drupal.t('Tour cancelled'), sticky: true});
        return;
      }

      // Dummy for stub data of a tooltip
      var tooltip = null;

      // Counts the amount of tooltips that hit their target
      var hits = 0;

      // Progress calculation
      var progress_by_step = 100 / active_tour.configuration.steps.length;
      var progress_at_step = Math.round(progress_by_step * current_step);
      var progress_by_tooltip = progress_by_step / active_tour.configuration.steps[current_step].tooltips.length;

      // Apply tooltips and count the hits of the selectors on the DOM
      for (var i = 0; i < active_tour.configuration.steps[current_step].tooltips.length; i++) {
        // Apply the tooltip
        tooltip = active_tour.configuration.steps[current_step].tooltips[i];

        GuidedTours.apply_tooltip(active_tour.title, tooltip, progress_at_step + Math.round(progress_by_tooltip * (i + 1)));

        // Check if the selector hit something in the DOM
        if ($(tooltip.selector).length > 0)
          hits++;
      }

      // Output a warning that the tour is out of date and cancel the tour, when not all tooltips hit something
      if (hits < active_tour.configuration.steps[current_step].tooltips.length) {
        // Output the warning dialog
        setTimeout('GuidedTours.broken()', 200);
      }
    }
  });

  /**
   * Return to the last step of the tour.
   */
  GuidedTours.go_back = function() {
    $.ajax({
      'url': Drupal.settings.basePath + 'guided_tours/decrease',
      'dataType': 'json',
      'success': function(answer) {
        if (answer.status === 'success') {
          history.back();
        }
      }
    });
  };

  /**
   * Do something when the tour seems to be broken.
   */
  GuidedTours.broken = function() {
    var content;

    $('.guided-tours-tooltip').remove();

    if (Drupal.settings.GuidedTours['current step'] > 0) {
      content = $('<p />', {
        html: Drupal.t('Something seems to be broken.') + '<br />' + Drupal.t('You can either go back and try again or cancel the tour.')
      }).add($('<button />', {
        text: Drupal.t('Cancel tour'),
        click: function() {
          GuidedTours.cancel_tour();
        },
        class: Drupal.settings.qTipExtensions['button additional classes'],
      })).add($('<button />', {
        text: Drupal.t('Go back & try again'),
        click: function() {
          GuidedTours.go_back();
        },
        class: Drupal.settings.qTipExtensions['button additional classes']
      }));
    } else {
      content = $('<p />', {
        html: Drupal.t('Something seems to be broken.') + '<br />' + Drupal.t('You can either go back and try again or cancel the tour.')
      }).add($('<button />', {
        text: Drupal.t('Cancel tour'),
        click: function() {
          GuidedTours.cancel_tour();
        },
        class: Drupal.settings.qTipExtensions['button additional classes']
      }));
    }

    qTipExtensions.dialog(content, Drupal.t('An error occurred'), true);
  };

  /**
   * Cancel the currently active tour for the user.
   */
  GuidedTours.cancel_tour = function() {
    $.ajax({
      // Try to cancel the tour and notify on success
      url: Drupal.settings.basePath + 'guided_tours/stop',
      dataType: 'json',
      success: function(answer) {
        if (answer.status === 'success') {
          $.jGrowl(Drupal.t('Tour has been cancelled! You can now simply continue using the site.'));
          $('.guided-tours-tooltip').hide();
        }
      }
    });
  };

  /**
   * Confirm if the currently active tour shall be hidden, cancelled or continued.
   * @param api API object of the qtip that has been closed.
   */
  GuidedTours.confirm_cancel = function(api) {
    if (Drupal.settings.GuidedTours['current step'] === Drupal.settings.GuidedTours['active tour'].configuration.steps.length - 1) {
      $.jGrowl(Drupal.t('You already finished the tour by reaching this site. You can now simply continue using the site.'));
      $('.guided-tours-tooltip').hide();
      return;
    }

    var content = $('<p />', {
      text: Drupal.t('Do you want to cancel the tour?')
    }).add($('<button />', {
      text: Drupal.t('Yes'),
      click: function() {
        GuidedTours.cancel_tour();
      },
      class: Drupal.settings.qTipExtensions['button additional classes']
    })).add($('<button />', {
      text: Drupal.t('No (continue)'),
      click: function() {
        // Show the tooltip that was hidden by the user, because he wants to continue the tour
        api.show();
      },
      class: Drupal.settings.qTipExtensions['button additional classes']
    }));

    // Show dialog to the user
    qTipExtensions.dialog(content, Drupal.t('Cancel tour?'));
  };

  /**
   * Apply a tooltip.
   * @param title Title of the tour tooltip, should be the title of the tour.
   * @param data Data of the tooltip.
   * @param progress Integer between 0 and 100
   */
  GuidedTours.apply_tooltip = function(title, data, progress) {
    var classes = Drupal.settings.qTipExtensions['tour additional classes'] + ' ui-tooltip-shadow guided-tours-tooltip';
    // If requested, set the tooltip as fixed
    if (data.fixed === '1')
      classes += ' guided-tours-tooltip-fixed';

    var progress_classes = '';
    if (progress === 100)
      progress_classes = 'guided-tours-completed';
    title = '<div class="guided-tours-progress-bar" title="' + Drupal.t('Progress of guided tour') + '"><div style="width: ' + progress + '%;" class="' + progress_classes + '">&nbsp;</div></div>' + title;

    $(data.selector).qtip({
      content: {
        text: data.content,
        title: {
          text: title,
          button: 'Close'
        }
      },
      position: {
        my: data.position_on_tooltip,
        at: data.position_on_target,
        target: $(data.selector)
      },
      show: {
        event: false,
        ready: true
      },
      hide: false,
      events: {
        hide: function(event, api) {
          GuidedTours.confirm_cancel(api);
        }
      },
      style: {
        classes: classes,
        tip: {
          border: 1,
          height: 20,
          width: 20
        }
      }
    });

    // Enable dynamic behaviour of tooltip
    var api = $(data.selector).qtip('api');
    if (data.dynamic.enabled === '1') {
      $(data.selector).bind(data.dynamic.event, function() {
        api.set('content.text', data.dynamic.content);
        api.set('position.target', $(data.dynamic.selector));
      });
    }

    // Hide tooltip, when it is clicked
    if (data.hide_on_click === '1') {
      $($(data.selector)).click(function() {
        $(api.elements.tooltip).remove();
      });
    }
  };
})(jQuery);