(function($) {
  Drupal.behaviors.walkthru = {
    attach: function() {
      // Find all walkthru links and make them open their walkthru's
      $('a.walkthru-link').each(function() {
        var walkthruName = $(this).attr('name');
        $(this).click(function() {
          Drupal.walkthru.load(walkthruName);
          return false;
        });
      });

      // Check if the walkthru was loaded on the last refresh.
      if (Drupal.settings.walkthru.continue) {
        Drupal.walkthru.load(Drupal.settings.walkthru.name);
      }
    }
  }

  Drupal.walkthru = Drupal.walkthru || {};

  Drupal.settings.walkthru = Drupal.settings.walkthru || {};

  // Load a walkthru
  Drupal.walkthru.load = function(walkthru_name) {
    // Get the load path
    var path = Drupal.settings.basePath + 'walkthru/ajax/load';
    if (typeof walkthru_name !== 'undefined') {
      path += '/' + walkthru_name;
    }

    // Make sure their isn't already one loading.
    if (!Drupal.settings.walkthru.loading) {
      Drupal.settings.walkthru.loading = true;

      $.getJSON(path, function(data) {

        // We've completed the loading.
        Drupal.settings.walkthru.loading = false;
        if (data) {
          Drupal.settings.walkthru.data = data;

          // If there is a previous step then load that one.
          if (Drupal.settings.walkthru.continue) {
            Drupal.walkthru.minimize(Drupal.settings.walkthru.step_name);
          }
          // Otherwise load the first step.
          else {
            var currentStep = '';
            $.each(data, function(i, step) {
              currentStep = step.id;
              return false;
            });
            Drupal.walkthru.show(currentStep);
          }
        }
      });
    }
  }

  // Hide the currently open walkthru
  Drupal.walkthru.hide = function() {
    $.get(Drupal.settings.basePath + 'walkthru/ajax/close');

    // Hide the overlay
    if ($('#guider_overlay').length) {
      $('#guider_overlay').remove();
    }

    // Hide the last window.
    $('#' + Drupal.settings.walkthru.step_name).remove();

    // Remove the step_name variable
    delete Drupal.settings.walkthru.step_name;
    Drupal.settings.walkthru.continue = false;
  }

  // Move to step
  Drupal.walkthru.show = function(step_name) {
    // Get the step data
    var step = Drupal.settings.walkthru.data[step_name];

    var validated = true;
    // If validation needs to be done on the url then do it.
    if (typeof step.force_url !== 'undefined') {
      validated = Drupal.walkthru.validateURL(step);
    }

    var current_step_name = Drupal.settings.walkthru.step_name;

    // Check if everything is validated.
    if (validated) {
      $.get(Drupal.settings.basePath + 'walkthru/ajax/current/' + step_name);
      Drupal.settings.walkthru.step_name = step_name;

      if (typeof step.overlay == 'undefined' && $('#guider_overlay').length) {
        $('#guider_overlay').remove();
      }

      // Show the step.
      guiders.createGuider(step).show();

      // Hide the last window if there is one.
      if (typeof current_step_name !== 'undefined' && current_step_name !== step_name) {
        $('#' + current_step_name).remove();
      }
    }
    else {
      if (current_step_name) {
        $('#' + current_step_name + ' .guider_description').before('<div>' + step.force_url_error_message + '</div>');
      }
      else {
        Drupal.walkthru.errorBox(step);
      }
    }
  }

  // Minimize the current step.
  Drupal.walkthru.minimize = function(step_name) {
    // Hide the overlay
    if ($('#guider_overlay').length) {
      $('#guider_overlay').remove();
    }

    // Hide the last window.
    $('#' + Drupal.settings.walkthru.step_name).remove();

    // Set the step_name
    Drupal.settings.walkthru.step_name = step_name;

    // Attach our minimize div.
    $('body').prepend('<div id="walkthru-minimize">Click here to restore your walkthru.</div>');
    $.get(Drupal.settings.basePath + 'walkthru/ajax/minimize/true');

    // On click restore the last step of the walkthru.
    $('#walkthru-minimize').click(function() {
      $.get(Drupal.settings.basePath + 'walkthru/ajax/minimize/false');
      $('#walkthru-minimize').remove();
      Drupal.walkthru.show(Drupal.settings.walkthru.step_name);
    });
  }

  // Validate that the step is allowed at the current url.
  Drupal.walkthru.validateURL = function(step) {
    // Get the current url
    var currentURL = window.location.pathname;

    var regex = Drupal.settings.basePath + step.force_url;
    // Replace all * wildcards with the regex version.
    regex = regex.replace(/\*/g, "[^ ]*");
    var re = new RegExp(regex);
    var match = currentURL.match(re);

    // If there is a match and the match is the same as the current url then we're good.
    if (match && match[0] === currentURL) {
      return true;
    }
    // Otherwise create an error box.
    else {
      return false;
    }
  }

  // Show an error box
  Drupal.walkthru.errorBox = function(step) {
    errorBox = new Object();
    errorBox.title = "Error";
    errorBox.description = step.force_url_error_message;
    errorBox.buttons = [{"name": "Close", "onclick": "Drupal.walkthru.hide();"}];

    guiders.createGuider(errorBox).show();
  }

})(jQuery);
